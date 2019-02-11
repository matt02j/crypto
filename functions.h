#ifndef FUNC_H

#define FUNC_H

#ifndef MAX_KEYLEN
#define MAX_KEYLEN 30 
#endif
//ALL FUNCTIONS EXPECT TEXT TO BE ALL UPPERCASE!!

void shift(char* in, char* out, int key, int textLen); //out = in+key
void unshift(char* in, char* out, int key, int textLen); //out = in-key
void frequency(char* in, int textLen, float letterFrequency[26], char digrams[10][3], char topQuads[10][5]);
float IC(float *freq);
int shiftedIC(float* freq);
void substitution(char* in, char* out, char table[26]);
void makeSubTable(float *freq, char* table);
int countOccurences(char* substring, char* text);
void countTris(char* text, char tris[20][4]);
void kasiki_test(char *text, char tris[20][4],int* key1, int* key2); //performs kasiki test based off 10 most common tris
void vigenere(char* in, char* out, char* key, int keylen);
void permute(char *text, char * out, int width);
void printpart(char* text, int len);
int multiplicativeInverse(int num, int mod);
void printSubTbl(char *tbl);
void printgrams( char* grams[], int num);
#endif
