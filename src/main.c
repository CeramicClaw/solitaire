#include <stdio.h>
#include <stdlib.h>

#include "deck.h"

int main(int argc, char ** argv)
{
  deck_t* pDeck = makeDeck();
  shuffleDeck(pDeck);
  freeDeck(pDeck);
  return EXIT_SUCCESS;
}
