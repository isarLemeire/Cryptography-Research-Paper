# Cryptography Research Paper

This was created as an assignment for university.

This project implements the Ascon algorithm, a constant time implementation was achieved by using bitslicing in the substitution layer.

## Execution

use -h" or --help to receive an explanation of the expected arguments.

The executable expects the following arguments:
  1. -k or -key, an ASCII representation of the key consisting of a maximum of 20 characters
  2. -N or -nonce, a decimal representation of the nonce or message number consisting of a maximum of 128 bits
  3. -P or -plaintext, an ASCII representation of the plaintext
  
Additionally the following arguments can be provided:
  1. -A or -data, an ASCII representation of the associated data
  2. -a, number of permutation rounds for p^a, default value 12
  3. -b, number of permutation rounds for p^b, default value 8
  4. -r or -rate, finalisation permutation a, default value 64
  5. -debug, turns on debug-mode, making the program print intermediate steps
  
An example of an valid command is given below:

  ./ascon.exe -k thisIsAKey -N 123 -A thisIsAssociatedData -P thisIsAPlaintext -a 12 -b 8 -r 64
