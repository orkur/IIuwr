from collections import deque
def palindrome(text):
    table = [[0 for i in range(len(text))] for j in range(len(text))]
    d = deque()
    max_length = -1
    for i in range(0, len(text)):
        table[i][i] = 1
    for i in range(0, len(text)-1):
        if text[i] == text[i+1]:
            table[i][i+1] = 1
            max_length = 2
            d.append((i, 2))
    for k in range(3, len(text)+1):
        for i in range(0, len(text) - k + 1):
            j = i + k - 1
            if table[i+1][j-1] == 1 and text[i] == text[j]:
                table[i][j] = 1
                if k > max_length:
                    d.clear()
                    d.append((i, k))
                    max_length = k
                elif k == max_length:
                    d.append((i, k))
    if max_length == -1:
        return ()
    return d

if __name__ == '__main__':
    print(palindrome('abba'))
    print(palindrome('rababbar'))
    print(palindrome('ab'))
    print(palindrome('aabbccdd'))
    print(palindrome('aaabbbccddd'))