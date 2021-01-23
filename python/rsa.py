#!/bin/python3
import random
import time
from modInv import *

iterationNumber = 10000

class RSAKeys(object):
    def __init__(self, nBit):
        self.nBit = nBit
        self.public = 0
        self.__private = 0
        self.__paraphrase = ""

    def __genCandidate(self):
        return random.randint(1,2**(self.nBit-1))

    def __genPrime(self):
        t0 = time.time()
        primeTest = self.__genCandidate()
        baseTest_v= self.__genCandidate()
        baseTest = []
        i = 0
        #primeTest = 7
        #print("asasd")
        while (i < iterationNumber):
            while (baseTest_v % primeTest == 0 or baseTest_v in baseTest):
                baseTest_v= self.__genCandidate()
            baseTest.append(baseTest_v)
            #print(baseTest[-1])
            test = pow(baseTest[-1],(primeTest-1), primeTest)
            #            print(test)
            if (test == 1):
                #print(primeTest)
                #print(i)
                i += 1
            else:
                #print(test)
                #print(baseTest)
                #break
                primeTest = self.__genCandidate()
                baseTest = []
                baseTest_v= self.__genCandidate()
                i = 0
        tf = time.time()
        return (tf - t0, primeTest)
    
    def getPrivateKey(self, paraphrase):
        result = -1
        if (paraphrase == self.__paraphrase):
            result = self.__private
        return result
    
    def genKeys(self, paraphrase):
        q = self.__genPrime()[1]
        p = self.__genPrime()[1]
        n = p * q

        d = self.__genCandidate()
        e = modInv(d, (p-1)*(q-1))
        
        while (e == 0): #or (d * e %((p-1)*(q-1))) != 1):
            d = self.__genCandidate()
            e = modInv(d, (p-1)*(q-1))
        self.__paraphrase = paraphrase
        self.public = (n, e)
        self.__private = (n, d)

    def save_keys_to_file(self, filename, paraphrase):
        result = -1
        if (paraphrase == self.__paraphrase):
            f = open(filename, "w")
            w = self.__paraphrase + "\n" + str(self.__private)[1:-1] + "\n" + str(self.public)[1:-1] + "\n"
            f.write(w)
            result = 0
            f.close()
        return result
    def load_keys_from_file(self, filename, paraphrase):
        f = open(filename, "r")
        result = []
        for line in f:
            result.append(line[:-1])
        if (result[0] == paraphrase):
            self.__paraphrase = paraphrase
            self.__private = tuple(map(int, result[1].split(', ')))
            self.public = tuple(map(int, result[2].split(', ')))
            result = 0
        f.close()
        return result


def rsa_encrypt(key, string_msg):
    int_msg_list = []
    n, e = key
    #private = keys.getPrivateKey(paraphrase)
    if (key != 0):
        for char in string_msg:
            int_msg_list.append(pow(ord(char), e, n)) # pow(char, e, n) --> (char ** e) % n
    return int_msg_list

def rsa_decrypt(key, secret_msg):
    result = ""
    n, d = key
    if (key != 0):
        for num in secret_msg:
            result += chr(pow(num, d, n))  #(num ** d) % n
    return result
    
if __name__ == '__main__':

    #N = 330 #nuber of binary digits (bits). Example: RSA-100 -> N = 330 bits
    #print (str(genKeys(N)))
    keys = RSAKeys(1024)
    if (False):
        keys.genKeys("enrico")
        keys.save_keys_to_file("key.dat", "enrico")
    else:
        keys.load_keys_from_file("key.dat", "enrico")
        #print(keys.public)
    
    msg = "HOLA QUE TAL"
    secret = rsa_encrypt(keys.public, msg)
    print(secret)
    msg = rsa_decrypt(keys.getPrivateKey("enrico"), secret)
    print(msg)
