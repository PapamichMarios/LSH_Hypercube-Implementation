#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

int main(int argc, char **argv)
{
	int opt;
	int k, M, probes;	
	int inputFileIndex, queryFileIndex, outputFileIndex;

	/*==struct for getopt long options*/
	static struct option long_options[] = 
	{
		{"d"	 , required_argument, NULL, 'd'},
		{"q"	 , required_argument, NULL, 'q'},
		{"k"	 , required_argument, NULL, 'k'},
		{"M"	 , required_argument, NULL, 'M'},
		{"o"	 , required_argument, NULL, 'o'},
		{"probes", required_argument, NULL, 'p'}
	};

	/*===get all input arguments through getopt()*/
	while ((opt = getopt_long_only(argc, argv, "", long_options, NULL)) != -1)
	{
        switch (opt)
		{
			printf("asdas %d\n", optind);
			case 'd':
				inputFileIndex = optind-1;
				break;
			case 'q':
				queryFileIndex = optind-1;
				break;
			case 'k':
				k = atoi(optarg);
				break;
			case 'M':
				M = atoi(optarg);
				break;
			case 'o':
				outputFileIndex = optind-1;
				break;
			case 'p':
				probes = atoi(optarg);
				break;
			default: 
				fprintf(stderr, "Usage: $./cube –d <input file> –q <query file> –k <int> -M <int> -probes <int> -ο <output file>\n");
				exit(EXIT_FAILURE);
		}
    }

	/*===check if user entered all arguments*/
	if( argc != 13 )
	{
		fprintf(stderr, "Usage: $./cube –d <input file> –q <query file> –k <int> -M <int> -probes <int> -ο <output file>\n");
		exit(EXIT_FAILURE);
	}
	
}
