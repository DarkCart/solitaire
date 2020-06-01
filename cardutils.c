#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include <SDL/SDL.h>

#include "cardutils.h"

#define MAX_CARDS 52

char suits[4][10] = {"Clubs", "Hearts", "Spades", "Diamonds"};

const struct Card BLANK_CARD[] = {"", 0, false, false};

SDL_Color BLACK = {32, 32, 32};
SDL_Color RED = {255, 80, 80};

void initialize_deck(struct Card deck[]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			//printf("%s of %s\n", values[j], suits[i]);
			strcpy(deck[(i*13)+j].suit, suits[i]);
			deck[(i*13)+j].value = (j+1);
			deck[(i*13)+j].visible = true;
		}
	}
}

void shuffle_deck(struct Card deck[]) {
	int swapped_index = 0;
	int i = 0;
	struct Card temp = {"", 0, NULL, NULL};
	srand(time(NULL));
	for (i = 0; i < MAX_CARDS; i++) {
		swapped_index = rand() % 52;
		temp = deck[i];
		deck[i] = deck[swapped_index];
		deck[swapped_index] = temp;
	}
}

void initialize_stacks(struct Card shuffled_deck[], struct Card stacks[7][52]) {
	for (int k = 0; k < 7; k++) {
		for (int l = 0; l <= k; l++) {
			//printf("setting %d stack card %d to %s of %s\n", (k+1), (l+1), shuffled_deck[(k*7)+l].value, shuffled_deck[(k*7)+l].suit);
			stacks[k][l] = shuffled_deck[(k*7)+l];
			stacks[k][l].exists = true;
			stacks[k][l].visible = false;
		}
	}
}

void move_card(struct Card indeck[], int index1, struct Card outdeck[], int index2) {
	outdeck[index2] = indeck[index1];
	indeck[index1] = *BLANK_CARD;
}

int get_true_end(struct Card stack[]) {
	int count = 0;
	for (int i = 0; i < 52; i++) {
		if (!(stack[i].exists)) {
			break;
		}
		count++;
	}

	return count;
}

int get_difference(struct Card card1, struct Card card2) {
	return card1.value - card2.value;
}


SDL_Color getColor(char suit[10]) {
	if (strcmp(suit, "Clubs") == 0 || strcmp(suit, "Spades") == 0) {
		return BLACK;
	}
	return RED;
}

bool areCardsOpposite(struct Card card1, struct Card card2) {
	return !(getColor(card1.suit).r == getColor(card2.suit).r);
}

void move_stack(struct Card original[], int begin_index, struct Card dest[], int original_size) {
	//printf("O %d\n", get_true_end(original));
	int length = get_true_end(original);
	int new_index = original_size;
	for (int i = begin_index; i < length; i++) {
		//printf("%d\n", i);
		dest[new_index] = original[i];
		original[i] = *BLANK_CARD;
		dest[new_index].exists = true;
		new_index++;
	}
	//printf("E %d\n", original[7].value); 
	//printf("D %d\n", get_true_end(dest));
}

int get_visible_cards(struct Card stack[]) {
	int count = 0;
	for (int i = 0; i < 52; i++) {
		if (!(stack[i].visible)) {
			break;
		}
		count++;
	}
	return count;
}
