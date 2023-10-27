import random
def uprosc_zdanie(tekst, dl_slowa, liczba_slow):
    tekst = tekst.split()
    tekst = [i for i in tekst if len(i) <= dl_slowa ]
    while(len(tekst) > liczba_slow):
        tekst.pop(tekst.index(random.choice(tekst)))
    return ' '.join(tekst)

with open('lorem.txt', 'r') as lorem:
    data = lorem.read().lower()
print(uprosc_zdanie(data, 10, 50))
