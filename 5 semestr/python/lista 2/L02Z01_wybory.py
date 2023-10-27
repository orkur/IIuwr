import math
def Dhondt(n, komitety):
    prog = math.ceil(sum(komitety) / 20)
    dzielnik = []
    for i in komitety:
        if i >= prog:
            dzielnik.append(1)
        else:
            dzielnik.append(-1)
    wynik = [x // y for x, y in zip(komitety, dzielnik)]
    print(wynik)
    while(n > 0):
        n-=1
        indeks = wynik.index(max(wynik))
        dzielnik[indeks] += 1
        wynik[indeks] = komitety[indeks] // dzielnik[indeks]
    mandaty = []
    for i in dzielnik:
        if i >= 1:
            mandaty.append(i-1)
        else:
            mandaty.append(0)
    for i in range(0, len(komitety)):
        print("partia: ", komitety[i], " mandaty: ", mandaty[i])

Dhondt(460, [8051935, 5060355, 2319946, 1578523, 1256953, 144773, 18918, 5448, 1765])
Dhondt(460, [5711687, 3661474, 1339094, 1155370, 779875])
Dhondt(8, [720, 300, 480])
