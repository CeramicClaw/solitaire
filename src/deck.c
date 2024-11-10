#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "deck.h"

/* Allocate a card with the desired value.
  Valid cards are values 1-54 which represent a standard deck
  of A-K Clubs->Diamonds->Hearts-Spaces followed by "A" and "B" Joker.
  "A" Joker is represented by 53 of Clubs, "B" Joker is 53 of Spades.
  Any invalid value or suit will return NULL. */
card_t* makeCard(unsigned value)
{
  if (value < 1 || value > 54)
    return NULL;

  card_t* pCard = malloc(sizeof(card_t));
  if (value <= 13) // CLUBS
  {
    pCard->value = value;
    pCard->suit = CLUBS;
  }
  else if (value <= 26) // DIAMONDS
  {
    pCard->value = value - 13;
    pCard->suit = DIAMONDS;
  }
  else if (value <= 39) // HEARTS
  {
    pCard->value = value - 26;
    pCard->suit = HEARTS;
  }
  else if (value <= 52) // SPADES
  {
    pCard->value = value - 39;
    pCard->suit = SPADES;
  }
  else if (value == 53) // Joker "A"
  {
    pCard->value = 53;
    pCard->suit = CLUBS;
  }
  else if (value == 54) // Joker "B"
  {
    pCard->value = 53;
    pCard->suit = SPADES;
  }
  return pCard;
}

/* Allocate a deck of nCards, but do not allocate the individual card_ts */
deck_t* makeNullDeck(size_t nCards)
{
  // Allocate the deck
  deck_t* pDeck = malloc(sizeof(deck_t));
  pDeck->cards = malloc(sizeof(pDeck->cards));
  pDeck->nCards = 0;
  for (size_t i = 0; i < nCards; i++)
  {
    pDeck->nCards++;
    pDeck->cards = realloc(pDeck->cards, pDeck->nCards * sizeof(pDeck->cards));
    pDeck->cards[i] = NULL;
  }
  return pDeck;
}

/* Allocate a standard deck of 52 cards plus 2 Jokers */
deck_t* makeStandardDeck()
{
  deck_t* pDeck = makeNullDeck(54); // 52 cards + 2 Jokers
  for (unsigned i = 1; i <= 54; i++)
  {
    card_t* pCard = makeCard(i);
    assert(pCard != NULL);
    pDeck->cards[i-1] = pCard;
  }
  return pDeck;
}

/* Shuffle a deck of cards */
void shuffleDeck(deck_t* pDeck)
{
  printf("Shuffling deck...\n");
  /* Grab the top card of the deck and randomly insert it into the deck
     repeat at least 54 + 54/2 + 54/3 + 54/4 + ... + 54/53 + 54/54 = 247 times.
     Use 500 just to be safe.*/
  size_t r = 0;
  for (int i = 0; i < 500; i++)
  {
    // For now, use unseeded random(). To be replaced by a /dev/urandom read "later"
    r = (size_t)random() % pDeck->nCards; // Random number between 0 and nCards (53)
    if (r > 0)
      moveCard(pDeck, 0, r);
  }
  printDeck(pDeck);
}

/* Move a card in a deck from a position, to another position */
void moveCard(deck_t* pDeck, size_t iFrom, size_t iTo)
{
  assert(pDeck->nCards > iFrom && pDeck->nCards > iTo);
  card_t* pC = pDeck->cards[iFrom];
  if (iTo < iFrom)
  {
    for (size_t i = iFrom; i > iTo; i--)
      pDeck->cards[i] = pDeck->cards[i-1];
  }
  else
  {
    for (size_t i = iFrom; i < iTo; i++)
      pDeck->cards[i] = pDeck->cards[i+1];
  }
  pDeck->cards[iTo] = pC;
}

/* Print a value/suit pair
   Ace = 'A'
   1-9 = '1' to '9'
   Jack = 'J'
   Queen = 'Q'
   King = 'K'
   Joker = 'W' (wild)
   Clubs/Diamonds/Hearts/Spades = 'c'/'d'/'h'/'s' */
void printCard(card_t* pCard)
{
  char suit = 'x';
  switch (pCard->suit)
  {
    case CLUBS: suit = 'c'; break;
    case DIAMONDS: suit = 'd'; break;
    case HEARTS: suit = 'h'; break;
    case SPADES: suit = 's'; break;
    default: assert(false);
  }

  char value = 'x';
  if (pCard->value == 1)
    value = 'A';
  else if (pCard->value <= 9) // Values 2-9 are their value above '0'
    value = '0' + pCard->value;
  else if (pCard->value == 10)
    value = '0';
  else if (pCard->value == 11)
    value = 'J';
  else if (pCard->value == 12)
    value = 'Q';
  else if (pCard->value == 13)
    value = 'K';
  else if (pCard->value == 53 || pCard->value == 54)
    value = 'W';
  else
    assert(false);

  printf("%c%c",value,suit);
}

/* Print out a deck of cards */
void printDeck(deck_t* pDeck)
{
  for (unsigned i = 0; i < pDeck->nCards; i++)
  {
    printCard(pDeck->cards[i]);
    printf(" ");
  }
  printf("\n");
}

/* Free all memory allocated for a deck_t.
   Pass "true" to bFreeCards to free all cards, too. */
void freeDeck(deck_t* pDeck, bool bFreeCards)
{
  if (bFreeCards)
  {
    for (size_t i = 0; i < pDeck->nCards; i++)
      free(pDeck->cards[i]);
  }
  free(pDeck->cards);
  free(pDeck);
}
