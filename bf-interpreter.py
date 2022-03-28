## Written by Jordan Perry
## https://jappl.es/
## https://github.com/sponege

import re, requests

l = input("Local file (y/n) ").lower() == 'y'

if l: ## local file
    path = input("Path to file: ")
    file = open(path, 'r')
    bf = file.read()
    file.close()
else:
    link = input("Link to bf code: ")
    r = requests.get(link)
    bf = r.text

md = input("Memory dump? (y/n) ").lower() == 'y'

code = ''
accIn = False

for c in bf:
    if re.match('[><+-.,\[\]]', c):
        if c == ',':
            accIn = True
        code += c

if accIn: ## if input is used
    f = input("File input? (y/n) ").lower() == 'y'
    if f:
        fn = input("File name: ")
        file = open(fn, 'r')
        inp = file.read()
        file.close()
    else:
        inp = input("BF input: ")
    inputP = 0 ## input pointer

fo = input("File output? (y/n) ").lower() == 'y'

if fo:
    fW = open('bf.out', 'w')

mem = bytearray([0x0] * 30000) ## 30 thousand cells, cell size is 8 bits

ip = 0 ## ip is short for "instruction pointer"
dp = 0 ## dp is short for "data pointer"

i = 0 ## instruction executed

while ip  < len(code):
    if code[ip] == '>': ## increment data pointer
        dp += 1
        if dp >= 30000:
            dp = 0
    elif code[ip] == '<': ## decrement data pointer
        dp -= 1
        if dp < 0:
            dp = 30000 - 1
    elif code[ip] == '+': ## increment byte at data pointer
        if mem[dp] == 0xff:
            mem[dp] = 0 ## simulate overflow
        else:
            mem[dp] += 1
    elif code[ip] == '-': ## decrement byte at data pointer
        if mem[dp] == 0:
            mem[dp] = 0xff ## simulate underflow
        else:
            mem[dp] -= 1
    elif code[ip] == '.': ## print byte at data pointer
        if fo:
            fW.write(chr(mem[dp]))
        else:
            print(chr(mem[dp]), end = '')
    elif code[ip] == ',': ## accept one byte as input, store input in pointer
        if inputP >= len(inp):
            mem[dp] = 0
        else:
            mem[dp] = ord(inp[inputP])
            inputP += 1
    elif code[ip] == '[': ## if byte at dp is 0, jump to corresponding closing bracket
        if mem[dp] == 0:
            bS = 0 ## brackets seen, i know bs just shut up
            while bS >= 0:
                ip += 1 ## increment instruction pointer
                if code[ip] == '[':
                    bS += 1
                if code[ip] == ']':
                    bS -= 1
    elif code[ip] == ']': ## if byte at dp is not 0, jump to corresponding opening bracket
        if mem[dp] != 0:
            bS = 0 ## brackets seen, i know bs just shut up
            while bS >= 0:
                ip -= 1 ## decrement instruction pointer
                if code[ip] == ']':
                    bS += 1
                if code[ip] == '[':
                    bS -= 1
    i += 1
    ip += 1 ## increment data pointer

print()
print(i, "instructions executed")

if md:
    print("Memory dump:")
    print('Location' + '\t\t' + 'Hex' + '\t' + 'Char' + '\t' + 'Int')
    for i in range(len(mem)):
        if mem[i] != 0: ## don't print NULLs!
            print(hex(i) + '\t\t  ' + hex(mem[i]) + '\t' + chr(mem[i]) + '\t' + str(int(mem[i])))
