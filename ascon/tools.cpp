#include "tools.h"

string XOR(string str1, string str2) {
    string str = "";
    assert(str1.length() == str2.length());
    for (int i = 0; i < str2.length(); i++) {
        if (str1[i] == str2[i]) {
            str = str + "0";
        }
        if (str1[i] != str2[i]) {
            str = str + "1";
        }
    }
    return str;
}

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

string rotate(string str, int n) {
    string outp = "";
    n = n % str.length();
    for (int i = 0; i < str.length(); i++) {
        outp = outp + str[(i + str.length() - n) % str.length()];
    }
    return outp;
}

string textToBinaryString(string words) {
    string binaryString = "";
    for (char& _char : words) {
        binaryString += bitset<8>(_char).to_string();
    }
    return binaryString;
}

string intToBinaryStringNonce(unsigned long long int input) {
    string s = std::bitset< 128 >(input).to_string();
    return s;
}

string intToBinaryStringByte(unsigned int input) {
    string s = std::bitset< 8 >(input).to_string();
    return s;
}