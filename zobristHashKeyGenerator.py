import random

name=["black", "white", "empty"]
MAX = pow(2,62)
f = open("HashKeys.txt","w")
s="long int HashKey[3][BOARDSIZE][BOARDSIZE]={"
for i in xrange(0,3):
    s+="{"
    for j in xrange(0,9):
        s+="{"
        for j in xrange(0,9):
            s += str(random.randint(2,MAX))+", "
        s+="},\n"
    s+="},\n"
s+="};"
f.write(s)
