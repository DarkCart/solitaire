#include <SDL/SDL.h>

#define MAX_CARDS 52

struct Card {
	char suit[10];
	int value;
	bool visible;
	bool exists;
	int renderX;
	int renderY;
	int renderHeight;
};

void shuffle_deck(struct Card deck[]);

void initialize_deck();

void initialize_stacks();

void move_card(struct Card original[], int original_index, struct Card newdeck[], int newindex);

void move_stack(struct Card original[], int begin_index, struct Card dest[], int original_size); 

int get_true_end(struct Card stack[]);

int get_visible_cards(struct Card stack[]);

int get_difference(struct Card card1, struct Card card2);

SDL_Color getColor(char suit[10]);

bool areCardsOpposite(struct Card card1, struct Card card2);
