A C-implementation of Bruce Schneier's [solitaire cipher](https://www.schneier.com/academic/solitaire/).

# Building
In the base directory, run the command:

```
$ make
```

# Running
There are two run modes: Encryption and Decryption. Regardless of the run mode, a formatted input file is required as an input. For example, to encrypt run:

```
$ ./solitaire encrypt.txt
```

To decrypt, pass the `-d` parameter. For example:

```
$ ./solitaire -d decrypt.txt
```

After successfully running, an output file `output.txt` will be generated summarizing the run mode, formatted input/output text, and the starting/ending deck order.

# Formatting the input file
Input files are either one or two lines long; any lines beyond the first two are ignored. The first line is the input text, the optional second line specifies the deck by explicit deck order or using an encryption key. If no second line is provided, and the user is choosing to encrypt, a randomly shuffled deck of cards will be created for encryption. All decks must contain 54 cards: a standard deck plus the "A" and "B" jokers. Any non-alphabetic characters in the input text or encryption key are ignored. The maximum raw input text or encryption key length is 999.

## Using the deck order
Decks are numbered according to bridge ordering: Clubs -> Diamonds -> Hearts -> Spades, aces low. Numbering starts at 1 and continues 
to 54, with card 53 and 54 representing the "A" and "B" jokers, respectively. For example, the ace of diamonds would be numbered 14. An example input file `inputNum.txt` to encrypt the text `AAAAA` with a standard order deck would be formatted as:

```
AAAAA
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54
```
Encryption using the deck order is the standard method; no other flags are required:

```
$ ./solitaire inputNum.txt
```

## Using a deck key
Decks may also be keyed according to a passphrase. A minimum of 64 characters is required in order to ensure cryptographic security; at least 80 is recommended. An example input file to encrypt the text `AAAAA` with the key `SOLITAIRE` the file would be formatted as:

```
AAAAA
SOLITAIRE
```

In order to indicate a deck key is being used the `-k` parameter must be passed. For example, if the the text `HWWQR` was to be decrypted by a deck using the key `SOLITAIRE` the file `decryptKey.txt` should be formatted as:

```
HWWQR
SOLITAIRE
```

and the decrypt command would be called as:

```
$ ./solitaire -dk decryptKey.txt
```

# Specifying an output file

A custom-named output file may be specified using the `-o` parameter. For example, the decrypt command above can be called specifying the output file `custom.txt` as:

```
$ ./solitaire -dk input.txt -o custom.txt
```