#ifndef DECK_H
#define DECK_H
#include <stdlib.h>

/* Bridge ordering: Clubs < Diamonds < Hearts < Spades */
typedef enum
{
  CLUBS,
  DIAMONDS,
  HEARTS,
  SPADES,
  NUM_SUITS
} suit_t;

/* Aces have value 1, Jokers have value 53 */
struct card_tag
{
  unsigned value;
  suit_t suit;
};
typedef struct card_tag card_t;

struct deck_tag
{
	card_t** cards;
	size_t   nCards;
};
typedef struct deck_tag deck_t;

card_t* makeCard(unsigned value);
void printCard(card_t* pCard);
deck_t* makeDeckFromInt(int* pList, size_t iLen);
deck_t* makeDeckFromKey(int* pList, size_t iLen);
deck_t* makeNullDeck();
deck_t* makeStandardDeck();
bool validateDeck(int* pList, size_t iLen);
void shuffleDeck(deck_t* pDeck);
void moveCard(deck_t* pDeck, size_t iFrom, size_t iTo);
void moveJokers(deck_t* pDeck);
void tripleCut(deck_t* pDeck);
void countCutBottom(deck_t* pDeck);
void countCutValue(deck_t* pDeck, size_t iValue);
char* writeDeck(deck_t* pDeck);
void printDeck(deck_t* pDeck);
deck_t* copyDeck(deck_t* pDeck);
void freeDeck(deck_t* pDeck);
#endif
