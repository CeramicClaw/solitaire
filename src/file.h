#include <stdbool.h>
#include "deck.h"

bool parseFile(char* pFile, char** pInput, char** pKey);
void cleanInput(char* pInput);
size_t cleanAlphaKey(char* pKey, int** pNum);
size_t cleanDeckKey(char* pKey, int** pNum);
