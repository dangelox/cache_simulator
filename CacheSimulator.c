#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv []) {

	char * filename;

	//Print the table header
	if ((argv[1]) != NULL) {
		filename = argv[1];
	} else {
		printf("Enter address trace file as an argument.\n");
		printf("For example: ./CacheSimulator AddressTrace_LastIndex.bin\n");
		return(1);
	}

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

	printf("Sizes of int %d\n", sizeof(int));

	//size of file
	fseek(ptr_file, 0, SEEK_END);
	long fileSize = ftell(ptr_file);
	rewind(ptr_file);

	printf("filesize : %d\n", fileSize);

	long numberOfEntries = fileSize / sizeof(int);
	printf("number of entries : %d\n", numberOfEntries);

// Read the next address from the file
//	fseek(ptr_file, 2*sizeof(int), SEEK_SET);

	fseek(ptr_file, 0, SEEK_SET);

	int i = 0;

	while(fread(&value,sizeof(int),1, ptr_file) == 1){
		printf("value %d\n", value);
		i++;
		if (i == 11){
			break;
		}
	}

//    size_t temp = fread(&value,sizeof(int),1, ptr_file);
//	printf("temp %d\n", temp);
//
//

//	fread(&value,sizeof(int),1, ptr_file);
//	printf("value %d\n", value);
//
//	fseek(ptr_file, (64*sizeof(int))+1, SEEK_SET);
//	fread(&value, sizeof(int), 1, ptr_file);
//	printf("value %d\n", value);
//
//	fseek(ptr_file, (64*sizeof(int))+1, SEEK_SET);
//	fread(&value, sizeof(int), 1, ptr_file);
//	printf("value %d\n", value);

// Close file
	if (fclose(ptr_file) != 0) {
		printf("Error: File Not Closed!\n");
	}

	//Clean up variables
//	free(filename);
	return (0);
}
