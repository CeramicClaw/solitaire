#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "file.h"

/* Read in the first two lines of the input text file and allocate the raw text to the input arrays.
   If the last character in the line is a line break, it will be removed.
   Returned key array may be null if line was blank or missing.
   If an array is non-null, it is guaranteed to be null-terminated.
   Returns true if file was read successfully and cipher text was not blank.
   If false is returned, no memory was allocated. */
bool parseFile(char* pFile, char** pInput, char** pKey)
{
  if (pFile  == NULL)
  {
    fprintf(stderr, "Input filename was null.\n");
    return false;
  }

  assert(pInput != NULL);
  assert(*pInput == NULL);
  assert(pKey != NULL);
  assert(*pKey == NULL);

  FILE* f = fopen(pFile, "r");
  if (f == NULL)
  {
    fprintf(stderr, "Error opening file '%s': %s.\n", pFile, strerror(errno));
    return false;
  }

  // Get the file contents. Assume the first line is the input text and the second is either the key or the deck
  // The key/deck is optional
  char pCipherText[1000]; // Raw text
  char pKeyText[1000]; // Raw key text
  char pLine[1000];
  size_t iLine = 0;
  while (fgets(pLine, 1000, f) != NULL) // fgets always returns a null-terminated string
  {
    iLine++;
    if (iLine == 1)
      strncpy(pCipherText, pLine, 1000);
    else if (iLine == 2)
      strncpy(pKeyText, pLine, 1000);
    else
      break;
  }

  if (fclose(f) != 0)
  {
    fprintf(stderr, "Error closing file '%s': %s\n", pFile, strerror(errno));
    return false;
  }

  // Strip off any trailing line breaks
  if (strlen(pCipherText) > 0 && pCipherText[strlen(pCipherText) - 1] == '\n')
    pCipherText[strlen(pCipherText) - 1] = '\0';

  if (strlen(pKeyText) > 0 && pKeyText[strlen(pKeyText) - 1] == '\n')
    pKeyText[strlen(pKeyText) - 1] = '\0';

  if (strlen(pCipherText) == 0)
  {
    fprintf(stderr, "Input cipher text was blank\n");
    return false;
  }
  else
  {
    size_t iLen = strlen(pCipherText) + 1;
    *pInput = malloc(iLen * sizeof(char));
    strncpy(*pInput, pCipherText, iLen);
  }

  if (strlen(pKeyText) == 0)
  {
    *pKey = NULL;
  }
  else
  {
    size_t iLen = strlen(pKeyText) + 1;
    *pKey = malloc(iLen * sizeof(char));
    strncpy(*pKey, pKeyText, iLen);
  }

  return true;
}

/* Clean the input text to an alpha-only, all caps, null-terminated string. */
void cleanInput(char* pInput)
{
  size_t iLen = strlen(pInput);

  // Overwrite the input array with the cleaned array, appending the null terminator
  size_t iFinal = 0;
  for (size_t i = 0; i < iLen; i++)
  {
    if (isalpha(pInput[i]))
    {
      pInput[iFinal] = toupper(pInput[i]);
      iFinal++;
    }
  }
  pInput[iFinal] = '\0';
}

/* Convert the input alpha key to an allocated array of numbers. The returned size_t is the length of the array.
   Input char array pKey will be cleaned to a all caps, alpha-only, null-terminated string.
   Any non-alpha characters will be ignored. */
size_t cleanAlphaKey(char* pKey, int** pNum)
{
  assert(pNum != NULL);
  assert(*pNum == NULL);

  size_t iLen = strlen(pKey);
  size_t iFinal = 0;
  for (size_t i = 0; i < iLen; i++)
  {
    if (isalpha(pKey[i]))
    {
      pKey[iFinal] = toupper(pKey[i]);
      iFinal++;
      *pNum = realloc(*pNum, iFinal * sizeof(int));
      (*pNum)[iFinal - 1] = (int)(toupper(pKey[i]) - 'A' + 1);
    }
  }

  if (iFinal == 0)
  {
    fprintf(stderr, "Input key '%s' was not a string of alphabetical characters.\n", pKey);
  }
  else
  {
    pKey[iFinal] = '\0';
  }
  return iFinal;
}

/* Convert the input numeric deck to an allocated array of ints.
   If the input deck is invalid, NULL will be returned.
   Any non-numeric characters will be ignored. */
size_t cleanDeckKey(char* pKey, int** pNum)
{
  assert(pKey != NULL);
  assert(pNum != NULL);
  assert(*pNum == NULL);

  // Read through the text using strtol to convert the spaced array of numbers into an allocated array of ints
  size_t iFinal = 0;
  char* pEnd = pKey;
  int iValue = 0;
  do
  {
    iValue = (int)(strtol(pEnd, &pEnd, 10));
    if (iValue != 0)
    {
      iFinal++;
      *pNum = realloc(*pNum, iFinal * sizeof(int));
      (*pNum)[iFinal - 1] = iValue;
    }
  }
  while (iValue != 0 && *pEnd != '\n'); // Keep reading as long as you only hit spaces and numbers

  if (!validateDeck(*pNum, iFinal))
  {
    iFinal = 0;
    free(*pNum);
  }

  return iFinal;
}
