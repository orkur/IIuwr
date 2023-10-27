from decimal import Decimal
def vat_faktura(lista):
    wynik = sum(lista)
    return wynik * 1.23
def vat_paragon(lista):
    lista = list(map(lambda x : x*1.23, lista))
    wynik = sum(lista)
    return wynik

def vat_faktura2(lista):
    wynik = sum(lista)
    return wynik * Decimal(1.23)
def vat_paragon2(lista):
    lista = list(map(lambda x : x*Decimal(1.23), lista))
    wynik = sum(lista)
    return wynik
zakupy = [1.21111, 1.1222222, 100.2333333, 21.37] #przyklad korzystajacy z IEEE 754, tj precyzji liczb zmiennoprzecinkowych 
print(vat_faktura(zakupy) == vat_paragon(zakupy)) 
zakupy2 = [Decimal(1.21111), Decimal(1.1222222), Decimal(100.2333333), Decimal(21.37)]
# print(zakupy2)
print(vat_faktura2(zakupy2) == vat_paragon2(zakupy2)) 
