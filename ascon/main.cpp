#include <stdio.h>
#include <iostream>
#include <string>


#include "tools.h"
#include "encryption.h"

using namespace std;

bool debug = false;

//"x64/Debug/ascon.exe" -k thisIsAKeyabcdef -N 128 -A data -P plaintext -a 12 -b 8
int main(int argc, char* argv[])
{
    //initialise variables
    string key = "";
    string nonce = "";
    string data = "";
    string plaintext = "";
    int a = 12;
    int b = 8;
    int r = 64;


    //Load inputs
    for (int i = 0; i < argc; i++)
    {
        if (string(argv[i]) == "-h" || string(argv[i]) == "--help")
        {
            cout << "-k or -key, an acsii representation of the key consisting of maximum 20 characters, must be given" << endl;
            cout << "-N or -nonce, a decimal representation of the nonce or message number consisting of maximum 128 bits, must be given" << endl;
            cout << "-A or -data, an acsii representation of the associated data" << endl;
            cout << "-P or -plaintext, an acsii representation of the plaintext, must be given" << endl;
            cout << "-a, finalisation permutation a, automatically set to 12" << endl;
            cout << "-b, finalisation permutation a, automatically set to 8" << endl;
            cout << "-r or -rate, finalisation permutation a, automatically set to 8" << endl;
            cout << "-debug, turns on debug-mode" << endl;
            return 0;
        }
        else if (string(argv[i]) == "-k" || string(argv[i]) == "-key")
        {
            if (strlen(argv[i + 1]) > KEY_LENGTH/8) {
                cout << "Key " << argv[i + 1] << " is longer than 160 bits" << endl;
                return 0;
            }
            for (int j = 0; j < strlen(argv[i+1]); j++) {
                key = textToBinaryString(argv[i + 1]);
            }
        }
        else if (string(argv[i]) == "-N" || string(argv[i]) == "-nonce")
        {
            unsigned long long int n = std::strtoull(argv[i+1], NULL, 0);
            if (n == NULL) {
                cout << "nonce " << argv[i + 1] << " is not a valid decimal number bigger than 0" << endl;
                return 0;
            }
            for (int j = 0; j < strlen(argv[i + 1]); j++) {
                nonce = intToBinaryStringNonce(n);
            }
        }
        else if (string(argv[i]) == "-a")
        {
            a = std::strtoull(argv[i + 1], NULL, 0);
            if (a == NULL) {
                cout << "-a " << argv[i + 1] << " is not a valid decimal number" << endl;
                return 0;
            }
            if (!(a==6 || a== 8 || a== 12)) {
                cout << "-a = " << argv[i + 1] << ", a should be 6, 8 or 12" << endl;
                return 0;
            }
        }
        else if (string(argv[i]) == "-b")
        {
            b = std::strtoull(argv[i + 1], NULL, 0);
            if (b == NULL) {
                cout << "-b " << argv[i + 1] << " is not a valid decimal number" << endl;
                return 0;
            }
            if (!(b == 6 || b == 8 || b == 12)) {
                cout << "-b = " << argv[i + 1] << ", b should be 6, 8 or 12" << endl;
                return 0;
            }
        }
        else if (string(argv[i]) == "-r")
        {
            r = std::strtoull(argv[i + 1], NULL, 0);
            if (b == NULL) {
                cout << "-r " << argv[i + 1] << " is not a valid decimal number bigger than 0" << endl;
                return 0;
            }
            if (b > 128) {
                cout << "-r = " << argv[i + 1] << " but should be smaller than 128" << endl;
            }
        }
        else if (string(argv[i]) == "-A" || string(argv[i]) == "-data")
        {
            data = textToBinaryString(argv[i + 1]);
        }
        else if (string(argv[i]) == "-A" || string(argv[i]) == "-data")
        {
            data = textToBinaryString(argv[i + 1]);
        }
        else if (string(argv[i]) == "-P" || string(argv[i]) == "-plaintext")
        {
            plaintext = textToBinaryString(argv[i + 1]);
        }
        else if (string(argv[i]) == "-debug") {
            debug = true;
            
        }
    }
    if (key.empty()) {
        cout <<  "No key was given, check --help for more information" << endl;
        return 0;
    }
    if (nonce.empty()) {
        cout << "No nonce was given, check --help for more information" << endl;
        return 0;
    }
    if (plaintext.empty()) {
        cout << "No plaintext was given, check --help for more information" << endl;
        return 0;
    }
    if (b > a) {
        cout << "-b should be smaller than a" << endl;
        return 0;
    }

    if (debug) {
        cout << "key = " << key << endl;
        cout << "nonce = " << nonce << endl;
        cout << "data = " << data << endl;
        cout << "plaintext = " << plaintext << endl;
        cout << "a = " << a << endl;
        cout << "b = " << b << endl;
        cout << "r = " << r << endl;
    }
    encrypted_message message = encrypt(key, nonce, data, plaintext, a, b, r, debug);
    decrypted_message outp = decrypt(key, nonce, data, message, a, b, r, debug);
    cout << "ENCRYPTING" << endl;
    cout << "plaintext = " << BinaryStringToText(plaintext) << endl;
    cout << "plaintext in bytes = " << plaintext << endl;
    cout << "cyphertext = " << message.ciphertext << endl;
    cout << "tag = " << message.tag << endl;
    cout << "DECRYPTING" << endl;
    cout << "decrypted plaintext in bytes = " << outp.plaintext << endl;
    cout << "decrypted plaintext = " << BinaryStringToText(outp.plaintext) << endl;
    cout << "verification = " << outp.verification << endl;
    return 0;
}