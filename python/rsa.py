#!/bin/python3
import random
import time

def genCandidate(n):
    return random.randint(0,2**n-1)

def genPrime(iterationNumber, N):
    t0 = time.time()
    primeTest = genCandidate(N)
    i = 0
    while (i < iterationNumber):
        baseTest = genCandidate(N)
        test = pow(baseTest,(primeTest-1), primeTest)
        if (test == 1):
            i += 1
        else:
            primeTest = genCandidate(N)
            i = 0
    tf = time.time()
    return (tf - t0, primeTest)

def modularInv(num, mod):#repasar algorisme
    k = 0
    inv = (1 + mod*k)/num
    while (inv % 1 != 0.0 and int(inv) < mod):
        print(inv)
        k += 1
        inv = (1 + mod*k)/num
    if (int(inv) > mod):
        inv = -1
    return int(inv)

def genKeys(nBit):
    iterationNumber = 100
    q = genPrime(iterationNumber,nBit/2)[1]
    p = genPrime(iterationNumber,nBit/2)[1]
    n = p * q
    
    d = genCandidate(nBit)
    e = modularInv(d, (p-1)*(q-1))
    while (e < 0):
        d = genCandidate(nBit)
        e = modularInv(d, (p-1)*(q-1))
    return {"private":(q,p,d), "public": (e, n)}
if __name__ == '__main__':

    N = 330 #nuber of binary digits (bits). Example: RSA-100 -> N = 330 bits
    print (str(genKeys(N)))
