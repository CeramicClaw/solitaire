#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "cipher.h"
#include "file.h"

char* cipher(bool bEncrypt, deck_t* pDeck, char* pCipher, size_t iLen);
bool writeOutput(char* pOutput, bool bEncrypt, char* pCleanInput, char* pCleanKey, deck_t* pInputDeck, deck_t* pOutputDeck, char* pCipher);
int genKeystream(deck_t* pDeck);
void moveJokers(deck_t* pDeck);
void tripleCut(deck_t* pDeck);
void countCut(deck_t* pDeck);
int charToInt(char c);
char intToChar(int i);

/* Read the input file pInput.
   Set bEncrypt to true to encrypt the text, false to decrypt it.
   Set isDeck to true if a deck is to be used, false if key text is to be used.
   If successful, the file pOutput will be created. If pOutput is NULL, 'output.txt' is used.*/
bool run(char* pInput, bool bEncrypt, bool isDeck, char* pOutput)
{
  char* pRawInput = NULL;
  char* pRawKey = NULL;
  if (!parseFile(pInput, &pRawInput, &pRawKey))
    return false;

  // If no key is given and we're trying to decrypt, fail the calculation
  if (pRawKey == NULL && !bEncrypt)
  {
    fprintf(stderr, "Unable to decrypt, key/deck was empty.\n");
    free(pRawInput);
    return false;
  }

  // Clean the input text
  char* pCleanInput = NULL;
  size_t iLen = strlen(pRawInput) + 1;
  pCleanInput = malloc(iLen * sizeof(char));
  strncpy(pCleanInput, pRawInput, iLen);
  cleanInput(pCleanInput);
  if (strlen(pCleanInput) == 0)
  {
    fprintf(stderr, "Input text '%s' did not contain any alpha characters.\n", pRawInput);
    free(pRawInput);
    free(pRawKey);
    free(pCleanInput);
    return false;
  }

  // Clean the input key (pRawKey), if provided, and transform it into an allocated deck (pDeck)
  char* pCleanKey = NULL;
  deck_t* pDeck = NULL;
  if (pRawKey != NULL)
  {
    size_t iCleanLen = strlen(pRawKey) + 1;
    pCleanKey = malloc(iCleanLen * sizeof(char));
    strncpy(pCleanKey, pRawKey, iCleanLen);

    int* pDeckKey = NULL;
    if (isDeck)
      iCleanLen = cleanDeckKey(pCleanKey, &pDeckKey);
    else
      iCleanLen = cleanAlphaKey(pCleanKey, &pDeckKey);

    if (iCleanLen == 0)
    {
      free(pRawInput);
      free(pRawKey);
      free(pCleanInput);
      free(pCleanKey);
      return false;
    }

    if (isDeck)
      pDeck = makeDeckFromInt(pDeckKey, iCleanLen);
    else
      pDeck = makeDeckFromKey(pDeckKey, iCleanLen);

    free(pDeckKey);
  }
  
  // If no input deck/key was provided, create a random, shuffled deck of cards
  if (pDeck == NULL)
  {
    printf("No input deck/key was provided. Creating a random deck of cards...\n");
    // Allocate a random, shuffled deck of cards to pDeck
    pDeck = makeStandardDeck();
    shuffleDeck(pDeck);
  }

  // Here we *should* have a valid deck, a clean input, and a non-empty cipher text
  assert(pDeck != NULL);
  assert(pCleanInput != NULL);
  assert(strlen(pCleanInput) != 0);

  deck_t* pInputDeck = copyDeck(pDeck);
  // Also need to modify the print method to return a null-terminated string
  char* pCipher = cipher(bEncrypt, pDeck, pCleanInput, strlen(pCleanInput));
  assert(pCipher != NULL);

  // Create output file
  if (pOutput == NULL)
    pOutput = "output.txt";

  if (!writeOutput(pOutput, bEncrypt, pCleanInput, (isDeck ? NULL : pCleanKey), pInputDeck, pDeck, pCipher))
    return false;

  // Free all memory
  free(pRawInput);
  free(pRawKey);
  free(pCleanInput);
  free(pCleanKey);
  free(pCipher);
  freeDeck(pInputDeck);
  freeDeck(pDeck);

  return true;
}

/* Encode/decode text from a deck of cards.
   If encrypting, set bEncrypt to true; if decrypting set to false.
   Returned output is an allocated, null-terminated string of chars. */
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

/* Write the summary to an ouptut file 'pOutput' */
bool writeOutput(char* pOutput, bool bEncrypt, char* pCleanInput, char* pCleanKey, deck_t* pInputDeck, deck_t* pOutputDeck, char* pCipher)
{
  FILE* f = fopen(pOutput, "w");
  if (f == NULL)
  {
    fprintf(stderr, "Unable to create output file '%s': %s\n", pOutput, strerror(errno));
    return false;
  }

  // pCleanInput and pCipher are both null-terminated
  fputs(bEncrypt ? "Encrypt Mode\n" : "Decrypt Mode\n", f);
  fputs("Cleaned input text: '", f);
  fputs(pCleanInput, f);
  fputs("'\n", f);
  if (pCleanKey != NULL)
  {
    fputs("Cleaned input key: '", f);
    fputs(pCleanKey, f);
    fputs("'\nConverted deck: '", f);
  }
  else
  {
    fputs("Input deck: '", f);
  }
  char* pDeck = writeDeck(pInputDeck);
  fputs(pDeck, f);
  free(pDeck);
  fputs("'\n", f);
  fputs("Ending deck: '", f);
  pDeck = writeDeck(pOutputDeck);
  fputs(pDeck, f);
  free(pDeck);
  fputs("'\n", f);
  fputs("Output text: '", f);
  fputs(pCipher, f);
  fputs("'\n", f);

  if (fclose(f) != 0)
  {
    fprintf(stderr, "Error closing output file '%s': %s\n", pOutput, strerror(errno));
    return false;
  }
  return true;
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
  tripleCut(pDeck);
  countCut(pDeck);

  // Finally, get the output card
  card_t* pTop = pDeck->cards[0];
  size_t iValue = pTop->value;
  if (iValue != 53)
  {
    switch(pTop->suit)
    {
      case CLUBS: break;
      case DIAMONDS: iValue += 13; break;
      case HEARTS: iValue += 26; break;
      case SPADES: iValue += 39; break;
      case NUM_SUITS: assert(false); break;
    }
  }

  card_t* pOutput = pDeck->cards[iValue];
  iValue = pOutput->value;
  if (iValue == 53) // Jokers are ignored
    return genKeystream(pDeck);

  switch(pOutput->suit)
  {
    case CLUBS: break;
    case DIAMONDS: iValue += 13; break;
    case HEARTS: break;
    case SPADES: iValue += 13; break;
    case NUM_SUITS: assert(false); break;
  }
  return (int)iValue;
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
  size_t iTo = iPos + 1;
  if (iTo >= pDeck->nCards) // Wrap around back to the start
    iTo = iTo - pDeck->nCards + 1;

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
  iTo = iPos + 2;
  if (iTo >= pDeck->nCards)
    iTo = iTo - pDeck->nCards + 1;

  moveCard(pDeck, iPos, iTo);
}

/* Triple cut: Swap all cards before the first joker with all cards after the second joker */
void tripleCut(deck_t* pDeck)
{
  // Copy all cards into a temp deck, grabbing the joker positions
  card_t** pTempDeck = malloc(pDeck->nCards * sizeof(card_t*));
  size_t iJoker1 = pDeck->nCards + 1;
  size_t iJoker2 = pDeck->nCards + 1;
  for(size_t i = 0; i < pDeck->nCards; i++)
  {
    pTempDeck[i] = pDeck->cards[i];
    if (pTempDeck[i]->value == 53)
    {
      if (iJoker1 > pDeck->nCards)
        iJoker1 = i;
      else
        iJoker2 = i;
    }
  }

  // Move the cards after the second joker to the front
  size_t idx = 0;
  for (size_t i = iJoker2 + 1; i < pDeck->nCards; i++)
  {
    pDeck->cards[idx] = pTempDeck[i];
    idx++;
  }
  // Move the cards between both jokers, inclusive
  for (size_t i = iJoker1; i <= iJoker2; i++)
  {
    pDeck->cards[idx] = pTempDeck[i];
    idx++;
  }
  // Move the cards before the first joker
  for (size_t i = 0; i < iJoker1; i++)
  {
    pDeck->cards[idx] = pTempDeck[i];
    idx++;
  }
  free(pTempDeck);
}

/* Using the bottom card as a reference, cut the deck and move to the bottom leaving the bottom card intact. */
void countCut(deck_t* pDeck)
{
  // If the bottom card is a joker, do nothing
  card_t* pBottom = pDeck->cards[(pDeck->nCards - 1)];
  if (pBottom->value == 53)
    return;
  // Otherwise get the card value from 1 to 53
  size_t iValue = pBottom->value;
  switch(pBottom->suit)
  {
    case CLUBS: break;
    case DIAMONDS: iValue += 13; break;
    case HEARTS: iValue += 26; break;
    case SPADES: iValue += 39; break;
    case NUM_SUITS: assert(false); break;
  }
  // Move the cards into a temp array
  card_t** pTempDeck = malloc(pDeck->nCards * sizeof(card_t*));
  for (size_t i = 0; i < pDeck->nCards; i++)
    pTempDeck[i] = pDeck->cards[i];

  // Move all cards below to the top (except the bottom)
  size_t idx = 0;
  for (size_t i = iValue; i < pDeck->nCards - 1; i++)
  {
    pDeck->cards[idx] = pTempDeck[i];
    idx++;
  }
  // ...and move all cards on top to the bottom
  for (size_t i = 0; i < iValue; i++)
  {
    pDeck->cards[idx] = pTempDeck[i];
    idx++;
  }
  pDeck->cards[idx] = pTempDeck[idx];
  free(pTempDeck);
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
