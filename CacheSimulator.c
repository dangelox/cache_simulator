#include <stdio.h>
#include <stdlib.h>

/**
 Cache line Data Structure
 Following structure will hold all the data associated with line in cache.
 */
typedef struct _line_t {
	unsigned int m_tag;
	int m_vaild;

	unsigned int m_value, m_line; //testing purpose, DELETE
} line_t;

/**
 Array that mimics the direct associate cache.
 # number of entries = 32 MB / 64 B = 512
 */
line_t cache[512];

/**
 Helper function to print int to binary, DELETE
 */
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

/**
 * Main program that simulate cache mechanism
 */
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
	unsigned int value; // address read from data file
	int cache_hits = 0, cache_misses = 0; // keep track of hits and misses

	// Open file in read mode
	ptr_file = fopen(filename, "rb");

	// Check the file is valid
	if (!ptr_file) {
		printf("Error: Unable to Open File!\n");
		return (1);
	}

	// Testing info, DELETE
	fseek(ptr_file, 0, SEEK_END);
	long fileSize = ftell(ptr_file);
	rewind(ptr_file);
	long numberOfEntries = fileSize / sizeof(int);
	printf("TESTING INFO\n");
	printf("filesize          : %d\n", fileSize);
	printf("number of entries : %d\n", numberOfEntries);
	printf("------------------------------------\n");

	// Set file cursor to the beginning of the file
	fseek(ptr_file, 0, SEEK_SET);

	// Main loop, read data from file and insert to cache
	while (fread(&value, sizeof(int), 1, ptr_file) == 1) {

		// bit-shift the value read from file to calculate
		// line number and tag
		unsigned int tag = value >> 15;
		unsigned int line = ((value << 17) >> 17) >> 6;

		// Check for cache miss
		if (cache[line].m_tag != tag) {

			cache_misses++; // increment the miss counter

			// Set the tag and vaild bit.
			cache[line].m_tag = tag;
			cache[line].m_vaild = 1;

			cache[line].m_line = line; //testing purpose, DELETE
			cache[line].m_value = value; //testing purpose, DELETE
		}

		// Check for cache hit
		if ((cache[line].m_vaild == 1) && (cache[line].m_tag == tag)) {
			cache_hits++; // increase the hit counter
			// cache is not updated since it was hit
		}
	} // end while

	// print cache to console, TESTING/DELETE
	printf("CONTENTS OF CACHE AT THE END\n");
	for (int i = 0; i < 512; ++i) {
		printf("cache %d : value:%d ,tag:%d ,line:%d ,valid=%d\n", i,
				cache[i].m_value, cache[i].m_tag, cache[i].m_line,
				cache[i].m_vaild);
	}
	printf("------------------------------------\n");

	// print all the status to console
	printf("CACHE STATS\n");
	printf("Number of cache hits   : %d\n", cache_hits);
	printf("Number of cache misses : %d\n", cache_misses);
	printf("Cache hit ratio        : %2.2f%\n", ((double) cache_hits / (cache_hits + cache_misses)) * 100);
	printf("------------------------------------\n");

	// Close file
	if (fclose(ptr_file) != 0) {
		printf("Error: File Not Closed!\n");
	}

	// Clean up variables
	return (0);
}
