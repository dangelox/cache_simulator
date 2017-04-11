//*****************************************************************************
//
//	Program:		CacheSimulator.c
//	Author:			Dilesh Fernando <fernando.dilesh@gmail.com>
//					Cheng-Yeh Lee <chengyeh90@gmail.com>
//					Vuong Nguyen <nptvuong2912@gmail.com>
//	Date:			2017-04-04
//	Description:	A program to simulate L1 cache.
//
//*****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define global cache constants
//Address Size
#define ADDRESS_SIZE_EXP 32

//Size of the cache
#define CACHE_SIZE_EXP 15
#define CACHE_SIZE (1 << CACHE_SIZE_EXP)

//Block Size
#define BLOCK_SIZE_EXP 6
#define BLOCK_SIZE (1 << BLOCK_SIZE_EXP)

/**
 * Cache line Data Structure
 * Following structure will hold all the data associated with line in cache.
 */
typedef struct _block_t {
	unsigned int m_tag;
	int m_valid;
} block_t;

/**
 * Helper function to print command line argument error message
 */
void printCommandFormatMessage() {
	printf("Please enter command in following format.\n");
	printf("$ ./CacheSimulator <cache associativity> <name of data file>\n");
	printf("For example: $ ./CacheSimulator 1 AddressTrace_LastIndex.bin\n");
}

/**
 * Main program that simulate cache mechanism
 */
int main(int argc, char* argv[]) {

	// Random number generation variables
	time_t t;
	srand((unsigned) time(&t)); // use current time as seed value

	// Setup variables for the program
	int cache_associativity_exp = 0,
	    cache_associativity = 0,
		line_size_exp = 0,
		line_size = 0,
		tag_exp = 0,
		tag_size = 0;

	char * filename; // Name of command line input filename
	FILE *ptr_file;

	unsigned int address; // address read from data file
	int cache_hits = 0,  // keep track of cache hits
	    cache_misses = 0; // keep track of cache misses

	// Check for command line arguments
	// Check command line arguments are less than required number
	if (argc < 2) {
		printf("ERROR: Insufficient command line arguments.\n");
		printCommandFormatMessage();
		return (1);
	}

	// Check command line arguments are more than required number
	if (argc > 3) {
		printf("ERROR: Too many command line arguments.\n");
		printCommandFormatMessage();
		return (1);
	}

	// Right number of command line arguments
	if (argc == 3) {
		// Check command line cache associativity argument
		if (atoi(argv[1]) == 0) {
			printf(
					"ERROR: Error in cache associativity command line argument.\n");
			return (1);
		} else if (atoi(argv[1]) == 1) {
			cache_associativity_exp = 0;
		} else if (atoi(argv[1]) == 2) {
			cache_associativity_exp = 1;
		} else if (atoi(argv[1]) == 4) {
			cache_associativity_exp = 2;
		} else if (atoi(argv[1]) == 8) {
			cache_associativity_exp = 3;
		} else {
			// Command line argument cache associativity have failed.
			printf("Error: Cache associativity must be 1,2,4, or 8.\n");
			printCommandFormatMessage();
			return (1);
		}

		// Check command line input file argument
		if (argv[2] != NULL) {
			filename = argv[2];
		} else {
			// Command line argument filename have failed.
			printf("Error: Error in input file name command line argument.\n");
			printCommandFormatMessage();
			return (1);
		}
	} else {
		// Command line arguments have failed.
		printf("ERROR: Error in command line arguments.\n");
		printCommandFormatMessage();
		return (1);
	}

	// Calculate tag and line size
	cache_associativity = (1 << cache_associativity_exp);

	line_size_exp =
			(CACHE_SIZE_EXP - (cache_associativity_exp + BLOCK_SIZE_EXP));
	line_size = (1 << line_size_exp);

	tag_exp = (ADDRESS_SIZE_EXP - (BLOCK_SIZE_EXP + line_size_exp));
	tag_size = (1 << tag_exp);

	/**
	 Setup the array that mimics cache.
	 */
	block_t cache[line_size][cache_associativity];

	// Initialize array values to zero
	for (int i = 0; i < line_size; ++i) {
		for (int j = 0; j < cache_associativity; ++j) {
			cache[i][j].m_tag = 0;
			cache[i][j].m_valid = 0;
		}
	}

	// Open file in read mode
	ptr_file = fopen(filename, "rb");

	// Check the file is valid
	if (!ptr_file) {
		printf("Error: Unable to Open File!\n");
		return (1);
	}

	// Set file cursor to the beginning of the file
	fseek(ptr_file, 0, SEEK_SET);

	// Main loop, read data from file and insert to cache
	while (fread(&address, sizeof(int), 1, ptr_file) == 1) {

		// bit-shift the value read from file to calculate
		// line number and tag
		unsigned int tag = address >> (line_size_exp + BLOCK_SIZE_EXP);
		unsigned int line = ((address << tag_exp) >> tag_exp) >> BLOCK_SIZE_EXP;

		int isFound = 0; // Auxiliary variable indicate cache hit

		// Check for cache hit
		for (int r = 0; r < cache_associativity; ++r) {
			if ((cache[line][r].m_valid == 1)
					&& (cache[line][r].m_tag == tag)) {
				cache_hits++; // increase the hit counter
				// cache is not updated since it was hit
				isFound = 1; // address has been found in cache
				break;
			}
		}

		// Address is not found in cache will result a cache miss
		if (isFound == 0) {
			int isCacheUpdated = 0; // Auxiliary variable indicate cache has been updated

			// Find the first invalid or empty block and write.
			// This will only occur  starting from a empty cache
			for (int s = 0; s < cache_associativity; ++s) {
				if ((cache[line][s].m_valid == 0)
						&& (cache[line][s].m_tag == 0)) {
					// Found the first invalid block
					cache_misses++; // increase the miss count

					// Update the block
					cache[line][s].m_tag = tag;
					cache[line][s].m_valid = 1;

					isCacheUpdated = 1; // Set auxiliary variable
					break;
				}
			}

			// Cache already full, now have to replace a block
			if (isCacheUpdated == 0) {
				cache_misses++; // increment the miss count

				// Replace the cache block
				// Cache is replaced in random order
				int temp = (rand() % ((cache_associativity - 1) + 1 - 0) + 0);

				// Update the block
				cache[line][temp].m_tag = tag;
				cache[line][temp].m_valid = 1;
			}
		}
	} // end while

	// Print cache report
	printf("\n");
	printf("=======================================================\n");
	printf("                     CACHE REPORT\n");
	printf("=======================================================\n");
	printf("Data filename	         : %s\n", filename);
	printf("Cache Associativity      : %d\n", cache_associativity);
	printf("Size of L1 Cache         : %d\n", CACHE_SIZE);
	printf("Size of a Block          : %d\n", BLOCK_SIZE);
	printf("Number of lines in cache : %d\n", line_size);
	printf("Number of Block bits     : %d\n", BLOCK_SIZE_EXP);
	printf("Number of Line bits      : %d\n", line_size_exp);
	printf("Number of Tag bits       : %d\n", tag_exp);
	printf("-------------------------------------------------------\n");
	printf("Number of cache hits     : %d\n", cache_hits);
	printf("Number of cache misses   : %d\n", cache_misses);
	printf("Cache hit ratio          : %2.2f%\n",
			((double) cache_hits / (cache_hits + cache_misses)) * 100);
	printf("=======================================================\n");

	// Close file
	if (fclose(ptr_file) != 0) {
		printf("Error: File Not Closed!\n");
	}

	return (0);
}
