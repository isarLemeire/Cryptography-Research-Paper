#include <stdio.h>
#include <iostream>
#include <string>
#include <bitset>

#define KEY_LENGTH 160
#define NONCE_LENGTH 16

using namespace std;

//Converts a string into a string containing its binary representation
string TextToBinaryString(string words) {
    string binaryString = "";
    for (char& _char : words) {
        binaryString += bitset<8>(_char).to_string();
    }
    return binaryString;
}

//Converts an integer into a string containing its binary representation
string IntToBinaryString(unsigned long long int input) {
    string s = std::bitset< NONCE_LENGTH >(input).to_string();
    return s;
}


//"x64/Debug/ascon.exe" -k abc -N 10 -A blabla -P test -debug 
//inputs: K = key (max 160 bits), n = nonce (public message number, 128 bits), A= associated data, P = plaintext
//Outputs: C = ciphertext, T = authentication tag (128 bits)
int main(int argc, char* argv[])
{
    //initialise key variables
    string key = "";
    string nonce = "";
    string data = "";
    string plaintext = "";
    int a = 12;
    int b = 8;
    bool debug = false;

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
                key = TextToBinaryString(argv[i + 1]);
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
                nonce = IntToBinaryString(n);
            }
        }
        else if (string(argv[i]) == "-a")
        {
            a = std::strtoull(argv[i + 1], NULL, 0);
            if (a == NULL) {
                cout << "-a " << argv[i + 1] << " is not a valid decimal number bigger than 0" << endl;
                return 0;
            }
        }
        else if (string(argv[i]) == "-b")
        {
            b = std::strtoull(argv[i + 1], NULL, 0);
            if (b == NULL) {
                cout << "-b " << argv[i + 1] << " is not a valid decimal number bigger than 0" << endl;
                return 0;
            }
        }
        else if (string(argv[i]) == "-A" || string(argv[i]) == "-data")
        {
            data = TextToBinaryString(argv[i + 1]);
        }
        else if (string(argv[i]) == "-A" || string(argv[i]) == "-data")
        {
            data = TextToBinaryString(argv[i + 1]);
        }
        else if (string(argv[i]) == "-P" || string(argv[i]) == "-plaintext")
        {
            plaintext = TextToBinaryString(argv[i + 1]);
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

    if (debug) {
        cout << "key = " << key << endl;
        cout << "nonce = " << nonce << endl;
        cout << "data = " << data << endl;
        cout << "plaintext = " << plaintext << endl;
        cout << "a = " << a << endl;
        cout << "b = " << b << endl;
    }

    
    return 0;
}