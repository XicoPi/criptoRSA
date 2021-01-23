import math

def modInv(a, module):
    result = 0
    if (math.gcd(a,module) == 1):
        residue = module
        y = 0
        x = 1
        
        while (a > 1):
            quotient = a // residue
            temp = residue

            residue = a % residue
            a = temp
            temp = y
            y = x - quotient * y
            x = temp
        result = x % module
    return result
