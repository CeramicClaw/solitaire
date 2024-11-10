#include <stdio.h>
#include <stdlib.h>

#include "deck.h"

int main(int argc, char ** argv)
{
  deck_t* pDeck = makeDeck();
  freeDeck(pDeck);
  return EXIT_SUCCESS;
}
