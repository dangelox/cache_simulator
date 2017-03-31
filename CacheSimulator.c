#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv []) {

	// command line input filename
	char * filename;

	// Check for command line arguments
	if ((argv[1]) != NULL) {
		filename = argv[1];
	} else {
		printf("Enter address trace file as an argument.\n");
		printf("For example: ./CacheSimulator AddressTrace_LastIndex.bin\n");
		return(1);
	}

	// Setup variables for program
	FILE *ptr_file;
	int value;
	char buffer[4];

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
	while(fread(&value,sizeof(int),1, ptr_file) == 1){
		printf("value %d\n", value);
		i++;
		if (i == 11){
			break;
		}
	}

	// Close file
	if (fclose(ptr_file) != 0) {
		printf("Error: File Not Closed!\n");
	}

	// Clean up variables
	//free(filename);
	return (0);
}
