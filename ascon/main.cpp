#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <chrono>


#include "tools.h"
#include "ascon.h"

using namespace std;
using namespace std::chrono;

bool debug = false;

int main(int argc, char* argv[])
{
    //initialise variables
    string key = "";
    string nonce = "";
    string data = "";
    string plaintext = "";

    //if not specifief a, b and r will have the values of Ascon-128
    int a = 12;
    int b = 8;
    int r = 64;


    //LOAD-INPUTS-GIVEN-BY-TERMINALl----------------------------------------------------------------------------------------------------------------------------
    //program can be run by executing for example: "x64/Debug/ascon.exe" -k thisIsAKey -N 123 -A thisIsAssociatedData -P thisIsAPlaintext -a 12 -b 8 -r 64
    for (int i = 0; i < argc; i++)
    {
        if (string(argv[i]) == "-h" || string(argv[i]) == "--help")
        {
            cout << "-k or -key, an ASCII representation of the key consisting of maximum 20 characters, must be given" << endl;
            cout << "-N or -nonce, a decimal representation of the nonce or message number consisting of maximum 128 bits, must be given" << endl;
            cout << "-A or -data, an acsii representation of the associated data" << endl;
            cout << "-P or -plaintext, an ASCII representation of the plaintext, must be given" << endl;
            cout << "-a, number of permutation rounds for pa, default value 12" << endl;
            cout << "-b, number of permutation rounds for pb, default value 8" << endl;
            cout << "-r or -rate, finalisation permutation a, default value 64" << endl;
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
            if (n == 0) {
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
            if (a == 0) {
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
            if (b == 0) {
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
            if (b == 0) {
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
    //EXECUTE-ENCRYPTION-AND-DECRYPTION----------------------------------------------------------------------------------------------------------------------------
    encrypted_message ciphertext = encrypt(key, nonce, data, plaintext, a, b, r, debug);
    decrypted_message decrypted = decrypt(key, nonce, data, ciphertext, a, b, r, debug);

    //PRINT-OUTPUT-------------------------------------------------------------------------------------------------------------------------------------------------
    //inputs
    cout << "INPUTS" << endl;
    cout << "plaintext = " << BinaryStringToText(plaintext) << endl;
    cout << "plaintext in bytes = " << plaintext << endl << endl;

    //encrypting
    cout << "ENCRYPTION" << endl;
    cout << "ciphertext = " << ciphertext.ciphertext << endl;
    cout << "tag = " << ciphertext.tag << endl << endl;

    //decrypting
    cout << "DECRYPTION" << endl;
    
    cout << "decrypted plaintext in bytes = " << decrypted.plaintext << endl;
    cout << "decrypted plaintext = " << BinaryStringToText(decrypted.plaintext) << endl;
    cout << "verification = " << decrypted.verification << endl << endl;
   

}