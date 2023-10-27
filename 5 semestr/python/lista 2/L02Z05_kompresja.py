def kompresja(tekst):
    skompresowany = list()
    i = 1
    literka = tekst[0]
    liczba = 1
    while i < len(tekst):
        if tekst[i] != literka:
            skompresowany.append((literka, liczba))
            liczba = 1
            literka = tekst[i]
        else:
            liczba += 1
        i += 1
    skompresowany.append((literka, liczba))
    return skompresowany
def dekompresja(kompresowany):
    tekst = ''
    for a, b in kompresowany:
        i = 0
        while i < b:
            tekst += a 
            i += 1
    return tekst
with open('lorem.txt', 'r') as lorem:
    data = lorem.read().lower()
print(kompresja(data))
print(dekompresja(kompresja(data)))


