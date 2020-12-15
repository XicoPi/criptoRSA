#!/bin/python3
import random




def genPrime(iterationNumber, N):
    limit =(10 ** (N)) -1 
    primeTest = random.randint(0,limit)
    i = 0
    while (i < iterationNumber):
        baseTest = random.randint(0, limit)
        test = pow(baseTest,(primeTest-1), primeTest)
        if (test == 1):
            i += 1
        else:
            primeTest = random.randint(0,limit)
            i = 0
    return primeTest
    
if __name__ == '__main__':

    N = 331 #nuber of digital digits. Example: RSA-100 -> N = 100
    print (str(genPrime(100,N)))
