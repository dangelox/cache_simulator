//hello_world.c
#include <stdio.h>
#include <stdlib.h>

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

	unsigned int tlb = 251686912;
	unsigned int tl = tlb >> 6;
	unsigned int t = tl >> 9;

	unsigned int l = (tl << 17) >> 17;

	printf("tlb : %d\n", tlb);
	printf("tl  : %d\n", tl);
	printf("t   : %d\n", t);
	printf("l   : %d\n", l);

	printf("tlb : %s\n", getBinary(tlb));
	printf("tl  : %s\n", getBinary(tl));
	printf("t   : %s\n", getBinary(t));
	printf("l   : %s\n", getBinary(l));

	unsigned int tlb2 = 251686913;
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
	return (0);
}
