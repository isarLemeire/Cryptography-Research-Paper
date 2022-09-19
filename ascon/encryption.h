#include <vector>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

#ifndef encryption
#define encryption

struct encrypted_message {
    string ciphertext;
    string tag;
    encrypted_message(string ciphertext, string tag) { this->ciphertext = ciphertext; this->tag = tag; }
};

struct decrypted_message {
    string plaintext;
    bool verification;
    decrypted_message(string plaintext, bool verification) { this->plaintext = plaintext; this->verification = verification; }
};


//Encrypts a plaintext using the ascon encrytion using a key, a nonce, the accosiated data, the plaintext, permutation round number a, pemutation round number b, and datarate r
encrypted_message encrypt(string key, string nonce, string data, string plaintext, int a, int b, int r, bool debug);

decrypted_message decrypt(string key, string nonce, string data, encrypted_message ciphertext, int a, int b, int r, bool debug);

string test();

#endif