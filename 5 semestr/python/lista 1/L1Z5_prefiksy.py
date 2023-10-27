import os 
def common_prefix(arr):
    arr = list(map(lambda x: x.lower(), arr))
    arr.sort()
    prefix = ''
    for i in range(0,len(arr)-2):
        prefix = os.path.commonprefix(arr[i:i+3]) if len(prefix) < len(os.path.commonprefix(arr[i:i+3])) else prefix
    print(prefix)    
common_prefix(["Cyprian", "cyberotoman", "cynik", "ceniąc", "czule"])
#korzystam z dwoch obserwacji:
#1. Jezeli mamy najlepszy prefix 3 slow, to kolejne dodanie kolejnego slowa da wynik nielepszy od tych 3
#2. sortowanie umiejscawia "najbardziej podobne" liczby obok siebie, poniewaz sortujemy leksykograficznie
#dzieki tym spostrzezeniom udalo sie zbic ilosc sprawdzen wynikow prefixa z (n choose 3) + (n choose 4) + ... na okolo n 