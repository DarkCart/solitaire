#include <SDL/SDL.h>

#define MAX_CARDS 52

struct Card {
	char suit[10];
	int value;
	bool visible;
	bool exists;
};

void shuffle_deck(struct Card deck[]);

void initialize_deck();

void initialize_stacks();

void move_card(struct Card original[], int original_index, struct Card newdeck[], int newindex);

int get_true_end(struct Card stack[]);

int get_difference(struct Card card1, struct Card card2);

SDL_Color getColor(char suit[10]);

bool areCardsOpposite(struct Card card1, struct Card card2);
