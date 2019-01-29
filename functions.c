#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "functions.h"
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

bool contains(char list[10][5], char * word) {
	for (int i = 0; i < 10; i++) {
		if (list[i] == NULL) {
			return false;
		}
		if (strcmp(list[i], word) == 0) {
			return true;
		}
	}
	return false;
}

void frequency(char* in, int textLen, float letterFrequency[26], char digrams[10][3], char topTrigrams[10][4], char topQuads[10][5]) {
	char last4[4];
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

	//most common digrams
	for (int i = 1; i < textLen; i++) { //super not efficient...but it should work
		di[0] = di[1];
		di[1] = in[i];
		if (contains(digrams, di)) { //if its already a common digram, skip it
			continue;
		}
		x= countOccurences(di, in);
		for (int j = 0; j < 10; j++) {
			if (x > count[j]) {
				for (int k = 9; k >j; k--) {
					digrams[k][0] = digrams[k - 1][0];
					digrams[k][1] = digrams[k - 1][1];
				}
				count[j] = x;
				digrams[j][0] = di[0];
				digrams[j][1] = di[1];
				break;
			}
		}
	}

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

int IC(float* freq) {
	int sum;
	for (int i = 0; i < 26; i++) {
		sum += freq[i]/100.0 * freq[i]/100.0;
	}
	return sum;
}

int shiftedIC(float* freq) { //returns the value that produces the highest IC
	int sum, ic, shift;

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