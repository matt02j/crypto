
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "functions.h"

enum ciphers{ Shift, Affine, Substitution, Vigenere };

int main(int argc, char* argv[]) {
	if(argc < 2){
		printf("Usage: crypto FileName -o plaintext_output_file(optional)\n");
		exit(1);
	}
	char *cipherText, *plainText;
	enum ciphers type;
	int textLen;
	bool done = false;
	char cipher[30000];
	char plain[30000] = { 0 };
	float freq[26];
	char digrams[10][3] = { {0} };
	char yn,yn2='y';
	float ic;
	while(!done){
		if(yn2=='y'){ // re initialize cipher to start over
			FILE *infile = fopen(argv[1], "r");
			if (infile == NULL) {
				printf("ERROR reading file %s\n",argv[1]);
				return 1;
			}
			fscanf(infile, "%s", cipher);
			fclose(infile);
		}
		frequency(cipher, strlen(cipher), freq, digrams, NULL);
		ic = IC(freq);
		
		//printf("%s\n", cipher);
		for (int i = 0; i < 26; i++) { //testing letter frequency
			printf("%c - %.2f \n", i + 'A', freq[i]);
		}
		printf("Index of Coincidence: %f\n",ic);
		if(ic < 0.04){
			printf("probably a one time pad\n");
		}
		else if(ic < 0.055){
			printf("Recomendation: Vigenere\n");
		}
		else{
			if(shiftedIC(freq)==0){
				printf("try permutation\n");
			}
			else{
				printf("try one of the top 3\n");
			}
		}
		int choice;
		printf("based on the frequency, which type of chypher would you like to attempt?\n 1 -> Simple Shift\n 2 -> Affine \n 3 -> Substitution \n 4 -> Vigenere\n 5 -> permutation (columnar transposition)\nEnter a number: ");
		scanf("%d",&choice);
		int key;
		char tris[20][4] = { {0} };
		int key1=0, key2=0;
		switch(choice){
		case 1: //simple shift
			key = shiftedIC(freq);
			unshift(cipher, plain, key, strlen(cipher));
			printf("%s\n", plain);
			printf("This was decrypted with a key of %d, Does this make sense?(y/n)",key);
			break;
		case 2:// Affine

			break;
		case 3: // Substitution

			break;
		case 4: // Vigenere
			countTris(cipher, tris);
			kasiki_test(cipher, tris, &key1, &key2);
			printf("Most common GCD %d, Second most common %d, attemping decryption with key length %d\n", key1, key2, key1);
			key = key1;
			vigenere(cipher, plain, NULL, key);
			printpart(plain,200);
			break;
		case 5: // permutation (columnar transposition)
			countTris(cipher, tris);
			kasiki_test(cipher, tris, &key1, &key2);
			key = key1;
			permute(cipher,plain,key);
			//printf("%s\n",plain);
			printpart(plain,200);
			break;
		default:
			printf("Bad Number");
			break;
		}
	printf("would you like to continue?(for multi layer decryption) (y/n) ");
	scanf(" %c",&yn);
	if(yn == 'n'){ 
		printf("would you like to start over?(y/n) ");
		scanf(" %c",&yn2);
		if(yn2=='n'){
			break;
		}
	}
	else{
		for(int i=0;i<strlen(cipher);i++){
			cipher[i] = plain[i];
			plain[i]=0;
		}
	}
		

	}
	//for (int i = 0; i < 10; i++) { //testing digrams
	//	printf("%s \n", digrams[i]);
	//}



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
}
