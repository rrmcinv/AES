# AES
AES encryption algorithm implementation
Robert McInvale
rrm2754

# Usage:

build: make

run: ./aes --keysize <128|256> --keyfile <key file name> --inputfile <input file name> --outputfile <output file name> --mode <encrypt|decrypt>

example: ./aes --keysize 128 --keyfile keyfile --inputfile inputfile --outputfile outputfile --mode encrypt

note: all arguments are required

# Implementation:

For simplicity and speed, sbox and multiplication values have been hardcoded.

In all cases, the program starts by parsing command line arguments and attempting to resolve filenames and encryption size and type. If it succeeds, it reads in data and calls the appropriate function, either Encrypt() or Decrypt().

## Encryption:

Encrypt() begins by padding the input by the CMS method with a call to Pad(). This function appends numbers to the end of the data until the data size is divisible by 16 bytes, with each padded number being equal to the total number of padded bytes.This function ALWAYS pads, even if the data size was already divisible by 16.

We then calculate Nk and Nr according to standard:
Nk = # of words in the key
Nr= # of rounds of encryption to run, based off of Nk

Next, we expand our key with a call to ExpandKey. This creates an array of words based off of our original key. The first Nk words are exactly equivalent to the original key, but the following words modified. Every Nkth word is modified by taking the previous word, rotating using RotWord, substituting using SubWord, and then xoring with the RCon array using XorWords. Additional calculations are made if the key is of size 256. If the key is of size 256, we sub once more using SubWord. Finally, regardless of which word we are operating on, we Xor the i-nkth word with the (sometimes modifed) previous word to get our final word.

Now that we have our key, we enter into a loop, processing 16 bytes of our input at a time and running EncryptCipher on them. When we're done, we return to main, where our output is written.

### EncryptCipher
This main processing function starts by reading input data into a state. We then call AddRoundKey an initial time before entering a loop for the remaining rounds (minus the last round). Each round calls SubBytes, ShiftRows, MixColumns, and AddRoundKey on our input data. The final round does not mix columns, but performs all other functions. Finally, we output our data from the state to our output buffer at the appropriate location.

### XorWords
Takes two Words (4-byte structures) and simply returns a new Word with the bytes of each parameter Word xored together.

### SubWord
Using the S-Box, we substitute each byte in the word with the S-box table, using high-nibble as row and low-nibble as column to lookup the correct value.

### RotWord
Rotates the bytes of a word very simply, moving each byte one space to the "left" (or "up") with wraparound.

### SubBytes
Using the S-Box, we substitute each byte in state with the S-box table (kSBox), using high-nibble as row and low-nibble as column to lookup the correct value.

### ShiftRows
Shifts each row of a state with wraparound, with shift values corresponding directly to the row number; e.g. each byte in row 2 is shifted 2 places to the "left".

### MixColumns
Effectively performs matrix multiplication with a provided matrix (formed from the columns of the state), but with addition being the Add function (xors) and multiplication being the Multiply function (uses a lookup table because the described methodology is slow and confusing). Just uses kMul2 and kMul3 multiplication tables.

### AddRoundKey
Differs slightly from the AES description of AddRoundKey; does not take a start or stop index for the expanded key words. Instead, uses the round number and establishes a start point as indicated in the standard (start = round*Nb). Simply goes down the array of key words and "adds" (Xors) their bytes to the state matrix column by column.

## Decryption:

Decrypt() is extremely similar to Encrypt(), only inversed. We start by calculating Nk and Nr then expanding our key again, and then we call our main cipher, DecryptCipher. Finally, we remove our padding with DePad and return to main.

### DecryptCipher
Basically EncryptCipher in reverse. Reads our input into a state, then calls AddRoundKey (exact same function as in EncryptCipher, as it is its own inverse) for the last word in our expanded key. Then runs through a reverse round process, starting at the end of the expanded key and moving back to round 0. For each round, we call InvShiftRow, InvSubBytes, AddRoundKey, and then InvMixColumns (I'm actually not 100% sure why this order, but the standard states that we should use it and it works). For the last round (which reverses the first round of our encryption), we just do InvShiftRows and InvSubBytes, and then a final AddroundKey.

### AddRoundKey
as in Encryption

### InvShiftRows
Same as ShiftRows, but we shift "right" instead of "left". Negative indices do have to be accounted for, so there's an extra guard here.

### InvSubBytes
Exactly like SubBytes, but uses the kInverseSBox array instead of kSBox.

### InvMixColumns
Like MixColumns, but the matrix we've multiplied by is different, so our calculations are different. Uses kMul9, kMul11, kMul13, kMul14 multiplication tables.

### DePad
Takes our deciphered output and a reference to the input size, reads the last byte (which will always be a count of the # of padded bytes), and simply reduces the size accordingly so that our main function will output less of the raw output contained in our array.

