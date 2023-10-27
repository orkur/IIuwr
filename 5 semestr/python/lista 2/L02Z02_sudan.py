def memoise(k):
    sudan_memo = {}
    def partner(*args):
        if args not in sudan_memo:
            # print(*args)
            sudan_memo[args] = k(*args)
        # print(sudan_memo[args])
        return sudan_memo[args]
    return partner
@memoise
def sudan(n, x, y):
    if n == 0:
        return x + y
    if y == 0:
        return x
    return sudan(n-1, sudan(n, x, y-1), sudan(n, x, y-1) + y)

print(sudan(2, 0, 4))

# już dla n = 1 i stosunkowo niewielkiego y (koło 25) wersja bez memoizacji nie dawała rady
# wraz z memoizacją dla n = 1 można rozważyć większe liczby (dla y = 498 wylicza się ostatni wynik)
# natomiast dla n = 2 i x = 0, maksymalnie wylicza y = 3
