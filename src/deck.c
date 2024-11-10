#include "deck.h"

/* Allocate a standard deck of playing cards plus 2 Jokers */
deck_t* makeDeck()
{
  // Allocate the deck
  deck_t* pDeck = malloc(sizeof(deck_t));
  pDeck->cards = malloc(sizeof(pDeck->cards));
  pDeck->nCards = 0;

  // Make cards 1-54, reallocating as needed
  for (unsigned i = 1; i <= 54; i++)
  {
    pDeck->nCards++;
    pDeck->cards = realloc(pDeck->cards, pDeck->nCards * sizeof(pDeck->cards));
    pDeck->cards[i-1] = makeCard(i);
  }
  return pDeck;
}

/* Free all memory allocated for a pDeck */
void freeDeck(deck_t* pDeck)
{
  for (size_t i = 0; i < pDeck->nCards; i++)
  {
    free(pDeck->cards[i]);
  }
  free(pDeck->cards);
  free(pDeck);
}
