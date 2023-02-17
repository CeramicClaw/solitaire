#include <stdio.h>
#include <stdlib.h>

#include "deck.h"
#include "encrypt.h"

int main(int argc, char** argv)
{
  deck_t* pDeck = makeStandardDeck();
  //shuffleDeck(pDeck);
  char* pCipher = "AAAAAAAAAA";
  printf("Input:  %s\n", pCipher);
  char* pOutput = cipher(true, pDeck, pCipher, 10);
  printf("Output: %s\n", pOutput);
  free (pOutput);
  freeDeck(pDeck, true);
  return EXIT_SUCCESS;
}
