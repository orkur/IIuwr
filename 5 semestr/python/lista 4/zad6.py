import itertools
import copy

def check_subsequence(sub, arr):
    l = len(sub)
    for i in range(len(arr)-l + 1):
        if arr[i:i+l] == sub:
            return True
    return False

def sprawdz(tab, H, V):
    col = 0
    for i in H:
        col_arr = list(tab[i][col] for i in range(len(V)))
        subs = [1] * i
        if sum(col_arr) != i or check_subsequence(subs, col_arr) == False:
            return None
        col += 1
    row = 0
    for i in V:
        row_arr = tab[row]
        subs = [1] * i
        if sum(row_arr) != i or check_subsequence(subs, row_arr) == False:
            return None
        row += 1
    
    return tab


def rekursja(tab, H, V, i, rozw):
    if i < len(H) * len(V):
        tab[i//len(H)][i%len(H)] = 0
        a = rekursja(tab, H, V, i+1, rozw)
        tab[i//len(H)][i%len(H)] = 1
        a = rekursja(tab, H, V, i+1, rozw)
    else:
        a = sprawdz(tab, H, V)
        if a != None:
            rozw.append(copy.deepcopy(a))
    return rozw
        
def shadow(H, V):
    tab = [[0 for _ in range(len(H))] for _ in range(len(V))]
    return rekursja(tab, H, V, 0, [])

def wypisz(H, V, tab):
    for i in H:
        print(i, end="")
    print()
    for i in range(len(tab)):
        for j in range(len(tab[i])):
            print("X" if tab[i][j] == 1 else " ", end="")
        print(V[i])
        
if __name__ == '__main__':
    H = (1, 1, 1)
    V = (1, 1, 1)
    # program dziala, ale mam problem z wypisywaniem rekurencyjnym
    # EDIT, okazalo sie ze nie wykorzystalem deepcopying przez co sie psulo,
    #  dodatkowo nie wiedzialem ze "rekursja()" przyjmuje pole "rozw" przez referencje, a nie kopie
    # prosilbym na nastepnych zajeciach wytlumaczyc kiedy co jest kopiowane, a kiedy brane przez referencje
    for i in shadow(H, V):
        wypisz(H,V,i)
    