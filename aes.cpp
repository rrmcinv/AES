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

namespace aes {

} // end namespace


using namespace std;
using namespace aes;

int main (int argc, char *argv[]){
           int option;
           while (1) {
               int this_option_optind = optind ? optind : 1;
               int option_index = 0;
               static struct option long_options[] = {
                   {"keysize",     required_argument, 0,  0 },
                   {"keyfile",  required_argument,       0,  0 },
                   {"inputfile",  required_argument, 0,  0 },
                   {"outputfile", required_argument,       0,  0 },
                   {"mode",  required_argument, 0, 'c'},
                   {0,         0,                 0,  0 }
               };

               option = getopt_long_only(argc, argv, "", long_options, &option_index);
               if (option == -1)
                   break;
               switch (option) {
               case 0:
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


	return 0;
}


