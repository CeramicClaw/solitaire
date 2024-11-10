#include <stdlib.h>
#include "cards.h"

struct deck_tag
{
	card_t** cards;
	size_t   nCards;
};
typedef struct deck_tag deck_t;

deck_t* makeDeck();
void freeDeck(deck_t* pDeck);
