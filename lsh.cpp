#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int opt;
	int k, L;	
	int inputFileIndex, queryFileIndex, outputFileIndex;

	/*===get all input arguments through getopt()*/
	while ((opt = getopt(argc, argv, "d:q:k:L:o:")) != -1)
	{
        switch (opt)
		{
			case 'd':
				inputFileIndex = optind-1;
				break;
			case 'q':
				queryFileIndex = optind-1;
				break;
			case 'k':
				k = atoi(optarg);
				break;
			case 'L':
				L = atoi(optarg);
				break;
			case 'o':
				outputFileIndex = optind-1;
				break;
			default: 
				fprintf(stderr, "Usage: ./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file>\n");
				exit(EXIT_FAILURE);
		}
    }

	/*===check if user entered all arguments*/
	if( argc != 11)
	{
		fprintf(stderr, "Usage: ./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file>\n");
		exit(EXIT_FAILURE);
	}
	
}
