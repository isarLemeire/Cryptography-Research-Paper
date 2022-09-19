#include <stdio.h>
#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <assert.h>

using namespace std;

#define KEY_LENGTH 160
#define NONCE_LENGTH 128
#define STATE_LENGTH 320


//Cr is defined by the ascon documentation
const vector<std::string> Cr = {
    "0000000000000000000000000000000000000000000000000000000011110000",
    "0000000000000000000000000000000000000000000000000000000011100001",
    "0000000000000000000000000000000000000000000000000000000011010010",
    "0000000000000000000000000000000000000000000000000000000011000011",
    "0000000000000000000000000000000000000000000000000000000010110100",
    "0000000000000000000000000000000000000000000000000000000010100101",
    "0000000000000000000000000000000000000000000000000000000010010110",
    "0000000000000000000000000000000000000000000000000000000010000111",
    "0000000000000000000000000000000000000000000000000000000001111000",
    "0000000000000000000000000000000000000000000000000000000001101001",
    "0000000000000000000000000000000000000000000000000000000001011010",
    "0000000000000000000000000000000000000000000000000000000001001011"};

bool debug = false;

//XORs 2 bitstreams of the same length
string XOR(string str1, string str2) {
    string str =  "";
    if (debug && str1.length() != str2.length()) {
        cout << str1.length() << " " << str2.length() << endl;
    }
    assert(str1.length() == str2.length());
    for (int i = 0; i < str2.length(); i++) {
        if (str1[i] ==  str2[i]) {
            str = str + "0";
        }
        if (str1[i] != str2[i]) {
            str = str + "1";
        }
    }
    return str;
}

//ANDs 2 bitstreams of the same length
string AND(string str1, string str2) {
    string str = "";
    assert(str1.length() == str2.length());
    for (int i = 0; i < str1.length(); i++) {
        if (str1[i] == '1' && str2[i] == '1') {
            str = str + "1";
        }
        else {
            str = str + "0";
        }
    }
    return str;
}

//ANDs 2 bitstreams of the same length
string Rotate(string str, int r) {
    string outp = "";
    r = r % str.length();
    for (int i = 0; i < str.length(); i++) {
        outp = outp +  str[(i + str.length() - r) % str.length()];
    }
    return outp;
}

//Converts a string into a string containing its binary representation
string TextToBinaryString(string words) {
    string binaryString = "";
    for (char& _char : words) {
        binaryString += bitset<8>(_char).to_string();
    }
    return binaryString;
}

//Converts an integer into a string containing its binary representation
string IntToBinaryStringNonce(unsigned long long int input) {
    string s = std::bitset< NONCE_LENGTH >(input).to_string();
    return s;
}

//Converts an integer into a string containing its binary representation
string IntToBinaryStringByte(unsigned int input) {
    string s = std::bitset< 8 >(input).to_string();
    return s;
}


//ENCRYPTION FUNCTIONS------------------------------------------

void Pc(string *x2, int i, int a, int b, bool Pa) {
    int r;
    if (Pa) {
        r = i;
    }
    else {
        r = i + a - b;
    }    
    *x2 = XOR(*x2, Cr[r]);
}

void Pl(string* x0, string* x1, string* x2, string* x3, string* x4) {
    (*x0) = XOR(XOR(*x0, Rotate(*x0, 19)), Rotate(*x0, 28));
    (*x1) = XOR(XOR(*x1, Rotate(*x1, 61)), Rotate(*x1, 39));
    (*x2) = XOR(XOR(*x2, Rotate(*x2, 1)), Rotate(*x2, 6));
    (*x3) = XOR(XOR(*x3, Rotate(*x3, 10)), Rotate(*x0, 17));
    (*x4) = XOR(XOR(*x4, Rotate(*x4, 7)), Rotate(*x0, 41));
}

void Ps(string* x0, string* x1, string* x2, string* x3, string* x4) {
    (*x0) = XOR(*x0, *x4);
    (*x2) = XOR(*x2, *x1);
    (*x4) = XOR(*x4, *x3);
    string temp0 = AND(XOR(*x0, string(64, '1')), *x1);
    string temp1 = AND(XOR(*x1, string(64, '1')), *x2);
    string temp2 = AND(XOR(*x2, string(64, '1')), *x3);
    string temp3 = AND(XOR(*x3, string(64, '1')), *x4);
    string temp4 = AND(XOR(*x4, string(64, '1')), *x0);
    (*x0) = XOR(*x0, temp1);
    (*x1) = XOR(*x1, temp2);
    (*x2) = XOR(*x2, temp3);
    (*x3) = XOR(*x3, temp4);
    (*x4) = XOR(*x4, temp0);
    (*x1) = XOR(*x1, *x0);
    (*x3) = XOR(*x3, *x2);
    (*x0) = XOR(*x0, *x4);
    (*x2) = XOR(*x2, string(64, '1'));
}

string Permutation(string S, int a, int b, bool Pa) {
    string x0 = S.substr(0, STATE_LENGTH/5);
    string x1 = S.substr(STATE_LENGTH/5, STATE_LENGTH / 5);
    string x2 = S.substr(2 * (STATE_LENGTH / 5), STATE_LENGTH / 5);
    string x3 = S.substr(3 * (STATE_LENGTH / 5), STATE_LENGTH / 5);
    string x4 = S.substr(4 * (STATE_LENGTH / 5), STATE_LENGTH / 5);
    int max = a * Pa + b * (!Pa);
    for (int i = 0; i < max; i++) {
        Pc(&x2, i, a, b, Pa);
        Ps(&x0, &x1, &x2, &x3, &x4);
        Pl(&x0, &x1, &x2, &x3, &x4);
    }
    return x0 + x1 + x2 + x3 + x4;
}

string initialisation(string K, string N, int a, int b, int r) {
    string IV = IntToBinaryStringByte(K.length()) + IntToBinaryStringByte(r) + IntToBinaryStringByte(a) + IntToBinaryStringByte(b) + string(160 - K.length(), '0');
    string S = IV + K + N;
    S = Permutation(S, a, b, true);
    S = XOR(S, string(STATE_LENGTH - K.length(), '0') + K);
    if (debug) {
        cout << "initialising..." << endl;
        cout << "S = " << S << endl;
    }
    return S;
}

string processingAssociatedData(string S, string A, int a, int b, int r) {
    if (A.length() > 0) {
        A = A + "1";
        int padding = (A.length() / r + 1) * r - A.length();
        A = A + string(padding, '0');
        for (int i = 0; i < A.length() / r; i++) {
            S = XOR(S.substr(0, r), A.substr(i*r, (i+1)*r)) + S.substr(r, S.length()- r);
            S = Permutation(S, a, b, false);
        }
    }
    S = XOR(S, string(STATE_LENGTH - 1, '0') + '1');
    if (debug) {
        cout << "processing associated data..." << endl;
        cout << "S = " << S << endl;
    }
    
    return S;

}

string encrypt(string key, string nonce, string data, string plaintext, int a, int b, int r) {
    cout << "encrypting..." << endl;
    string S = initialisation(key, nonce, a, b, r);
    S = processingAssociatedData(S, data, a, b, r);
    return S;
}

//MAIN------------------------------------------
//"x64/Debug/ascon.exe" -k abc -N 10 -A blabla -P test -debug 
//inputs: K = key (max 160 bits), n = nonce (public message number, 128 bits), A= associated data, P = plaintext
//Outputs: C = ciphertext, T = authentication tag (128 bits)
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
                nonce = IntToBinaryStringNonce(n);
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
    cout << "encrypted message = " << encrypt(key, nonce, data, plaintext, a, b, r) << endl;    
    return 0;
}