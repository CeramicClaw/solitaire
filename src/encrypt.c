#include <assert.h>
#include <stdio.h>
#include "encrypt.h"

/* Encode/decode text from a deck of cards.
   If encrypting, set bEncrypt to TRUE; if decrypting set to FALSE.
   Returned output is a null-terminated string of chars.
*/
char* cipher(bool bEncrypt, deck_t* pDeck, char* pCipher, size_t iLen)
{
  char* pOutput = malloc((iLen + 1) * sizeof(char)); // Add 1 for \0
  int iResult = -1;
  int iKey = -1;
  for (size_t i = 0; i < iLen; i++)
  {
    iKey = genKeystream(pDeck);
    if (bEncrypt) // Encryption adds the keystream to the text
    {
      iResult = charToInt(pCipher[i]) + iKey;
      pOutput[i] = intToChar(iResult > 26 ? (iResult - 26) : iResult);
    }
    else // Decryption subtracts the keystream from the text
    {
      iResult = charToInt(pCipher[i]) - iKey;
      pOutput[i] = intToChar(iResult <= 0 ? (iResult + 26) : iResult);
    }
  }
  pOutput[iLen] = '\0';
  return pOutput;
}

/* Given a deck of cards, return the next value for encryption.
   The method is:
   1. Move "A" and "B" Jokers
   2. Perform triple cut
   3. Perform count cut
   4. Find output card and return its value */
int genKeystream(deck_t* pDeck)
{
  moveJokers(pDeck);
  return charToInt('A');
}

/* Move the "A" and "B" jokers */
void moveJokers(deck_t* pDeck)
{
  // Find the "A" joker (53 of Clubs)
  card_t* pA = NULL;
  size_t iPos = 0;
  for(size_t i = 0; i < pDeck->nCards; i++)
  {
    if (pDeck->cards[i]->value == 53 && pDeck->cards[i]->suit == CLUBS)
    {
      pA = pDeck->cards[i];
      iPos = i;
      break;
    }
  }

  if (pA == NULL)
  {
    fprintf(stderr, "Unable to find 'A' Joker.\n");
    assert(false);
  }

  // Shift "A" Joker down one card.
  // If it is the last card, it goes into position 1
  size_t iTo = iPos == (pDeck->nCards - 1) ? 1 : iPos + 1;
  moveCard(pDeck, iPos, iTo);

  // Find the "B" joker (53 of Spades)
  card_t* pB = NULL;
  for(size_t i = 0; i < pDeck->nCards; i++)
  {
    if (pDeck->cards[i]->value == 53 && pDeck->cards[i]->suit == SPADES)
    {
      pB = pDeck->cards[i];
      iPos = i;
      break;
    }
  }

  if (pB == NULL)
  {
    fprintf(stderr, "Unable to find 'B' Joker.\n");
    assert(false);
  }

  // Shift "B" Joker down two cards
  if (iPos == (pDeck->nCards - 1))
    iTo = 2;
  else if (iPos == (pDeck->nCards - 2))
    iTo = 1;
  else
    iTo = iPos + 2;

  moveCard(pDeck, iPos, iTo);
}

/* Convert char to int with 'A' = 1, 'B' = 2, etc. Input char MUST be capitalized. */
int charToInt(char c)
{
  if (c < 'A' || c > 'Z')
  {
    fprintf(stderr, "Invalid char: '%c'\n", c);
    assert(false);
    return 0;
  }

  return (int)(c - 'A' + 1);
}

/* Convert int to char with 1 = 'A', 2 = 'B', etc. Input int MUST be between 1 and 26. */
char intToChar(int i)
{
  if (i < 1 || i > 26)
  {
    fprintf(stderr, "Invalid int: %i\n", i);
    assert(false);
    return '\0';
  }
  return (char)(i + 'A' - 1);
}
