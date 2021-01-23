
def dec2bin(num):
    if (num == 0):
        return ""
    else:
        return dec2bin(int(num/2)) + str(int(num%2))

def modExp(base, exp, mod):
    binExps = []
    binExp = dec2bin(int(exp))
    lim = len(binExp)
    for i in range(len(binExp)):
        binExps.append(2**(i))

    modExps = []
    modExps.append((base ** binExps[0]) % mod)
    for i in range(len(binExp)-1):
        modExps.append((modExps[i] * modExps[i]) % mod)

    result = 1
    for i,s in enumerate(reversed(binExp)):
        if (s == '1'):
            result = (result * modExps[i]) % mod
    return result

if __name__ == '__main__':
    print(dec2bin(117))
    print(str(modExp(5, 117, 19)))
