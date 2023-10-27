def statystyka(tekst):
    alfabet = {}
    for literka in tekst:
        if literka not in alfabet:
            alfabet[literka] = 0
        else:
            alfabet[literka] += 1
    dlugosc = len(tekst)
    for slowo in alfabet:
        alfabet[slowo] = alfabet[slowo]/dlugosc
    alfabet = dict(sorted(alfabet.items()))
    return alfabet

    return len(tekst), dict(sorted(alfabet.items()))
def prawdopodobienstwo(jezyk, wzorzec):
    wynik = 0.0
    mianownik = 0.0
    for literka in jezyk:
        if literka in wzorzec and jezyk[literka] != 0.0:
            if wzorzec[literka] < jezyk[literka]:
                wynik += wzorzec[literka]/jezyk[literka]
            else:
                wynik += jezyk[literka]/wzorzec[literka]
    
    return wynik / len(jezyk)


def porownaj(polski, angielski, lacina, bar):
    pol = prawdopodobienstwo(polski, bar)
    ang = prawdopodobienstwo(angielski, bar)
    lac = prawdopodobienstwo(lacina, bar)
    print(pol, ang, lac)
    if pol > ang and pol > lac:
        return "polski"
    elif ang > lac :
        return "angielski"
    else:
        return "łacina"

with open('pan-tadeusz.txt', 'r') as tadeusz:
    data = tadeusz.read().lower()
polski = statystyka(data)
with open('romeo.txt', 'r') as romeo:
    data2 = romeo.read().lower()
angielski = statystyka(data2)

with open('lorem.txt', 'r') as lorem:
    data3 = lorem.read().lower()
lacina = statystyka(data3)

with open('tekst.txt', 'r') as foo:
    data4 = foo.read().lower()
print(porownaj(polski, angielski, lacina, statystyka(data4))) #powinien być angielski

with open('tekst2.txt', 'r') as bar:
    data5 = bar.read().lower()
print(porownaj(polski, angielski, lacina, statystyka(data5))) # Powinna być łacina
