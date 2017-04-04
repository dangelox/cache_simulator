#include <stdio.h>
#include <stdlib.h>

//Address Size
#define ADDRESS_SIZE_EXP 32

//Size of the cache
#define CACHE_SIZE_EXP 15
#define CACHE_SIZE (1 << CACHE_SIZE_EXP)

//Block Size
#define BLOCK_SIZE_EXP 6
#define BLOCK_SIZE (1 << BLOCK_SIZE_EXP)
#define BLOCK_SIZE_MASK (BLOCK_SIZE - 1)

/**
 Cache line Data Structure
 Following structure will hold all the data associated with line in cache.
 */
typedef struct _line_t {
	unsigned int m_tag;
	int m_vaild;

	unsigned int m_value, m_line; //testing purpose, DELETE
} line_t;

void printCommandFormatMessage() {
	printf("Please enter command in following format.\n");
	printf("$ ./CacheSimulator <cache associativity> <name of data file>\n");
	printf("For example: $ ./CacheSimulator 1 AddressTrace_LastIndex.bin\n");
}

/**
 * Main program that simulate cache mechanism
 */
int main(int argc, char* argv[]) {

	// command line input filename
	char * filename;
	int CACHE_ASSOCIATIVITY_EXP = 0, CACHE_ASSOCIATIVITY = 0, LINE_SIZE_EXP = 0,
			LINE_SIZE = 0;
	int TAG_EXP, TAG_SIZE, TAG_SIZE_MASK;

	if (argc < 2) {
		printf("ERROR: Insufficient command line arguments.\n");
		printCommandFormatMessage();
		return (1);
	}

	if (argc > 3) {
		printf("ERROR: Too many command line arguments.\n");
		printCommandFormatMessage();
		return (1);
	}

	// Check for command line arguments
	if (argc == 3) {
		// Check command line cache associativity argument
		if (atoi(argv[1]) == 0) {
			printf(
					"ERROR: Error in cache associativity command line argument.\n");
			return (1);
		} else if (atoi(argv[1]) == 1) {
			CACHE_ASSOCIATIVITY_EXP = 0;
		} else if (atoi(argv[1]) == 2) {
			CACHE_ASSOCIATIVITY_EXP = 1;
		} else if (atoi(argv[1]) == 4) {
			CACHE_ASSOCIATIVITY_EXP = 2;
		} else if (atoi(argv[1]) == 8) {
			CACHE_ASSOCIATIVITY_EXP = 3;
		} else {
			printf("Error: Cache associativity must be 1,2,4, or 8.\n");
			printCommandFormatMessage();
			return (1);
		}

		// Check command line input file argument
		if (argv[2] != NULL) {
			filename = argv[2];
		} else {
			printf("Error: Error in input file name command line argument.\n");
			printCommandFormatMessage();
			return (1);
		}
	} else {
		printf("ERROR: Error in command line arguments.\n");
		printCommandFormatMessage();
		return (1);
	}

	// Calculate
	CACHE_ASSOCIATIVITY = (1 << CACHE_ASSOCIATIVITY_EXP);

	LINE_SIZE_EXP =
			(CACHE_SIZE_EXP - (CACHE_ASSOCIATIVITY_EXP + BLOCK_SIZE_EXP));
	LINE_SIZE = (1 << LINE_SIZE_EXP);

	TAG_EXP = (ADDRESS_SIZE_EXP - (BLOCK_SIZE_EXP + LINE_SIZE_EXP));
	TAG_SIZE = (1 << TAG_EXP);
	TAG_SIZE_MASK = (TAG_SIZE - 1);

	printf("Cache Associativity      : %d\n", CACHE_ASSOCIATIVITY);
	printf("Number of lines in cache : %d\n", LINE_SIZE);

	/**
	 Array that mimics the direct associate cache.
	 # number of entries = 32 MB / 64 B = 512
	 */
	line_t cache[LINE_SIZE][CACHE_ASSOCIATIVITY];

	// Initialize array values to zero
	for (int i = 0; i < LINE_SIZE; ++i) {
		for (int j = 0; j < CACHE_ASSOCIATIVITY; ++j) {
			cache[i][j].m_tag = 0;
			cache[i][j].m_vaild = 0;

			cache[i][j].m_line = 0; //DELETE
			cache[i][j].m_value = 0; //DELETE
		}
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
		unsigned int tag = value >> (LINE_SIZE_EXP + BLOCK_SIZE_EXP);
		unsigned int line = ((value << TAG_EXP) >> TAG_EXP) >> BLOCK_SIZE_EXP;

		int isFound = 0;

		// Check for cache miss
		for (int r = 0; r < CACHE_ASSOCIATIVITY; ++r) {
			if ((cache[line][r].m_vaild == 1)
					&& (cache[line][r].m_tag == tag)) {
				cache_hits++; // increase the hit counter
				// cache is not updated since it was hit
				isFound = 1;
				break;
			}
		}

		// address not found
		if (isFound == 0) {
			int isCacheUpdated = 0;
			//find the first invalid or empty block and write.
			for (int s = 0; s < CACHE_ASSOCIATIVITY; ++s) {
				if (cache[line][s].m_vaild == 0) {
					//invalid block
					cache_misses++;
					cache[line][s].m_tag = tag;
					cache[line][s].m_vaild = 1;
					cache[line][s].m_line = line; //testing purpose, DELETE
					cache[line][s].m_value = value; //testing purpose, DELETE
					isCacheUpdated = 1;
					break;
				}
			}

			if (isCacheUpdated == 0) {
				//all blocks are full, have to replace one
				cache_misses++;
				cache[line][0].m_tag = tag;
				cache[line][0].m_vaild = 1;
				cache[line][0].m_line = line; //testing purpose, DELETE
				cache[line][0].m_value = value; //testing purpose, DELETE
			}
		}
	} // end while

	// Print cache, DELETE
	printf("2D CACHE\n");
	for (int i = 0; i < LINE_SIZE; ++i) {
		for (int j = 0; j < CACHE_ASSOCIATIVITY; ++j) {
			printf("[%d,%d]: %d  ", i, j, cache[i][j].m_value);
		}
		printf("\n");
	}
	printf("------------------------------------\n");

	// print all the status to console
	printf("                     CACHE REPORT\n");
	printf("=======================================================\n");
	printf("Data filename	         : %s\n", filename);
	printf("Cache Associativity      : %d\n", CACHE_ASSOCIATIVITY);
	printf("Number of lines in cache : %d\n", LINE_SIZE);
	printf("Number of Block bits     : %d\n", BLOCK_SIZE_EXP);
	printf("Number of Line bits      : %d\n", LINE_SIZE_EXP);
	printf("Number of Tag bits       : %d\n", TAG_EXP);
	printf("-------------------------------------------------------\n");
	printf("Number of cache hits     : %d\n", cache_hits);
	printf("Number of cache misses   : %d\n", cache_misses);
	printf("Cache hit ratio          : %2.2f%\n",
			((double) cache_hits / (cache_hits + cache_misses)) * 100);
	printf("-------------------------------------------------------\n");

	// Close file
	if (fclose(ptr_file) != 0) {
		printf("Error: File Not Closed!\n");
	}

	// Clean up variables

	return (0);
}
