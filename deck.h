#include <stdlib.h>
#include "cards.h"

struct deck_tag {
	  card_t ** cards;
	    size_t n_cards;
};
typedef struct deck_tag deck_t;
