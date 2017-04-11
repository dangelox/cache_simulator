//*****************************************************************************
//
//	Program:		CacheSimulator.c
//	Author:			Dilesh Fernando <fernando.dilesh@gmail.com>
//					Cheng-Yeh Lee <chengyeh90@gmail.com>
//					Vuong Nguyen <nptvuong2912@gmail.com>
//	Date:			2017-04-04 (B70203)
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
#define BLOCK_SIZE_MASK (BLOCK_SIZE - 1)

/**
 * Cache line Data Structure
 * Following structure will hold all the data associated with line in cache.
 */
typedef struct _line_t {
	unsigned int m_tag;
	int m_valid;

	unsigned int m_value, m_line; //testing purpose, DELETE
} line_t;

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
	srand((unsigned) time(&t));

	// Setup variables for the program
	int CACHE_ASSOCIATIVITY_EXP = 0, CACHE_ASSOCIATIVITY = 0, LINE_SIZE_EXP = 0,
			LINE_SIZE = 0, TAG_EXP = 0, TAG_SIZE = 0, TAG_SIZE_MASK = 0;

	char * filename; // Name of command line input filename
	FILE *ptr_file;

	unsigned int value; // address read from data file
	int cache_hits = 0, cache_misses = 0; // keep track of hits and misses

	FILE *ptr_file2;
	ptr_file2 = fopen("data_8.txt", "a");

	// Check the file is valid
	if (!ptr_file2) {
		printf("Error: Unable to Open text File!\n");
		return (1);
	}

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
			CACHE_ASSOCIATIVITY_EXP = 0;
		} else if (atoi(argv[1]) == 2) {
			CACHE_ASSOCIATIVITY_EXP = 1;
		} else if (atoi(argv[1]) == 4) {
			CACHE_ASSOCIATIVITY_EXP = 2;
		} else if (atoi(argv[1]) == 8) {
			CACHE_ASSOCIATIVITY_EXP = 3;
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
	CACHE_ASSOCIATIVITY = (1 << CACHE_ASSOCIATIVITY_EXP);

	LINE_SIZE_EXP =
			(CACHE_SIZE_EXP - (CACHE_ASSOCIATIVITY_EXP + BLOCK_SIZE_EXP));
	LINE_SIZE = (1 << LINE_SIZE_EXP);

	TAG_EXP = (ADDRESS_SIZE_EXP - (BLOCK_SIZE_EXP + LINE_SIZE_EXP));
	TAG_SIZE = (1 << TAG_EXP);
	TAG_SIZE_MASK = (TAG_SIZE - 1);

	/**
	 Setup the array that mimics cache.
	 */
	line_t cache[LINE_SIZE][CACHE_ASSOCIATIVITY];

//	// Initialize array values to zero
//	for (int i = 0; i < LINE_SIZE; ++i) {
//		for (int j = 0; j < CACHE_ASSOCIATIVITY; ++j) {
//			cache[i][j].m_tag = 0;
//			cache[i][j].m_valid = 0;
//
//			cache[i][j].m_line = 0; //DELETE
//			cache[i][j].m_value = 0; //DELETE
//		}
//	}

	// Open file in read mode
	ptr_file = fopen(filename, "rb");

	// Check the file is valid
	if (!ptr_file) {
		printf("Error: Unable to Open File!\n");
		return (1);
	}

//	// Set file cursor to the beginning of the file
//	fseek(ptr_file, 0, SEEK_SET);

	for (int z = 0; z < 100; ++z) {
		cache_hits = 0;
		cache_misses = 0;

		// Initialize array values to zero
		for (int i = 0; i < LINE_SIZE; ++i) {
			for (int j = 0; j < CACHE_ASSOCIATIVITY; ++j) {
				cache[i][j].m_tag = 0;
				cache[i][j].m_valid = 0;

				cache[i][j].m_line = 0; //DELETE
				cache[i][j].m_value = 0; //DELETE
			}
		}

		// Set file cursor to the beginning of the file
		fseek(ptr_file, 0, SEEK_SET);

		// Main loop, read data from file and insert to cache
		while (fread(&value, sizeof(int), 1, ptr_file) == 1) {

			// bit-shift the value read from file to calculate
			// line number and tag
			unsigned int tag = value >> (LINE_SIZE_EXP + BLOCK_SIZE_EXP);
			unsigned int line = ((value << TAG_EXP) >> TAG_EXP)
					>> BLOCK_SIZE_EXP;

			int isFound = 0; // Auxiliary variable indicate cache hit

			// Check for cache hit
			for (int r = 0; r < CACHE_ASSOCIATIVITY; ++r) {
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
				for (int s = 0; s < CACHE_ASSOCIATIVITY; ++s) {
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
//				int temp = (CACHE_ASSOCIATIVITY-1);
					int temp =
							(rand() % ((CACHE_ASSOCIATIVITY - 1) + 1 - 0) + 0);
//				int temp = 0;

					// Update the block
					cache[line][temp].m_tag = tag;
					cache[line][temp].m_valid = 1;
				}
			}
		} // end while

	fprintf(ptr_file2, "%d\t%d\t%2.2f\n",cache_hits, cache_misses,(((double) cache_hits / (cache_hits + cache_misses)) * 100));
}
//	// Print cache, DELETE
//	printf("2D CACHE\n");
//	for (int i = 0; i < LINE_SIZE; ++i) {
//		for (int j = 0; j < CACHE_ASSOCIATIVITY; ++j) {
//			printf("[%d,%d]: %X  ", i, j, cache[i][j].m_tag);
//		}
//		printf("\n");
//	}

//	// Print cache report
//	printf("\n");
//	printf("=======================================================\n");
//	printf("                     CACHE REPORT\n");
//	printf("=======================================================\n");
//	printf("Data filename	         : %s\n", filename);
//	printf("Cache Associativity      : %d\n", CACHE_ASSOCIATIVITY);
//	printf("Size of L1 Cache         : %d\n", CACHE_SIZE);
//	printf("Size of a Block          : %d\n", BLOCK_SIZE);
//	printf("Number of lines in cache : %d\n", LINE_SIZE);
//	printf("Number of Block bits     : %d\n", BLOCK_SIZE_EXP);
//	printf("Number of Line bits      : %d\n", LINE_SIZE_EXP);
//	printf("Number of Tag bits       : %d\n", TAG_EXP);
//	printf("-------------------------------------------------------\n");
//	printf("Number of cache hits     : %d\n", cache_hits);
//	printf("Number of cache misses   : %d\n", cache_misses);
//	printf("Cache hit ratio          : %2.2f%\n",
//			((double) cache_hits / (cache_hits + cache_misses)) * 100);
//	printf("=======================================================\n");

// Close file
if (fclose(ptr_file) != 0) {
	printf("Error: File Not Closed!\n");
}

if (fclose(ptr_file2) != 0) {
	printf("Error: File text Not Closed!\n");
}

return (0);
}
