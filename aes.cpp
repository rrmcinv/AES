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
	int keysize = -1;
	string keyfile = "";
	string inputfile = "";
	string outputfile = "";
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
						keysize = stoi(optarg);
						if (keysize != 128 && keysize != 256) 
							throw invalid_argument("wrong # of bits");
					}
					catch (...){
						cout << "error reading keysize; must be 128 or 256." << endl;
						return 1;
					}
					break;
				case 1:
					keyfile = optarg;
					break;
				case 2:
					inputfile = optarg;
					break;
				case 3:
					outputfile = optarg;
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
			printf("option %s", long_options[option_index].name);
			if (optarg)
			printf(" with arg %s", optarg);
			printf("\n");
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
	
	// open files



	return 0;
}


