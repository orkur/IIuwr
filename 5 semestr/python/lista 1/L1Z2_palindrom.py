def is_palindrom(s):
    s=s.replace(" ", "")
    s=s.replace(",", "")
    s=s.replace(".", "")
    s=s.replace("!", "")
    s=s.replace("?", "")
    s=s.replace(":", "")
    s=s.replace(";", "")
    s=s.replace("'", "")
    s=s.replace("\"", "")
    s= s.lower()
    s2= s[::-1]
    print(s==s2)

is_palindrom("Eine g\"üldne, gute Tugend: Lüge nie!")
is_palindrom("Míč omočím.")
is_palindrom("Kobyła ma mały bok.")
    