#ifndef FUNC_H

#define FUNC_H


//ALL FUNCTIONS EXPECT TEXT TO BE ALL UPPERCASE!!

void shift(char* in, char* out, int key, int textLen); //out = in+key
void unshift(char* in, char* out, int key, int textLen); //out = in-key
void frequency(char* in, int textLen, float letterFrequency[26], char digrams[10][3], char topTrigrams[10][4], char topQuads[10][5]);
int IC(float *freq);
int shiftedIC(float* freq);
void substitution(char* in, char* out, char table[26]);
void makeSubTable(float *freq, char* table);
int countOccurences(char* substring, char* text);

#endif