#include <stdio.h>
#include <stdlib.h>

#include "cipher.h"

int main(int argc, char** argv)
{
  run(argv[1], true, true);

  deck_t* pDeck = makeStandardDeck();
  char* pCipher = "AAAAAAAAAA";
  printf("Input:  %s\n", pCipher);
  char* pOutput = cipher(true, pDeck, pCipher, 10);
  printf("Output: %s\n", pOutput);
  free (pOutput);
  freeDeck(pDeck);
  return EXIT_SUCCESS;
}
