#include <string>
#include <assert.h>
#include <bitset>

using namespace std;

#define KEY_LENGTH 160
#define NONCE_LENGTH 128
#define STATE_LENGTH 320
#define TAG_LENGTH 128

#ifndef tools
#define tools

//XORs 2 bitstreams of the same length
string XOR(string str1, string str2);

//ANDs 2 bitstreams of the same length
string AND(string str1, string str2);

//Rotates a bitstream ncharacters to the right
string rotate(string str, int n);

//Converts a string into a string containing its binary representation
string textToBinaryString(string words);

//Converts an integer into a string containing its binary representation with the length of the nonce
string intToBinaryStringNonce(unsigned long long int input);

//Converts an integer into a string containing its binary representation with length 8
string intToBinaryStringByte(unsigned int input);

#endif