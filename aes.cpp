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

namespace aes {

} // end namespace


using namespace std;
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
	char* raw_key;

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

	// open input file
	input_file_stream.open(input_file, ios::in|ios::binary|ios::ate); // end of file, binary read mode
	if (!input_file_stream.is_open()){
		cout << " failed to open input file" << endl;
		return 1;
	}
	
	input_size = input_file_stream.tellg();
	cout << "seen input size: " << input_size << " bytes" << endl;

	// open output file
	output_file_stream.open(output_file, ios::out|ios::binary); // binary write mode
	if (!output_file_stream.is_open()){
		cout << " failed to open output file" << endl;
		return 1;
	}

	return 0;
}


