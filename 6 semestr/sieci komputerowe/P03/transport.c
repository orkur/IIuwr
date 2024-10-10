//Jakub Galaszewski
//332042

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <stdbool.h>
#include <fcntl.h>

#define UDP_DATAGRAM_SIZE 1000
#define FRAME_SIZE 4500
#define TIME 1
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

struct datagram { //zamierzam wywalić ten size, ale na raie mam ten kod w trakcie debuggingu
    int32_t start;
    int16_t size;
    char *bytes;
}datagram;

struct pollfd make_ps(const int *sock_fd){
    struct pollfd ps;
    ps.fd = *sock_fd;
    ps.events = POLLIN;
    ps.revents = 0;
    return ps;
}

void ERROR(const char* str)
{
    fprintf(stderr, "%s: %s\n", str, strerror(errno));
    exit(EXIT_FAILURE);
}
int write_to_file(int* f, int *start, struct datagram* tab, int *no_bytes){
    int is = 0;
    int prev = -1;
    //przesuwam się ze startem cyklicznie póki mam element na prawo
    while(tab[*start].start > prev && tab[*start].bytes != NULL){
        prev = tab[*start].start;
        (*no_bytes)+=tab[*start].size;
        if(write(*f, tab[*start].bytes, tab[*start].size) < 0) ERROR("Write error");
        free(tab[*start].bytes);
        tab[*start].bytes = NULL;
        tab[*start].size=0;
        *start = (*start + 1)%FRAME_SIZE;
        is = 1;
    }
    return is;
}
int main(int argc, char** argv){
    if(argc != 5)
    {
        printf("please give exactly four arguments\n");
        return EXIT_FAILURE;
    }
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0)
        ERROR("socket error");
    char buffer[1018];
    char *ip_address = argv[1];
    int port = atoi(argv[2]);
    char *file_name = argv[3];
    int bytes = atoi(argv[4]);
    struct datagram arr_of_bytes[FRAME_SIZE];
    for(int i = 0; i < FRAME_SIZE; i++){
        arr_of_bytes[i].start = -1;
        arr_of_bytes[i].size = 0;
    }
    int *last_byte = (int *)malloc(sizeof(int));
    *last_byte = 0;
    int *received_bytes = (int *)malloc(sizeof(int));
    *received_bytes = 0;

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    if(0 >= inet_pton(AF_INET, ip_address, &server_address.sin_addr))
        ERROR("wrong address!");
    
    int file = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0666);
    int bytes_request_send; 
    char *start = malloc(12);
    char *length = malloc(11);
    char *request = malloc(20); 
    while (*received_bytes < bytes)
    {
        bytes_request_send = 0;
        //nawalam tyle requestów ile mam miejsca we frame, o ile nie mam tam już czegoś co mi się przyda
        for(int i = 0; i < FRAME_SIZE; i++) 
        {
            int s = ((*received_bytes)+ (UDP_DATAGRAM_SIZE*i));
            if(s >= bytes || arr_of_bytes[(s/UDP_DATAGRAM_SIZE)%FRAME_SIZE].size != 0) continue;
            
            sprintf(start, "%d", s);
            int size = MIN(UDP_DATAGRAM_SIZE,bytes - s);
            sprintf(length, "%d", size);
            bytes_request_send += size;
            request = strcat(strcat(strcat(strcat(strcpy(request, "GET "), start), " "), length), "\n\0");

            if((size_t)sendto(sock_fd, request, strlen(request), 0, (struct sockaddr*) &server_address, sizeof(server_address)) != strlen(request))
                ERROR("sendto error");
        }
        while(1){
            struct sockaddr_in sender;
            socklen_t sender_len = sizeof(sender);
            struct pollfd ps = make_ps(&sock_fd);
            char sender_ip_str[20];
            //nasłuchiwanko
            int ready = poll (&ps, 1, TIME);
            if(ready < 0)  ERROR("poll error");
            else if(ready == 0) break;
            else if(ready > 0){
                ssize_t res_len = recvfrom (sock_fd, buffer, UDP_DATAGRAM_SIZE+18, MSG_DONTWAIT, 
                                                        (struct sockaddr*)&sender, &sender_len);
                if (res_len < 0) { //theoretically never occures
                    fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
                    return EXIT_FAILURE;
                }
                //jak wszystko git to sprawdzam czy rzeczywiście to dla mnie info
                inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str)); 
                if(!strcmp(sender_ip_str, ip_address) && ntohs(sender.sin_port) == port){
                    int start = atoi(buffer+5);
                    int index = (start/UDP_DATAGRAM_SIZE)%FRAME_SIZE;
                    //jeżeli dostaję na to pole stary response to go po prostu nie uwzględniam
                    //nawet zdaje mi się że pierwsza część ifa jest zbędna
                    //rzeczywiście useless
                    if(arr_of_bytes[index].start >= start ) continue;
                    
                    arr_of_bytes[index].start = start;
                    arr_of_bytes[index].size = atoi(strchr(strchr(buffer, ' ') + 1, ' ') + 1);
                    arr_of_bytes[index].bytes = (char *)malloc(sizeof(char) *arr_of_bytes[index].size);
                    memcpy(arr_of_bytes[index].bytes, strchr(buffer + 11, '\n') + 1, arr_of_bytes[index].size);
                    
                    if(write_to_file(&file, last_byte, arr_of_bytes, received_bytes))
                        //będę musiał ukraść od ciebie tego printfa bo wygląda kozacko
                        printf("DONE : %f \n", (float)*received_bytes/(float)bytes * 100.0);
                }else
                    ERROR("wrong data");
            }
        }
    }
    close(sock_fd);
    close(file);
    return EXIT_SUCCESS;
}