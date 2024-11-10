#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deck.h"

char* writeCard(card_t* pCard);
char* writeDeck(deck_t* pCard);
unsigned short getRandom();

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

/* Make a standard deck of cards from a list of integers */
deck_t* makeDeckFromInt(int* pList, size_t iLen)
{
  if (!validateDeck(pList, iLen))
    return NULL;

  deck_t* pDeck = makeNullDeck(iLen);
  for (int i = 0; i < iLen; i++)
  {
    card_t* pCard = makeCard(pList[i]);
    assert(pCard != NULL);
    pDeck->cards[i] = pCard;
  }

  return pDeck;
}

/* Make a standard deck of cards from a list of integers derived from an input text key. */
deck_t* makeDeckFromKey(int* pList, size_t iLen)
{
  printf("Making a deck from a key isn't implemented (yet). Here's a regular deck.\n");
  return makeStandardDeck();
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

/* Take a list of iLen numbers and verify it is a complete deck.
   If the list is valid, return true. Otherwise, return false.*/
bool validateDeck(int* pList, size_t iLen)
{
  if (iLen != 54)
  {
    fprintf(stderr, "Invalid deck size: %lu. Input deck must be length 54.\n", iLen);
    return false;
  }

  // Loop through the input list and make a new deck, checking for invalid values or duplicates
  int pCheck[54] = { 0 }; // Used to keep track of which cards have already been found
  for (size_t i = 0; i < 54; i++)
  {
    if (pList[i] < 1 || pList[i] > 54)
    {
      fprintf(stderr, "Invalid input card value '%i' in position '%lu'. Input card value must be between 1 and 54, inclusive.\n", pList[i], i);
      return false;
    }
    pCheck[(size_t)(pList[i] - 1)]++; // Increment the *index* of the card by 1
    if (pCheck[(size_t)(pList[i] - 1)] > 1) // If the index ever exceeds 1, the card is duplicated
    {
      fprintf(stderr, "Invalid duplicate card value '%i' in position '%lu'.\n", pList[i], i);
      return false;
    }
  }
  return true;
}

/* Shuffle a deck of cards */
void shuffleDeck(deck_t* pDeck)
{
  /* Grab the top card of the deck and randomly insert it into the deck.
     Repeat until the bottom card has been moved.*/
  card_t* pLast = pDeck->cards[pDeck->nCards - 1];
  size_t randomNum = 0;
  int iCount = 0;
  bool bDone = false;
  bool bLast = false;
  while (!bDone)
  {
    iCount++;
    if (iCount > 10000)
    {
      fprintf(stderr, "The shuffle loop has exceeded 10,000 steps. Stopping...\n");
      break;
    }

    bLast = pDeck->cards[0] == pLast; // If the last card has been moved to the top, this is the last step
    randomNum = getRandom() % pDeck->nCards; // Random number between 0 and nCards (53)
    if (randomNum > 0)
      moveCard(pDeck, 0, randomNum);
    
    /* If the last card is no longer on top, the deck is shuffled.
    *  It's possible (a 1/54 chance) that the random number returned mod 53 is 0.
    *  So, a final check to see that the top card *actually* moved is required. */
    bDone = bLast && pDeck->cards[0] != pLast; 
  }
}

/* Get a random number */
unsigned short getRandom()
{
  char* randomPath = "/dev/urandom";
  FILE* f = fopen(randomPath, "r");
  if (!f)
  {
    fprintf(stderr, "Error reading from '%s': %s\n", randomPath, strerror(errno));
    return 0;
  }
  
  unsigned short rand = 0;
  size_t ret = fread(&rand, 1, sizeof(rand), f);

  if (ret != sizeof(rand))
  {
    fprintf(stderr, "Error reading '%s'\n", randomPath);
    rand = 0;
  }

  if (fclose(f) != 0)
  {
    fprintf(stderr, "Error closing '%s': %s\n", randomPath, strerror(errno));
    return 0;
  }
  
  return rand;
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

/* Write out pCard to an allocated char array */
char* writeCard(card_t* pCard)
{
  char* pOut = malloc(3 * sizeof(char));

  if (pCard->value == 1)
    pOut[0] = 'A';
  else if (pCard->value <= 9) // Values 2-9 are their value above '0'
    pOut[0] = '0' + pCard->value;
  else if (pCard->value == 10)
    pOut[0] = '0';
  else if (pCard->value == 11)
    pOut[0] = 'J';
  else if (pCard->value == 12)
    pOut[0] = 'Q';
  else if (pCard->value == 13)
    pOut[0] = 'K';
  else if (pCard->value == 53 || pCard->value == 54)
    pOut[0] = 'W';
  else
    assert(false);

  switch (pCard->suit)
  {
    case CLUBS: pOut[1] = 'c'; break;
    case DIAMONDS: pOut[1] = 'd'; break;
    case HEARTS: pOut[1] = 'h'; break;
    case SPADES: pOut[1] = 's'; break;
    default: assert(false);
  }

  pOut[2] = '\0';
  return pOut;
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
  char* pOut = writeCard(pCard);
  printf("%s",pOut);
  free(pOut);
}

/* Write out pDeck to an allocated char array*/
char* writeDeck(deck_t* pDeck)
{
  char* pOut = NULL;
  size_t iLen = 0;
  for (unsigned i = 0; i < pDeck->nCards; i++)
  {
    iLen += 3;
    pOut = realloc(pOut, iLen * sizeof(char));
    char* pCard = writeCard(pDeck->cards[i]);
    pOut[iLen - 3] = pCard[0];
    pOut[iLen - 2] = pCard[1];
    free(pCard);
    if (i+1 != pDeck->nCards)
      pOut[iLen - 1] = ' ';
    else
    {
      pOut[iLen - 1] = '\0';
    }
  }
  return pOut;
}

/* Print out a deck of cards */
void printDeck(deck_t* pDeck)
{
  char* pOut = writeDeck(pDeck);
  printf("%s\n", pOut);
  free(pOut);
}

/* Copy the input deck to an allocated output deck */
deck_t* copyDeck(deck_t* pDeck)
{
  deck_t* pOutput = makeNullDeck(pDeck->nCards);
  for (size_t i = 0; i < pDeck->nCards; i++)
  {
    card_t* pCard = malloc(sizeof(card_t));
    pCard->value = pDeck->cards[i]->value;
    pCard->suit = pDeck->cards[i]->suit;
    pOutput->cards[i] = pCard;
  }

  return pOutput;
}

/* Free all memory allocated for a deck_t. */
void freeDeck(deck_t* pDeck)
{
  if (pDeck != NULL)
  {
    for (size_t i = 0; i < pDeck->nCards; i++)
      free(pDeck->cards[i]);

    free(pDeck->cards);
    free(pDeck);
  }
}
