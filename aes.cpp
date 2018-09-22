// Robert McInvale
// rrm2754
// AES encryption project
// CS 361

#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdexcept>
#include <algorithm>
#include <iomanip>

using namespace std;

const int nb = 4; // number of columns & rows in state per standard

const int s_box[256] =  
 {0x63 ,0x7c ,0x77 ,0x7b ,0xf2 ,0x6b ,0x6f ,0xc5 ,0x30 ,0x01 ,0x67 ,0x2b ,0xfe ,0xd7 ,0xab ,0x76
 ,0xca ,0x82 ,0xc9 ,0x7d ,0xfa ,0x59 ,0x47 ,0xf0 ,0xad ,0xd4 ,0xa2 ,0xaf ,0x9c ,0xa4 ,0x72 ,0xc0
 ,0xb7 ,0xfd ,0x93 ,0x26 ,0x36 ,0x3f ,0xf7 ,0xcc ,0x34 ,0xa5 ,0xe5 ,0xf1 ,0x71 ,0xd8 ,0x31 ,0x15
 ,0x04 ,0xc7 ,0x23 ,0xc3 ,0x18 ,0x96 ,0x05 ,0x9a ,0x07 ,0x12 ,0x80 ,0xe2 ,0xeb ,0x27 ,0xb2 ,0x75
 ,0x09 ,0x83 ,0x2c ,0x1a ,0x1b ,0x6e ,0x5a ,0xa0 ,0x52 ,0x3b ,0xd6 ,0xb3 ,0x29 ,0xe3 ,0x2f ,0x84
 ,0x53 ,0xd1 ,0x00 ,0xed ,0x20 ,0xfc ,0xb1 ,0x5b ,0x6a ,0xcb ,0xbe ,0x39 ,0x4a ,0x4c ,0x58 ,0xcf
 ,0xd0 ,0xef ,0xaa ,0xfb ,0x43 ,0x4d ,0x33 ,0x85 ,0x45 ,0xf9 ,0x02 ,0x7f ,0x50 ,0x3c ,0x9f ,0xa8
 ,0x51 ,0xa3 ,0x40 ,0x8f ,0x92 ,0x9d ,0x38 ,0xf5 ,0xbc ,0xb6 ,0xda ,0x21 ,0x10 ,0xff ,0xf3 ,0xd2
 ,0xcd ,0x0c ,0x13 ,0xec ,0x5f ,0x97 ,0x44 ,0x17 ,0xc4 ,0xa7 ,0x7e ,0x3d ,0x64 ,0x5d ,0x19 ,0x73
 ,0x60 ,0x81 ,0x4f ,0xdc ,0x22 ,0x2a ,0x90 ,0x88 ,0x46 ,0xee ,0xb8 ,0x14 ,0xde ,0x5e ,0x0b ,0xdb
 ,0xe0 ,0x32 ,0x3a ,0x0a ,0x49 ,0x06 ,0x24 ,0x5c ,0xc2 ,0xd3 ,0xac ,0x62 ,0x91 ,0x95 ,0xe4 ,0x79
 ,0xe7 ,0xc8 ,0x37 ,0x6d ,0x8d ,0xd5 ,0x4e ,0xa9 ,0x6c ,0x56 ,0xf4 ,0xea ,0x65 ,0x7a ,0xae ,0x08
 ,0xba ,0x78 ,0x25 ,0x2e ,0x1c ,0xa6 ,0xb4 ,0xc6 ,0xe8 ,0xdd ,0x74 ,0x1f ,0x4b ,0xbd ,0x8b ,0x8a
 ,0x70 ,0x3e ,0xb5 ,0x66 ,0x48 ,0x03 ,0xf6 ,0x0e ,0x61 ,0x35 ,0x57 ,0xb9 ,0x86 ,0xc1 ,0x1d ,0x9e
 ,0xe1 ,0xf8 ,0x98 ,0x11 ,0x69 ,0xd9 ,0x8e ,0x94 ,0x9b ,0x1e ,0x87 ,0xe9 ,0xce ,0x55 ,0x28 ,0xdf
 ,0x8c ,0xa1 ,0x89 ,0x0d ,0xbf ,0xe6 ,0x42 ,0x68 ,0x41 ,0x99 ,0x2d ,0x0f ,0xb0 ,0x54 ,0xbb ,0x16};
 
 const int inverse_x_box[256] =
 {0x52 ,0x09 ,0x6a ,0xd5 ,0x30 ,0x36 ,0xa5 ,0x38 ,0xbf ,0x40 ,0xa3 ,0x9e ,0x81 ,0xf3 ,0xd7 ,0xfb
 ,0x7c ,0xe3 ,0x39 ,0x82 ,0x9b ,0x2f ,0xff ,0x87 ,0x34 ,0x8e ,0x43 ,0x44 ,0xc4 ,0xde ,0xe9 ,0xcb
 ,0x54 ,0x7b ,0x94 ,0x32 ,0xa6 ,0xc2 ,0x23 ,0x3d ,0xee ,0x4c ,0x95 ,0x0b ,0x42 ,0xfa ,0xc3 ,0x4e
 ,0x08 ,0x2e ,0xa1 ,0x66 ,0x28 ,0xd9 ,0x24 ,0xb2 ,0x76 ,0x5b ,0xa2 ,0x49 ,0x6d ,0x8b ,0xd1 ,0x25
 ,0x72 ,0xf8 ,0xf6 ,0x64 ,0x86 ,0x68 ,0x98 ,0x16 ,0xd4 ,0xa4 ,0x5c ,0xcc ,0x5d ,0x65 ,0xb6 ,0x92
 ,0x6c ,0x70 ,0x48 ,0x50 ,0xfd ,0xed ,0xb9 ,0xda ,0x5e ,0x15 ,0x46 ,0x57 ,0xa7 ,0x8d ,0x9d ,0x84
 ,0x90 ,0xd8 ,0xab ,0x00 ,0x8c ,0xbc ,0xd3 ,0x0a ,0xf7 ,0xe4 ,0x58 ,0x05 ,0xb8 ,0xb3 ,0x45 ,0x06
 ,0xd0 ,0x2c ,0x1e ,0x8f ,0xca ,0x3f ,0x0f ,0x02 ,0xc1 ,0xaf ,0xbd ,0x03 ,0x01 ,0x13 ,0x8a ,0x6b
 ,0x3a ,0x91 ,0x11 ,0x41 ,0x4f ,0x67 ,0xdc ,0xea ,0x97 ,0xf2 ,0xcf ,0xce ,0xf0 ,0xb4 ,0xe6 ,0x73
 ,0x96 ,0xac ,0x74 ,0x22 ,0xe7 ,0xad ,0x35 ,0x85 ,0xe2 ,0xf9 ,0x37 ,0xe8 ,0x1c ,0x75 ,0xdf ,0x6e
 ,0x47 ,0xf1 ,0x1a ,0x71 ,0x1d ,0x29 ,0xc5 ,0x89 ,0x6f ,0xb7 ,0x62 ,0x0e ,0xaa ,0x18 ,0xbe ,0x1b
 ,0xfc ,0x56 ,0x3e ,0x4b ,0xc6 ,0xd2 ,0x79 ,0x20 ,0x9a ,0xdb ,0xc0 ,0xfe ,0x78 ,0xcd ,0x5a ,0xf4
 ,0x1f ,0xdd ,0xa8 ,0x33 ,0x88 ,0x07 ,0xc7 ,0x31 ,0xb1 ,0x12 ,0x10 ,0x59 ,0x27 ,0x80 ,0xec ,0x5f
 ,0x60 ,0x51 ,0x7f ,0xa9 ,0x19 ,0xb5 ,0x4a ,0x0d ,0x2d ,0xe5 ,0x7a ,0x9f ,0x93 ,0xc9 ,0x9c ,0xef
 ,0xa0 ,0xe0 ,0x3b ,0x4d ,0xae ,0x2a ,0xf5 ,0xb0 ,0xc8 ,0xeb ,0xbb ,0x3c ,0x83 ,0x53 ,0x99 ,0x61
 ,0x17 ,0x2b ,0x04 ,0x7e ,0xba ,0x77 ,0xd6 ,0x26 ,0xe1 ,0x69 ,0x14 ,0x63 ,0x55 ,0x21 ,0x0c ,0x7d};

namespace aes {
  
  struct word {
  	char one, two, three, four;
  	word(char firstword, char secondword, char thirdword, char fourthword){
  		one = firstword;
  		two = secondword;
  		three = thirdword;
  		four = fourthword;
  	}
  	word(){}
  };
  
  // mostly for debug purposes
  void print_data(char* data, int size, int start = 0){
  	for(int i = start; i < (start + size); i++){
  		unsigned char c = data[i];
    	cout << hex << (int)c << " ";
    }
    cout << endl;
  }
  
  void print_state(char** state){
  	for(int j = 0; j < nb; j++){
  		for(int i = 0; i < nb; i++){
  			cout << int(state[j][i]) << " ";
  		}
  			cout << endl;
  	}
  }
  
  // pad input using CMS method
  void pad(char* bytes, int &size){
    int needed_bytes = 16 - size % 16;
    for (int i = size; i < (size + needed_bytes); i++){
    	bytes[i] = needed_bytes;
    }
		size += needed_bytes;
  }
	
	void get_state(char** state, char* input, int start_index){
		for (int i = 0; i < nb; i++){
			for (int j = 0; j < nb; j++){
				state[j][i] = input[start_index + j + 4*i];
			}
		}
	}
	
	void read_state(char** state, char* output, int start_index){
		for (int i = 0; i < nb; i++){
			for (int j = 0; j < nb; j++){
				output[start_index + j + 4*i] = state[j][i];
			}
		}
	}
	
	char get_high_nibble(char c){
		return (((1 << 4) - 1) & c);
	}
	
	char get_low_nibble(char c){
		return (((1 << 4) - 1) & (c >> 4));
	}
	
	char sub_byte(char byte){
		unsigned char c = byte;
		//cout << hex << "byte: " << int(c) << endl;
		unsigned char high_nibble = get_low_nibble(byte);
		unsigned char low_nibble = get_high_nibble(byte);
		//cout << hex << "read " << int(high_nibble) << " " << int(low_nibble) << endl;
		return s_box[high_nibble*16 + low_nibble];
	}
	
	void sub_bytes(char** state){
	
	}
	
	void sub_words(word* words){
	
	}
	
	word xor_words(word a, word b){
		word result;
		result.one = a.one^b.one;
		result.two = a.two^b.two;
		result.three = a.three^b.three;
		result.four = a.four^b.four;
		return result;
	}
	
	void key_expansion(char* key, word* words, int nk, int words_length){
		for (int i = 0; i < nk; i++){
			words[i] = word(key[4*i], key[4*i + 1], key[4*i + 2], key[4*i + 3]);
		}
		
		word temp;
		for (int i = nk; i < words_length; i++){
			temp = words[i-1];
			if (i % nk == 0){
				
			}
			else if ((nk > 6) && ((i%nk) == 4)) {
			
			}
			words[i] = xor_words(words[i-nk], temp);
		}
	}
	
	char add(char a, char b){
		return a^b;
	}
	
	char multiply(char a, char b){
	
	}
	
  void encrypt(char* raw_input, char* raw_key, char* raw_output, int key_size, int input_size){
    pad(raw_input, input_size); // pad the input appropriately
    
    const int word_size = 32; // 4 bytes
    const int nk = key_size/word_size; // standard variable, words in the key
    const int nr = nk + 6; // standard variable, should be either 10 or 14 depending on key_size
    
    if (input_size % 16 != 0 || input_size == 0){
    	cout << "padding size error" << endl;
    	return;
    }
    	
    // set up our state arrays
  	char** in_state = new char*[nb];
  	char** out_state = new char*[nb];
  	for(int j = 0; j < nb; j++){
  		in_state[j] = new char[nb];
  		out_state[j] = new char[nb];
  	}
  	
  	// get our key expansion (word array)
  	const int expanded_key_size = nb*(nr+1);
  	word* word_array = new word[expanded_key_size]; // each word is 4 bytes
    key_expansion(raw_key, word_array, nk, nb*(nr+1));
    print_data(raw_key, key_size/8);
    for(int i = 0; i < key_size/8; i++){
      unsigned char c = sub_byte(raw_key[i]);
    	cout << hex << int(c) << " ";
    }
    cout << endl;
    
    // set up main cipher loop
    for (int i = 0; i < input_size; i += 16){	
		  get_state(in_state, raw_input, i);
		  print_data(raw_input, nb*nb, i);
		  print_state(in_state);
    }
  }
  
  void decrypt(char* raw_input, char* raw_key, char* raw_output, int key_size, int input_size){
    
  }
  
} // end namespace

using namespace aes;

int main (int argc, char *argv[]){
	int key_size = -1;
	int input_size = -1;
	string key_file = "";
	string input_file = "";
	string output_file = "";
	string mode = "";

	// parse arguments
	int option;
	while (1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{"keysize",     required_argument, 0,  0 },
			{"keyfile",  required_argument,       0,  0 },
			{"inputfile",  required_argument, 0,  0 },
			{"outputfile", required_argument,       0,  0 },
			{"mode",  required_argument, 0, 0},
			{0,         0,                 0,  0 }
		};

		option = getopt_long_only(argc, argv, "", long_options, &option_index);
		if (option == -1)
			break;
		switch (option) {
		case 0:
			switch (option_index){
				case 0:
					try {
						key_size = stoi(optarg);
						if (key_size != 128 && key_size != 256) 
							throw invalid_argument("wrong # of bits");
					}
					catch (...){
						cout << "error reading keysize; must be 128 or 256." << endl;
						return 1;
					}
					break;
				case 1:
					key_file = optarg;
					break;
				case 2:
					input_file = optarg;
					break;
				case 3:
					output_file = optarg;
					break;
				case 4:
					mode = optarg;
					transform(mode.begin(), mode.end(), mode.begin(), ::tolower);
					if (mode != "encrypt" && mode != "decrypt"){
						cout << "invalid mode, must be encrypt or decrypt." << endl;
						return 1;
					}
					break;
			}
			break;
		case '?':
			break;
		default:
			printf("?? getopt returned character code 0%o ??\n", option);
		}
	}

	if (optind < argc) {
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}

	// see if they used all flags	
	if (key_size == -1 || key_file == "" || input_file == "" || output_file == "" || mode == ""){
		cout << "Usage: ./aes --keysize <128|256> --keyfile <key file name> --inputfile <input file name> --outputfile <output file name> --mode <encrypt|decrypt>" << endl;
		return 1;	
	}

	// attempt to open files
	ifstream key_file_stream, input_file_stream;
	ofstream output_file_stream;

	// open key
	key_file_stream.open(key_file, ios::in|ios::binary|ios::ate); // open at end of file in binary read mode
	if (!key_file_stream.is_open()){
		cout << " failed to open key file" << endl;
		return 1;	
	}
	
	cout << "seen key size: " << key_file_stream.tellg() << " bytes" << endl;
	if (key_file_stream.tellg() != key_size/8){
		cout << "key size mismatch, expected " << key_size << " bits, got " << key_file_stream.tellg() * 8 << " bits." << endl;
		return 1;
	}
	
	key_file_stream.seekg(0, key_file_stream.beg); // go back to start of file

	// open input file
	input_file_stream.open(input_file, ios::in|ios::binary|ios::ate); // end of file, binary read mode
	if (!input_file_stream.is_open()){
		cout << " failed to open input file" << endl;
		return 1;
	}
	
	input_size = input_file_stream.tellg();
	cout << "seen input size: " << input_size << " bytes" << endl;
	input_file_stream.seekg(0, input_file_stream.beg); // go back to start of file

	// open output file
	output_file_stream.open(output_file, ios::out|ios::binary); // binary write mode
	if (!output_file_stream.is_open()){
		cout << " failed to open output file" << endl;
		return 1;
	}

  // done opening, store data
  char* raw_key = new char[key_size/8];
  char* raw_input;
  char* raw_output;
  
  key_file_stream.read(raw_key, key_size/8);
  print_data(raw_key, key_size/8);

  
  // done storing data, call primary functions
	
	if (mode == "encrypt"){
		raw_input = new char[input_size/16 + 16]; // add extra space for the padding
		raw_output = new char[input_size/16 + 16]; 
		input_file_stream.read(raw_input, input_size); 
	  encrypt(raw_input, raw_key, raw_output, key_size, input_size);
	  }
	else if (mode == "decrypt"){
		raw_input = new char[input_size];
		raw_output = new char[input_size]; 
		input_file_stream.read(raw_input, input_size); 
	  decrypt(raw_input, raw_key, raw_output, key_size, input_size);
	}


	
	input_file_stream.close();
	key_file_stream.close();
	output_file_stream.close();


	return 0;
}


