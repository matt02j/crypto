
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "functions.h"

enum ciphers{ Shift, Affine, Substitution, Vigenere };

int main(int argc, char* argv[]) {

	char *cipherText, *plainText;
	enum ciphers type;
	int textLen,x=600;
	bool done = false;
	char cipher[30000];
	char plain[30000] = { 0 };
	float freq[26];
	char digrams[10][3] = { {0} };
	float ic;
	char infilename[50] = { 0 };
	printf("enter file name to read cipher text from: ");
	scanf(" %s", infilename);
	while(!done){
		char yn, yn2 = 'y', resub = 'n', plainLetter,cipherLetter;
		if(yn2=='y'){ // re initialize cipher to start over
			FILE *infile = fopen(infilename, "r");
			if (infile == NULL) {
				printf("ERROR reading file %s\n",infilename);
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
		printf("based on the frequency, which type of chypher would you like to attempt?\n 1 -> Simple Shift\n 2 -> Affine(use substitution) \n 3 -> Substitution \n 4 -> Vigenere\n 5 -> permutation (columnar transposition)\nEnter a number: ");
		scanf("%d",&choice);
		int key;
		char tris[20][4] = { {0} };
		int key1=0, key2=0;
		char tbl[26]={0};
		char keyword[30] = { 0 };
		switch(choice){
		case 1: //simple shift
			key = shiftedIC(freq);
			unshift(cipher, plain, key, strlen(cipher));
			printf("%s\n", plain);
			printf("This was decrypted with a key of %d\n",key);//, Does this make sense?(y/n)",key);
			break;
		case 2:// Affine
			printf("Not Implemented, Use Substitution");
			break;
		case 3: // Substitution
			countTris(cipher, tris);
			makeSubTable(freq, tris,tbl);
			while (resub == 'n') {
				printf("\n");
				printgrams(tris, 20);
				printEnglishTris();
				printf("\n");
				printSubTbl(tbl);
				printf("\n");

				printpartSub(cipher, x,tbl);
				printf("does this make sense?(y/n) or l display more text: ");
				scanf(" %c", &resub);
				if (resub == 'n') {
					printf("Enter a plain text letter and its coresponding ciphertext letter (please use caps): ");
					scanf(" %c %c", &plainLetter, &cipherLetter);
				}
				else if (resub == 'l') {
					printf("enter desired text length in characters: ");
					scanf("%d", &x);
					resub = 'n';
				}
				tbl[cipherLetter - 'A'] = plainLetter;
				for (int i = 0; i < 26; i++) {
					if (tbl[i] == plainLetter && i != cipherLetter -'A') {
						tbl[i] = 0;
						break;
					}
				}
			}
			substitution(cipher, plain, tbl);
			break;
		case 4: // Vigenere
			countTris(cipher, tris);
			kasiki_test(cipher, tris, &key1, &key2);
			printf("Most common GCD %d, Second most common %d, attemping decryption with key length %d\n", key1, key2, key1);
			key = key1;
			vigenere(cipher, plain, &keyword, key);
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

		char outfile[50];
		FILE* out;
		printf("would you like to continue?(for multi layer decryption) (y/n) ");
		scanf(" %c",&yn);
		if(yn == 'n'){ 
			printf("would you like to start over?(y/n) ");
			scanf(" %c",&yn2);
			if(yn2=='n'){
				printf("would you like to print plain text to an output file?(y/n) ");
				scanf(" %c", &yn2);
				if (yn2 == 'y') {
					printf("enter file name: ");
					scanf(" %s", outfile);
					out = fopen(outfile, "w");
					fprintf(out, "%s", plain);
					fclose(out);
				}
				printf("would you like to print stats file?(y/n) ");
				scanf(" %c", &yn2);
				if (yn2 == 'y') {
					printf("enter file name: ");
					scanf(" %s", outfile);
					out = fopen(outfile, "w");
					for (int i = 0; i < 26; i++) { //testing letter frequency
						fprintf(out,"%c - %.2f \n", i + 'A', freq[i]);
					}
					fprintf(out,"Index of Coincidence: %f\n", ic);
					if (choice == 3) {
						fprintf(out,"Cipher: ");
						for (int i = 0; i<26; i++) {
							fprintf(" %c |", i + 'A');
						}
						fprintf(out,"\nPlain:  ");
						for (int i = 0; i<26; i++) {
							if (tbl[i] == 0) {
								fprintf(out," _ |");
							}
							else {
								fprintf(out," %c |", tbl[i]);
							}
						}
						fprintf(out,"\n");
					}
					else if (choice == 1) {
						fprintf(out, "Key is %d", key);
					}
					else if (choice == 4) {
						fprintf(out, "Key is %s", keyword);
					}
					else {

					}
					fclose(out);
				}
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
}
