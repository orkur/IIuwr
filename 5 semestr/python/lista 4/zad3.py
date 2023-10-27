import math
import functools 
import timeit
def ile(a, b):
    i = 0
    while a%b == 0:
        i+=1
        a /= b
    return i
def rozklad_skladana(n):
    p = [i for i in range(2, n) if [j for j in range(2, (int) (math.sqrt(i)) + 1) if i%j == 0] == []]
    return [(i, (int)(ile(n,i))) for i in p if n%i == 0 ]

def rozklad_imperatywna(n):
    p = []
    i = 2
    while n > 1:
        wiel = ile(n, i)
        if wiel != 0:
            p.append((i, wiel))
            n = n / (i ** wiel)
        i += 1
    return p

def rozklad_funkcyjna(n):
    p = lambda x : all([(x%j) for j in range(2, (int) (math.sqrt(x)) + 1)]) and x >= 2
    lista = [i for i in range(2, n)]
    lista = list(filter(p, lista))
    lista = list(filter(lambda x : n % x == 0, lista ))
    return list(map(lambda x : (x, ile(n, x)), lista))





if __name__ == '__main__':
    n = 10
    print("  n  skladana imperatywna funkcyjna")
    while n < 100:
        start = timeit.default_timer()
        rozklad_imperatywna(n)
        imp = timeit.default_timer() - start
        start = timeit.default_timer()
        rozklad_skladana(n)
        skl = timeit.default_timer() - start
        start = timeit.default_timer()
        rozklad_funkcyjna(756)
        fun = timeit.default_timer() - start
        print("{}:     {:.3f}       {:.3f}     {:.3f}".format(n, imp, skl, fun) )
        n += 10        