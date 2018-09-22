# AES
AES encryption algorithm implementation

# Usage:

build: make
run: ./aes --keysize <128|256> --keyfile <key file name> --inputfile <input file name> --outputfile <output file name> --mode <encrypt|decrypt>
	example: ./aes --keysize 128 --keyfile keyfile --inputfile inputfile --outputfile outputfile --mode encrypt
	note: all arguments are required

# Implementation:

For simplicity and speed, sbox and multiplication values have been hardcoded.
