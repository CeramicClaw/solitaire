#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "cards.h"

/* Allocate a card with the desired value.
  Valid cards are values 1-54 which represent a standard deck'
  of A-K Clubs->Diamonds->Hearts-Spaces followed by A and B Joker.
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
  else if (value <= 26)
  {
    pCard->value = value - 13;
    pCard->suit = DIAMONDS;
  }
  else if (value <= 39)
  {
    pCard->value = value - 26;
    pCard->suit = HEARTS;
  }
  else if (value <= 52)
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

  printf("%c%c\n",value,suit);
}
