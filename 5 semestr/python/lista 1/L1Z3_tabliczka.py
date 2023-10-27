import numpy as np
def tabliczka(x1, x2, y1, y2, d):
    pocz = [""]
    l = x1
    while l <= x2:
        pocz.append(l)
        l += d
    A.append(pocz)
    kol = [""]
    l = y1
    while l <= y2:
        kol.append(l)
        l += d


    i = 0
    for i in kol:
        wstaw = []
        for j in pocz:
            if j != "":
                wstaw.append(j*i)

    A=[["", x1, x1+d, x1+2*d],
    [y1, x1*y1, (x1+d)*y1, (x1+2*d)*y1],
    [y1+d, x1*(y1+d), (x1+d)*(y1+d), (x1+2*d)*(y1+d)],
    [y1+2*d, x1*(y1+2*d), (x1+d)*(y1+2*d), (x1+2*d)*(y1+2*d)]]
    for row in A:
        for val in row:
            print('{:1}'.format(val), end=""),
        print()

tabliczka(3.0, 5.0, 2.0, 4.0, 1.0)

# import numpy as np
# def tabliczka(x1, x2, y1, y2, d):
#     A=[[""," ", x1," ", x1+d," ", x1+2*d],
#     [y1," ", x1*y1," ", (x1+d)*y1," ", (x1+2*d)*y1],
#     [y1+d," ", x1*(y1+d)," ", (x1+d)*(y1+d)," ", (x1+2*d)*(y1+d)],
#     [y1+2*d," ", x1*(y1+2*d)," ", (x1+d)*(y1+2*d)," ", (x1+2*d)*(y1+2*d)]]
#     for row in A:
#         for val in row:
#             print('{:2}'.format(val), end=""),
#         print()

# tabliczka(3.0, 5.0, 2.0, 4.0, 1.0)