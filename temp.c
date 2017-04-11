//hello_world.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


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

int main() {

//	srand(time(NULL));
//
	unsigned int tlb = 168796156;
//	unsigned int tl = tlb >> 6;
//	unsigned int t = tl >> 9;

	unsigned int t = tlb >> 13;
	unsigned int l = ((tlb << 19) >> 19) >> 6;

	printf("tlb : %d\n", tlb);
//	printf("tl  : %d\n", tl);
	printf("t   : %d\n", t);
	printf("l   : %d\n", l);

	printf("tlb : %s\n", getBinary(tlb));
//	printf("tl  : %s\n", getBinary(tl));
	printf("t   : %s\n", getBinary(t));
	printf("l   : %s\n", getBinary(l));

	unsigned int tlb2 = 247148552;
	unsigned int tl2 = tlb2 >> 6;
	unsigned int t2 = tl2 >> 9;

	unsigned int l2 = (tl2 << 17) >> 17;

	printf("tlb : %d\n", tlb2);
	printf("tl  : %d\n", tl2);
	printf("t   : %d\n", t2);
	printf("l   : %d\n", l2);

	printf("tlb : %s\n", getBinary(tlb2));
	printf("tl  : %s\n", getBinary(tl2));
	printf("t   : %s\n", getBinary(t2));
	printf("l   : %s\n", getBinary(l2));
//
//	printf("-------------");
//	printf("Associate: %d\n", 1<<0);

//	for (int i = 0; i < 100; ++i) {
//		printf("%d\n", (rand() % (7 + 1 - 0) + 0));
//	}

//	for (int i = 0; i < 10; ++i) {
//		printf("%d\n",i);
//		if (i == 7){
//			break;
//		}
//	}

	int temp = (rand() % ((CACHE_ASSOCIATIVITY-1) + 1 - 0) + 0);
	return (0);
}
