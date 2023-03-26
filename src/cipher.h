#include <stdbool.h>
#include "deck.h"

char* cipher(bool bEncrypt, deck_t* pDeck, char* pCipher, size_t iLen);
bool run(char* pFile, bool bEncrypt, bool isDeck);
