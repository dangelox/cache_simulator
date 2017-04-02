#include <stdio.h>
#include <stdlib.h>

typedef struct _line_t {
	unsigned int m_value;
	unsigned int m_line;
	unsigned int m_tag;
	int m_vaild;
} line_t;

line_t cache[512];

char *getBinary(unsigned int num) {
	char* bstring;
	int i;

	/* Calculate the Binary String */

	bstring = (char*) malloc(sizeof(char) * 33);
	//assert(bstring != NULL);

	bstring[32] = '\0';

	for (i = 0; i < 32; i++) {
		bstring[32 - 1 - i] = (num == ((1 << i) | num)) ? '1' : '0';
	}

	return bstring;
}

int main(int argc, char* argv[]) {

	// command line input filename
	char * filename;

	// Check for command line arguments
	if ((argv[1]) != NULL) {
		filename = argv[1];
	} else {
		printf("Enter address trace file as an argument.\n");
		printf("For example: ./CacheSimulator AddressTrace_LastIndex.bin\n");
		return (1);
	}

	// Setup variables for program
	FILE *ptr_file;
	unsigned int value;
	char buffer[4];

	int hit = 0, miss = 0;

	// Open file in read mode
	ptr_file = fopen(filename, "rb");

	// Check the file is valid
	if (!ptr_file) {
		printf("Error: Unable to Open File!\n");
		return (1);
	}

	//size of file
	fseek(ptr_file, 0, SEEK_END);
	long fileSize = ftell(ptr_file);
	rewind(ptr_file);

	printf("filesize : %d\n", fileSize);

	long numberOfEntries = fileSize / sizeof(int);
	printf("number of entries : %d\n", numberOfEntries);

	// Set file cursor to the beginning of the file
	fseek(ptr_file, 0, SEEK_SET);

	// Print first 10 vales from file to screen.
	// Testing purposes only, remove at the end.
	int i = 0;
	while (fread(&value, sizeof(int), 1, ptr_file) == 1) {
		//printf("value %d\n", value);

		unsigned int tlb = value;
//		unsigned int tl = tlb >> 6;
//		unsigned int t = tl >> 9;
//		unsigned int l = (tl << 17) >> 17;

		unsigned int t = tlb >> 15;
		unsigned int l = ((tlb << 17) >> 17) >> 6;

//		cache[i].m_value = value;
//		cache[i].m_line = l;
//		cache[i].m_tag = t;
//		cache[i].m_vaild = 1;

		// miss
		if (cache[l].m_tag != t) {
			miss++;
			cache[l].m_line = l;
			cache[l].m_tag = t;
			cache[l].m_vaild = 1;
			cache[l].m_value = value;
		}
		// hit
		if ((cache[l].m_vaild == 1) && (cache[l].m_tag == t)) {
			hit++;
		}



//		printf("i:%d ,value:%d ,tag:%d ,l:%d\n",i,tlb,t,l);

		i++;
//		if (i == 1024) {
//			break;
//		}
	}

	//print cache
	for (int i = 0; i < 512; ++i) {
		printf("cache %d : value:%d ,tag:%d ,line:%d ,valid=%d\n", i,
				cache[i].m_value, cache[i].m_tag, cache[i].m_line,
				cache[i].m_vaild);
//		printf("tlb : %s\n", getBinary(cache[i].m_value));
//		printf("t   : %s\n", getBinary(cache[i].m_tag));
//		printf("l   : %s\n", getBinary(cache[i].m_line));
//		printf("------------------\n");

	}

	//print status
	printf("hits:%d\n",hit);
	printf("miss:%d\n",miss);

	double r = ((double)hit / (hit + miss));

	printf("cache hit ratio: %2.2f%\n", r * 100);

	// Close file
	if (fclose(ptr_file) != 0) {
		printf("Error: File Not Closed!\n");
	}

	// Clean up variables
	//free(filename);
	return (0);
}
