#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "functions.h"

#define MAX_KEYLEN 15 

//convert frequencies to decimal instead of percentage to save computation time
float englishFrequencies[26] = { 8.167,1.492,2.782,4.253,12.702,2.228,2.015,6.094,6.966,0.153,0.772,4.025,2.406,6.749,7.507,1.929,0.095,5.987,6.327,9.056,2.758,0.978,2.360,0.150,1.974,0.074 };
char mostCommonEnglishLetter[26] = { 'e','t','a','o','i','n','s','h','r','d','l','c','u','m','w','f','g','y','p','b','v','k','j','x','q','z' };

void shift(char* in, char* out, int key, int textLen) { //out = in+key
	for (int i = 0; i < textLen; i++) {
		out[i] = (in[i]-'A' +26+ key)%26 +'A';
	}

}
void unshift(char* in, char* out, int key, int textLen) { //out = in-key
	shift(in, out, -key, textLen);
}

bool contains(char list[][4], char * word, int size) {
	for (int i = 0; i < size; i++) {
		if (list[i] == NULL) {
			return false;
		}
		if (strcmp(list[i], word) == 0) {
			return true;
		}
	}
	return false;
}

void frequency(char* in, int textLen, float letterFrequency[26], char digrams[10][3], char topQuads[10][5]) {

	int letterCount[26] = { 0 };
	for (int i = 0; i < textLen; i++) { //count characters
		letterCount[in[i] - 'A'] ++;
	}
	for (int i = 0; i < 26; i++) { //calculate frequency
		letterFrequency[i] = (letterCount[i] / (float)textLen )*100;
	}
	int count[10] = { 0 } , x;
	char di[3]; di[2] = 0; // null terminated the string, dont change this character
	di[1] = in[0];
	char tri[4]; tri[3] = 0;

	//most common digrams
	//for (int i = 1; i < textLen/2; i++) { //super not efficient...but it should work
	//	di[0] = di[1];
	//	di[1] = in[i];
	//	if (contains(digrams, di,10)) { //if its already a common digram, skip it
	//		continue;
	//	}
	//	x= countOccurences(di, in);
	//	for (int j = 0; j < 10; j++) {
	//		if (x > count[j]) {
	//			for (int k = 9; k >j; k--) {
	//				digrams[k][0] = digrams[k - 1][0];
	//				digrams[k][1] = digrams[k - 1][1];
	//				count[k] = count[k - 1];
	//			}
	//			count[j] = x;
	//			digrams[j][0] = di[0];
	//			digrams[j][1] = di[1];
	//			break;
	//		}
	//	}
	//}

}

void countTris(char* text, char tris[20][4]) {
	int x;
	int count[20] = { 0 };
	char tri[4];
	tri[1] = text[0]; tri[2] = text[1]; tri[3] = 0;
	for (int i = 2; i < strlen(text) / 2; i++) {
		tri[0] = tri[1]; tri[1] = tri[2]; tri[2] = text[i];
		if (contains(tris, tri, 20)) {
			continue;
		}
				
		//count tris
		x = countOccurences(tri, text);
		for (int j = 0; j < 20; j++) {
			if (x > count[j]) {
				for (int k = 19; k > j; k--) {
					tris[k][0] = tris[k - 1][0];
					tris[k][1] = tris[k - 1][1];
					tris[k][2] = tris[k - 1][2];
					count[k] = count[k - 1];
				}
				count[j] = x;
				tris[j][0] = tri[0];
				tris[j][1] = tri[1];
				tris[j][2] = tri[2];
				break;
			}
		}
	}
}

void kasiki_test(char *text, char tris[20][4], int* key1, int *key2) { //performs kasiki test based off 10-20 most common tris
	// find all occurences as of most common tris and their indexes
	// find most common gcd of all indexes
	// 
	int idx[20][20] = { {0} }; // up to 20 indexes for up to 20 tris
	int factors[MAX_KEYLEN+1] = { 0 };
	char * ptr=text;
	*key1 = 0; *key2 = 0;
	for (int i = 0; i <20; i++) {
		ptr = text;
		for (int j = 0; j < 20 && ptr <text+strlen(text); j++) {
			ptr = strstr(ptr, tris[i]);
			if (ptr == NULL) {
				break;
			}
			idx[i][j] = ptr - text;
			ptr++;
		}
	}
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 19; j++) {
			if (idx[i][j] == 0 && j >= 1) {
				break;
			}
			for (int k = 2; k < MAX_KEYLEN+1; k++) {  // count occurences of each factor from 2 to MAX_KEYLEN
				if (idx[i][j + 1] - idx[i][j] % k == 0) {
					factors[k]++;
				}
			}
		}
	}
	for (int i = 2; i < MAX_KEYLEN + 1; i++){
		if (factors[i] > *key1) {
			*key2 = *key1;
			*key1 = i;
		}
		printf(" %d -> %d\n", i, factors[i]);
	}

}
void vigenere(char * in, char * out, char * key, int keylen){ // if key is null try to find it with frequency analysis
	int keys[MAX_KEYLEN]; // up to 15 letter keys
	float freq[MAX_KEYLEN][26];
	int textLen = strlen(in);
	if (key == NULL) {
		for (int j = 0; j < keylen; j++) {
			int letterCount[26] = { 0 };
			for (int i = j; i < textLen; i += keylen) { //count characters
				letterCount[in[i] - 'A'] ++;
			}
			for (int i = 0; i < 26; i++) { //calculate frequency
				freq[j][i] = (letterCount[i] / (float)textLen) * 100;
			}
			keys[j] = shiftedIC(freq[j]);
		}
	}
	for (int i = 0; i < keylen; i++) {
		printf("key %d is %d\n", i, keys[i]);
	}
	for (int i = 0; i < textLen; i++) {
		out[i] = (in[i] - 'A' + keys[i%keylen]) % 26 + 'A';
	}
}
int GCD(int a, int b) {
	int c = 1, x, y, gcd;
	x = a;
	y = b;
	while (c != 0) {
		c = a % b;
		a = b;
		b = c;
	}
	gcd = a;
}
int countOccurences(char* substring, char* text) {
	int count=0;
	char *idx=text;
	while(idx<(text+strlen(text))){
		idx = strstr(idx, substring);
		if (idx == NULL) {
			break;
		}
		count++;
		idx++;
	}
	return count;
}

void makeSubTable(float *freq, char* table) {
	int idx;
	float max, lastmax=100;
	for (int i = 0; i < 26; i++) {
		max = -1;
		for (int j = 0; j < 26; j++) {
			if (freq[j] > max && freq[j] < lastmax) {
				max = freq[j];
				idx = j;
			}
		}
		lastmax = max;
		table[idx] = mostCommonEnglishLetter[i];
	}
}

void substitution(char* in, char* out, char table[26]) {// index is cyphertext, content is the coresponding plaintext character
	for (int i = 0; i < strlen(in); i++) {
		out[i] = table[in[i] - 'A'];
	}
}

float IC(float* freq) {
	float sum=0;
	for (int i = 0; i < 26; i++) {
		sum += freq[i]/100.0 * freq[i]/100.0;
	}
	return sum;
}

int shiftedIC(float* freq) { //returns the value that produces the highest IC
	float sum, ic = 0;
	int shift = 0;

	for (int i = 0; i < 26; i++) {
		sum = 0;
		for (int j = 0; j < 26; j++) {
			sum += englishFrequencies[j]/100.0 * freq[(i + j) % 26];
		}
		if (sum > ic) {
			ic = sum;
			shift = i;
		}
	}
	return shift;
}

void permute(char *text, char * out, int width) {
	int len = strlen(text);
	for (int i = 0; i < strlen(text); i++) {
		out[i] = text[ i*(len / width) % len ];
	}

}