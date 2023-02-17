#include "deck.h"

char* cipher(bool bEncrypt, deck_t* pDeck, char* pCipher, size_t iLen);
int genKeystream(deck_t* pDeck);
void moveJokers(deck_t* pDeck);
void tripleCut(deck_t* pDeck);
void countCut(deck_t* pDeck);
int charToInt(char c);
char intToChar(int i);
