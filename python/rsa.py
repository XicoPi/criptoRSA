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
    
if __name__ == '__main__':

    N = 2048 #nuber of binary digits (bits). Example: RSA-100 -> N = 330 bits
    print (str(genPrime(100,N)))
