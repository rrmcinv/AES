# AES
AES encryption algorithm implementation

# Usage:

build: make
run: ./aes --keysize <128|256> --keyfile <key file name> --inputfile <input file name> --outputfile <output file name> --mode <encrypt|decrypt>
	example: ./aes --keysize 128 --keyfile keyfile --inputfile inputfile --outputfile outputfile --mode encrypt
	note: all arguments are required

# Implementation:

For simplicity and speed, sbox and multiplication values have been hardcoded.

In all cases, the program starts by parsing command line arguments and attempting to resolve filenames and encryption size and type. If it succeeds, it reads in data and calls the appropriate function, either Encrypt() or Decrypt().

# Encryption:

Encrypt() begins by padding the input by the CMS method with a call to Pad(). This function appends numbers to the end of the data until the data size is divisible by 16 bytes, with each padded number being equal to the total number of padded bytes.This function ALWAYS pads, even if the data size was already divisible by 16.

We then calculate Nk and Nr according to standard:
Nk = # of words in the key
Nr= # of rounds of encryption to run, based off of Nk

Next, we expand our key with a call to ExpandKey. This creates an array of words based off of our original key. The first Nk words are exactly equivalent to the original key, but the following words modified. Every Nkth word is modified by taking the previous word, rotating using RotWord, substituting using SubWord, and then xoring with the RCon array using XorWords. Additional calculations are made if the key is of size 256. If the key is of size 256, we sub once more using SubWord. Finally, regardless of which word we are operating on, we Xor the i-nkth word with the (sometimes modifed) previous word to get our final word.

Now that we have our key, we enter into a loop, processing 16 bytes of our input at a time and running EncryptCipher on them. When we're done, we return to main, where our output is written.

EncryptCipher: This main processing function starts by reading input data into a state. We then call AddRoundKey an initial time before entering a loop for the remaining rounds (minus the last round). Each round calls SubBytes, ShiftRows, MixColumns, and AddRoundKey on our input data. The final round does not mix columns, but performs all other functions. Finally, we output our data from the state to our output buffer at the appropriate location.

XorWords: Takes two Words (4-byte structures) and simply returns a new Word with the bytes of each parameter Word xored together.

SubWord: Using the S-Box, we substitute each byte in the word with the S-box table, using high-nibble as row and low-nibble as column to lookup the correct value.

RotWord: Rotates the bytes of a word very simply, moving each byte one space to the "left" (or "up") with wraparound.

SubBytes: Using the S-Box, we substitute each byte in state with the S-box table, using high-nibble as row and low-nibble as column to lookup the correct value.

ShiftRows: Shifts each row of a state with wraparound, with shift values corresponding directly to the row number; e.g. each byte in row 2 is shifted 2 places to the "right".

MixColumns: Effectively performs matrix multiplication with a provided matrix (formed from the columns of the state), but with addition being the Add function (xors) and multiplication being the Multiply function (uses a lookup table because the described methodology is slow and confusing). 

AddRoundKey: Differs slightly from the AES description of AddRoundKey; does not take a start or stop index for the expanded key words. Instead, uses the round number and establishes a start point as indicated in the standard (start = round*Nb). Simply goes down the array of key words and "adds" (Xors) their bytes to the state matrix column by column.

# Decryption:
