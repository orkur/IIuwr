// Jakub Gałaszewski
// 332042
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <regex.h>
#include <poll.h>
#include <sys/time.h>

u_int16_t compute_icmp_checksum (const void *buff, int length)
{
    const u_int16_t* ptr = buff;
    u_int32_t sum = 0;
    assert (length % 2 == 0);
    for (; length > 0; length -= 2)
        sum += *ptr++;
    sum = (sum >> 16U) + (sum & 0xffffU);
    return (u_int16_t)(~(sum + (sum >> 16U)));
}
void print_as_bytes (unsigned char* buff, ssize_t length)
{
	for (ssize_t i = 0; i < length; i++, buff++)
		printf ("%.2x ", *buff);	
}
int verify_input(const char *ip_addr, int verbose){
    int number = -1;
    int parts = 0;
    for(const char *i = ip_addr; *i != '\0'; i++)
    {
        if(verbose) printf("%c\n", *i);
        if(*i == '.'){
            if(number > 255) return 1;
            number = -1;
            continue;
        }
        if(*i - '0' < 0 || *i - '0' > 9) return 1;
        if(number == -1){
            number = 0;
            parts++;
        }
        number = (number*10 + (*i - '0'));
        if(verbose) printf("%d <- \n", number);
    }
    if(parts != 4 || number > 255) return 1;
    return 0;
}
struct icmp make_header(const int *TTL,const int *seq){
    struct icmp header;
    header.icmp_type = ICMP_ECHO;
    header.icmp_code = 0;
    header.icmp_id = *TTL;
    header.icmp_seq = *seq;
    header.icmp_cksum = 0;
    header.icmp_cksum = compute_icmp_checksum (
        (u_int16_t*)&header, sizeof(header));
    return header;    
}
struct pollfd make_ps(const int *sock_fd){
    struct pollfd ps;
    ps.fd = *sock_fd;
    ps.events = POLLIN;
    ps.revents = 0;
    return ps;
}
long long int time_delta(const struct timeval *start,const struct timeval *end){
    return (end->tv_sec - start->tv_sec) * 1000LL + (end->tv_usec - start->tv_usec) / 1000LL;  
}
int are_IPs_same(const char *a,const char *b){
    const char *i, *j;
    for( i = a, j = b; *i !='\0' && *j !='\0'; i++, j++)
        if(*i != *j) return 0;
    if(*i != '\0' || *j != '\0') return 0;
    return 1;
}

int main(int argc, char *argv[])
{
    if(argv[2]){
        printf("please give only one argument\n");
        return EXIT_FAILURE;
    }
    if(verify_input(argv[1], 0) != 0){
        printf("invalid argument\n");
        return EXIT_FAILURE;
    }
    int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock_fd < 0) {
		fprintf(stderr, "socket error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}

    for(int TTL = 1; TTL <= 30; TTL++){
        struct sockaddr_in recipient;
        bzero (&recipient, sizeof(recipient));
        recipient.sin_family = AF_INET;
        inet_pton(AF_INET, argv[1], &recipient.sin_addr);       
        setsockopt(sock_fd, IPPROTO_IP, IP_TTL, &TTL, sizeof(int));
        for(int seq = 1; seq <= 3; seq++){
            struct icmp header = make_header(&TTL, & seq);   
            ssize_t bytes_sent = sendto (sock_fd,
                                        &header,
                                        sizeof(header), 
                                        0, 
                                        (struct sockaddr*)&recipient, 
                                        sizeof(recipient));
            if (bytes_sent < 0) {
                fprintf(stderr, "bytes_sent error: %s\n", strerror(errno));
                return EXIT_FAILURE;
		    }
        }
        

        
        struct timeval start_time, end_time;
        int time = 1000;
        ssize_t packet_len[3];
        struct sockaddr_in sender[3];
		socklen_t sender_len[3] = {sizeof(sender[0]), sizeof(sender[1]), sizeof(sender[2])};
		u_int8_t buffer[3][IP_MAXPACKET];
        int packets = 0;
        for(packets = 0; packets < 3; packets++)
        {   
            struct pollfd ps = make_ps(&sock_fd);
            if(gettimeofday(&start_time, NULL) == -1) {
                printf("time measurement error\n");
                return EXIT_FAILURE;
            }
            int ready = poll (&ps, 1, time);
            if(gettimeofday(&end_time, NULL) == -1) {
                printf("time measurement error\n");
                return EXIT_FAILURE;
            }

            if(ready == 0) break;
            else if(ready < 0){
                fprintf(stderr, "poll error: %s\n", strerror(errno));
			    return EXIT_FAILURE;
            }else if(ps.revents & POLLIN){
                packet_len[packets] = recvfrom (sock_fd, buffer[packets], IP_MAXPACKET, MSG_DONTWAIT, 
                                                (struct sockaddr*)&sender[packets], &sender_len[packets]);
                if (packet_len[packets] < 0) { //theoretically never occures
                    fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
                    return EXIT_FAILURE;
                }
                struct ip* ip_header = (struct ip*) buffer[packets];
                ssize_t ip_header_len = 4 * (ssize_t)(ip_header ->ip_hl);
                struct icmp* icmp_response = (struct icmp*)((void *)buffer[packets]+ ip_header_len);

                if(icmp_response->icmp_type == ICMP_TIME_EXCEEDED){
                    ip_header = (struct ip*)((void *)ip_header + ip_header_len + 28);
                    icmp_response = (struct icmp*)((void *)icmp_response + 28);
                    // printf("tle\n");
                }
                if(icmp_response -> icmp_id != TTL)
                    packets--;
            }
            time -= time_delta(&start_time, &end_time);
            if(time <= 0) break;
        }

        printf("%d: ", TTL);
        if(packets == 0){
            printf("*\n");
            continue;
        }else{
            char sender_ip_str[3][20];
            int end_traceroute = 0;
            for(int i = 0; i < packets; i++){
                inet_ntop(AF_INET, &(sender[i].sin_addr), sender_ip_str[i], sizeof(sender_ip_str[i]));  
                int check = 0;
                for(int j = i-1; j>=0; j--){
                    if(are_IPs_same(sender_ip_str[i],sender_ip_str[j])){
                        check = 1;
                        break;
                    }  
                }
                if(!check)
                    printf("%s ", sender_ip_str[i]);
                end_traceroute = (are_IPs_same(sender_ip_str[i], argv[1]) ? 1 : end_traceroute);   
            }
            if(packets  < 3) printf("???\n");
            else printf("%lfms\n", (double)(1000-time)/(double)3);
            if(end_traceroute) return EXIT_SUCCESS;
        }
    }

}