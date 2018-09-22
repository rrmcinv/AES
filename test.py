#!/usr/bin/env python3

from Crypto.Cipher import AES
import binascii
import argparse

parser = argparse.ArgumentParser(description='Test AES implem')
parser.add_argument('integers', metavar='aes_size', type=int,
                    help='Which AES mode (128/256)')

args = parser.parse_args()
if not(args.integers == 128 or args.integers == 256):
    print("Incorrect AES size")
    exit()
elif args.integers == 128:
    keysize = 16
else:
    keysize = 32

# Generate Key
key = bytes([ 0 for i in range(keysize)] )
f = open("keyfile", "wb")
f.write(key)

# Generate Input with CMS padding
inputfile = bytes([ 0 for i in range(16)] )
f = open("inputfile", "wb")
f.write(inputfile)

length = 16 - (len(inputfile) % 16)
inputfile += bytes([length])*length

cipher = AES.new(key, AES.MODE_ECB)
output = cipher.encrypt(inputfile)

f = open("output", "wb")
f.write(output)