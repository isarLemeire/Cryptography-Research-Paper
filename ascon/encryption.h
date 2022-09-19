#include <vector>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

#ifndef encryption
#define encryption

struct encrypted_message {
    string cyphertext;
    string tag;
    encrypted_message(string cyphertext, string tag) { this->cyphertext = cyphertext; this->tag = tag; }

};

//Encrypts a plaintext using the ascon encrytion using a key, a nonce, the accosiated data, the plaintext, permutation round number a, pemutation round number b, and datarate r
encrypted_message encrypt(string key, string nonce, string data, string plaintext, int a, int b, int r, bool debug);

#endif