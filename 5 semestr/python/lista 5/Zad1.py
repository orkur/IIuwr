class VariableNotFoundException(Exception):
    "zmienna nie istnieje!"
    pass

class WrongType(Exception):
    "zly typ!"
    pass

class Wyrazenie:
    def __init__(self, x, y):
        self.lstr = x
        self.pstr = y
    def __add__(w1, w2):
        return Dodaj(w1,w2).uprosc()
    def __mul__(w1, w2):
        return Razy(w1,w2).uprosc()
    def __floordiv__(w1, w2): #__div__ wspierane dla python 2.x
        return Podziel(w1,w2).uprosc()
    def __sub__(w1, w2):
        return Odejmij(w1,w2).uprosc()
    def uprosc(self):
        if type(self.lstr) is Stala and type(self.pstr) is Stala: #jezeli stala to mozemy wykonac obliczenia 
            return Stala(self.oblicz([]))
        if type(self) is Razy: #mnozenie przez 1, 0
            if type(self.lstr) is Stala:
                if self.lstr.oblicz([]) == 1:
                    return self.pstr
                if self.lstr.oblicz([]) == 0:
                    return Stala(0)
            if type(self.pstr) is Stala:
                if self.pstr.oblicz([]) == 1:
                    return self.lstr 
                if self.pstr.oblicz([]) == 0:
                    return Stala(0)
        if type(self) is Podziel: # dzielenie przez 1
            if type(self.pstr) is Stala:
                if self.pstr.oblicz([]) == 1:
                    return self.lstr
        if type(self) is Dodaj: # 2 + x + 3 -> 5 + x
            if type(self.pstr) is Stala:
                if type(self.lstr) is Dodaj: 
                    if type(self.lstr.lstr) is Stala:
                        self.lstr.lstr.stala += self.pstr.stala
                        del self.pstr
                        s = self.lstr
                        del self
                        return s
                    if type(self.lstr.pstr) is Stala:
                        self.lstr.pstr.stala += self.pstr.stala
                        del self.pstr
                        s = self.lstr
                        del self
                        return s
            if type(self.lstr) is Stala:
                if type(self.pstr) is Dodaj: 
                    if type(self.pstr.lstr) is Stala:
                        self.pstr.lstr.stala += self.lstr.stala
                        del self.lstr
                        s = self.pstr
                        del self
                        return s
                    if type(self.pstr.pstr) is Stala:
                        self.pstr.pstr.stala += self.lstr.stala
                        del self.lstr
                        s = self.pstr
                        del self
                        return s
        if type(self) is Razy:  #2 * x * 5 -> 10 * x
            if type(self.pstr) is Stala:
                if type(self.lstr) is Razy: 
                    if type(self.lstr.lstr) is Stala:
                        self.lstr.lstr.stala *= self.pstr.stala
                        del self.pstr
                        s = self.lstr
                        del self
                        return s
                    if type(self.lstr.pstr) is Stala:
                        self.lstr.pstr.stala *= self.pstr.stala
                        del self.pstr
                        s = self.lstr
                        del self
                        return s
            if type(self.lstr) is Stala:
                if type(self.pstr) is Razy: 
                    if type(self.pstr.lstr) is Stala:
                        self.pstr.lstr.stala *= self.lstr.stala
                        del self.lstr
                        s = self.pstr
                        del self
                        return s
                    if type(self.pstr.pstr) is Stala:
                        self.pstr.pstr.stala *= self.lstr.stala
                        del self.lstr
                        s = self.pstr
                        del self
                        return s
        if type(self) is Razy and type(self.pstr) is Stala: #zwyczajowo piszemy 3*x, nie x*3
            self.lstr, self.pstr = self.pstr, self.lstr
        return self
        
class Dodaj(Wyrazenie):
    def __init__(self, x, y):
        super().__init__(x, y)
    def oblicz(self, zmienne):
        return self.lstr.oblicz(zmienne) + self.pstr.oblicz(zmienne)
    def __str__(self):
        return "(" + self.lstr.__str__() + "+" + self.pstr.__str__() + ")"
        
class Odejmij(Wyrazenie):
    def __init__(self, x, y):
        super().__init__(x, y)
    def oblicz(self, zmienne):
        return self.lstr.oblicz(zmienne) - self.pstr.oblicz(zmienne)
    def __str__(self):
        return "(" + self.lstr.__str__() + "-" + self.pstr.__str__() + ")"


class Razy(Wyrazenie):
    def __init__(self, x, y):
        super().__init__(x, y)
    def oblicz(self, zmienne):
        return self.lstr.oblicz(zmienne) * self.pstr.oblicz(zmienne)
    def __str__(self):
        return self.lstr.__str__() + "*" + self.pstr.__str__()

class Podziel(Wyrazenie):
    def __init__(self, x, y):
        super().__init__(x, y)
    def oblicz(self, zmienne):
        return self.lstr.oblicz(zmienne) // self.pstr.oblicz(zmienne)
    def __str__(self):
        return self.lstr.__str__() + "//" + self.pstr.__str__()


class Stala(Wyrazenie):
    def __init__(self, x):
        if not type(x) is int:
            raise WrongType("zly typ")
        self.stala = x
    def oblicz(self, zmienne):
        return self.stala
    def __str__(self):
        return str(self.stala)
class Zmienna(Wyrazenie):
    def __init__(self, x):
        self.x = x
    def oblicz(self, zmienne):
        if zmienne.get(self.x) == None:
            raise VariableNotFoundException("brak zmiennej")
        if not type(zmienne.get(self.x)) is int:
            raise WrongType("zly typ")
        return zmienne.get(self.x)
    def __str__(self):
        return self.x 
foo = Razy(Dodaj(Zmienna("x"), Stala(2)), Zmienna("y"))
tab = {
    "x": 4,
    "y": 5,
    "n": 0
}
print(((foo - Zmienna("y") * Stala(10) *Stala(10)//Stala(2)).oblicz(tab)))
print(((foo - Zmienna("y") * Stala(10) *Stala(10))//Stala(2)))
print(Stala(2) + Stala(2) // Zmienna("x") * Stala(1)//Stala(1))
print(Stala(2) + Zmienna("x") + Stala(2))
print(Zmienna("x") * Stala(8) * Stala(4) * Stala(10) * Stala(10))