#include "tools.h"
#include "encryption.h"

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
"0000000000000000000000000000000000000000000000000000000001001011" };

void Pc(string* x2, int i, int a, int b, bool Pa) {
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
    (*x0) = XOR(XOR(*x0, rotate(*x0, 19)), rotate(*x0, 28));
    (*x1) = XOR(XOR(*x1, rotate(*x1, 61)), rotate(*x1, 39));
    (*x2) = XOR(XOR(*x2, rotate(*x2, 1)), rotate(*x2, 6));
    (*x3) = XOR(XOR(*x3, rotate(*x3, 10)), rotate(*x0, 17));
    (*x4) = XOR(XOR(*x4, rotate(*x4, 7)), rotate(*x0, 41));
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

string permute(string S, int a, int b, bool Pa) {
    string x0 = S.substr(0, STATE_LENGTH / 5);
    string x1 = S.substr(STATE_LENGTH / 5, STATE_LENGTH / 5);
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

void initialisation(string* S, string K, string N, int a, int b, int r) {
    string IV = intToBinaryStringByte(K.length()) + intToBinaryStringByte(r) + intToBinaryStringByte(a) + intToBinaryStringByte(b) + string(160 - K.length(), '0');
    *S = IV + K + N;
    *S = permute(*S, a, b, true);
    *S = XOR(*S, string(STATE_LENGTH - K.length(), '0') + K);
}

void processingAssociatedData(string* S, string A, int a, int b, int r) {
    if (A.length() > 0) {
        A = A + "1";
        int padding = (A.length() / r + 1) * r - A.length();
        A = A + string(padding, '0');
        for (int s = 0; s < A.length() / r; s++) {
            *S = XOR((*S).substr(0, r), A.substr(s * r, r)) + (*S).substr(r, (*S).length() - r);
            *S = permute(*S, a, b, false);
        }
    }
    *S = XOR(*S, string(STATE_LENGTH - 1, '0') + '1');

}

void processingPlaintext(string* S, string* C, string P, int a, int b, int r) {
    P = P + "1";
    int padding = (P.length() / r + 1) * r - P.length();
    P = P + string(padding, '0');
    for (int t = 0; t < (P.length() / r) - 1; t++) {
        string Sr = XOR((*S).substr(0, r), P.substr(t * r, r));
        *C += Sr;
        *S = Sr + (*S).substr(r, (*S).length() - r);
        *S = permute(*S, a, b, false);
    }
    string Sr = XOR((*S).substr(0, r), P.substr(P.length() - r, r));
    *S = Sr + (*S).substr(r, (*S).length() - r);
    *C += Sr.substr(0, Sr.length() - padding - 1);
}

void finalisation(string* S, string* C, string K, int a, int b, int r) {
    string temp = string(r, '0') + K + string(320 - r - K.length(), '0');
    *S = permute(XOR(*S, temp), a, b, true);
    string T; //currently T is not returned
    if (K.length() <= TAG_LENGTH) {
        T = XOR((*S).substr((*S).length() - TAG_LENGTH, TAG_LENGTH), string(TAG_LENGTH - K.length(), '0') + K);
    }
    else {
        T = XOR((*S).substr((*S).length() - TAG_LENGTH, TAG_LENGTH), K.substr(K.length() - TAG_LENGTH, TAG_LENGTH));
    }
    cout << T << " " << T.length() << endl;
}

string encrypt(string key, string nonce, string data, string plaintext, int a, int b, int r, bool debug) {
    string S = "";
    string C = "";
    initialisation(&S, key, nonce, a, b, r);
    if (debug) {
        cout << "initialising..." << endl;
        cout << "S = " << S << endl;
    }
    processingAssociatedData(&S, data, a, b, r);
    if (debug) {
        cout << "processing associated data..." << endl;
        cout << "S = " << S << endl;
    }
    processingPlaintext(&S, &C, plaintext, a, b, r);
    if (debug) {
        cout << "processing plaintext..." << endl;
        cout << "S = " << S << endl;
        cout << "C = " << C << endl;
    }
    finalisation(&S, &C, key, a, b, r);
    return S;
}