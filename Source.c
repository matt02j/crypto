
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "functions.h"

enum Cyphers{ Shift, Affine, Substitution, Vigenere };

int main(void) {
	char *cypherText, *plainText;
	enum Cyphers type;
	int textLen;
	bool done = false;
	FILE *infile = fopen("cipher2.txt", "r");
	if (infile == NULL) {
		printf("ERROR reading file\n");
		return 1;
	}
	char cypher[30000];
	char plain[30000] = { 0 };
	float freq[26];
	char digrams[10][3] = { {0 } };
	fscanf(infile, "%s", cypher);
	frequency(cypher, strlen(cypher), freq, digrams, NULL, NULL);

	//printf("%s\n", cypher);
	for (int i = 0; i < 26; i++) {
		printf("%c - %.2f \n", i + 'A', freq[i]);
	}
	for (int i = 0; i < 10; i++) {
		printf("%s \n", digrams[i]);
	}
	//char tbl[26];
	//makeSubTable(freq, tbl);


	//printf("Cipher\tplain\n");
	//for (int i = 0; i < 26; i++) {
	//	printf("%c\t%c\n", i + 'A', tbl[i]);
	//}


	//substitution(cypher, plain, tbl);
	//printf("%s \n", plain); 


	/* //  simple shift with e as highest frequency
	float max=0;
	int idx;
	for (int i = 0; i < 26; i++) {
		if (freq[i] > max) {
			max = freq[i];
			idx = i;
		}
	}
	printf(" key = %d\n", (idx - 4 + 26) % 26);
	unshift(cypher, plain, idx-4, strlen(cypher));
	printf("%s \n", plain);*/


	/* // multiplicative inverse 
	int a, n, t=0, newt=1, r, newr,q,temp;
	printf("enter a and n to find multiplicative inverse of a mod n: ");
	scanf("%d %d", &a, &n);
	r = n;     newr = a;
	while (newr != 0) {
		q = r / newr;
		temp = newt;
		newt = t - (q*newt); 
		t = temp;
		temp = newr;
		newr = r - (q*newr);
		r = temp;
		printf("| %d\t| %d\t| %d\t|\n", q, r, t);
	}
	if(r > 1 ){
		printf("a is not invertible");
	}
	else {
		if (t < 0) {
			t = t + n;
		}
			printf("multiplicative inverse of %d mod %d is %d\n", a, n, t);
		
	}
	*/


	/*  //GCD


	
	int a, b,c=1,x,y, gcd;
	printf("enter 2 numbers to find gcd: ");

	scanf("%d %d", &a, &b);
	x = a;
	y = b;
	while (c != 0) {
		c = a % b;
		a = b;
		b = c;
	}
	gcd = a;

	printf("gcd of %d and %d is %d\n", x, y, gcd);
	*/
}