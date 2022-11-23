# cryptoResearch

A constant time implementation of the Ascon algorithm.
A constant time implementation was achieved by using bitslicing in the substitution layer.

## file structure
This project in implemented using Microsoft Visual Studio. The solution can be opened by opening ascon.sln.
The source code is found in "/ascon".
The latest build is "/x64/Debug/ascon.exe".

## Execution
The location of the executable is "x64/Debug/ascon.exe".
Running "x64/Debug/ascon.exe -h" or ""x64/Debug/ascon.exe --help" will return an explonation of the expexted arguments.

The executable expects the following a arguments:
  1. -k or -key, an ASCII representation of the key consisting of maximum 20 characters
  2. -N or -nonce, a decimal representation of the nonce or message number consisting of maximum 128 bits
  3. -P or -plaintext, an acsii representation of the plaintext
  
Additionally the following arguments can be provided:
  1. -A or -data, an ACSII representation of the associated data
  2. -a, number of permutation rounds for pa, default value 12
  3. -b, number of permutation rounds for pb, default value 8
  4. -r or -rate, finalisation permutation a, default value 64
  5. -debug, turns on debug-mode, making the program print intermediate steps
  
An example of an valid input is given below:

  "x64/Debug/ascon.exe" -k thisIsAKey -N 123 -A thisIsAssociatedData -P thisIsAPlaintext -a 12 -b 8 -r 64
