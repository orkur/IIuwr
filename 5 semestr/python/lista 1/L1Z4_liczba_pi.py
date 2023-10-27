import random
import math
def pi(n):
    width = 200
    height = 200
    ltwo = 0
    cltwt = 0
    while(n>0):
        cltwt+=1
        x = random.uniform(-width, width)
        y = random.uniform(-height, height)
        if(math.sqrt(x*x+y*y) <= 200):
            ltwo+=1
        n-=1
    print(4*ltwo/cltwt)
n = int(input("podaj liczbe: "))
pi(n)
        