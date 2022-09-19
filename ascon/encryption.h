#include <vector>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

#ifndef encryption
#define encryption

//Encrypts a plaintext using the ascon encrytion using a key, a nonce, the accosiated data, the plaintext, permutation round number a, pemutation round number b, and datarate r
string encrypt(string key, string nonce, string data, string plaintext, int a, int b, int r, bool debug);

#endif