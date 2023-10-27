import math
def max_sublist_sum(lista):
    best_sum = - math.inf
    current_sum = 0
    x, y, odpx, odpy = 0, 0, 0, 0
    for i in lista:
        y += 1
        if i > current_sum + i:
            x = y
            current_sum = i
        else:
            current_sum = current_sum + i
        if current_sum > best_sum:
            best_sum = current_sum
            odpx, odpy = x, y
    return (odpx, odpy)

if __name__ == '__main__':
    print(max_sublist_sum([1, 2, 3, 4]))
    print(max_sublist_sum([-1, 2, 3, -4]))