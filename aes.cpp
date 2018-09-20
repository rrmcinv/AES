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

using namespace std;

const int nb = 4; // number of columns & rows in state per standard


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
  	for(int i = start; i < (start + size); i++)
    	cout << int(data[i]) << " ";
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
	
	void key_expansion(char* key, word* words, int nk, int words_length){
		for (int i = 0; i < nk; i++){
			words[i] = word(key[4*i], key[4*i + 1], key[4*i + 2], key[4*i + 3]);
		}
		
		word temp;
		for (int i = nk; i < words_length; i++)}
			temp = words[i-1];
			if (i % nk == 0){
				
			}
			else if ((nk > 6) && ((i%nk) == 4) {
			
			}
			words[i] = xor_words(words[i-nk], temp);
		}
	}
	
	word xor_words(word a, word b){
		world result;
		result.one = a.one^b.one;
		result.two = a.two^b.two;
		result.three = a.three^b.three;
		result.four = a.four^b.four;
		return result;
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
  	word* word_array = new word[nb*(nr+1)]; // each word is 4 bytes
    key_expansion(raw_key, word_array, nk, nb*(nr+1));
    
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
  char* raw_key = new char[key_size];
  char* raw_input;
  char* raw_output;
  
  key_file_stream.read(raw_key, key_size);

  
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


