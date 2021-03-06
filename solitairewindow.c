#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#include "cardutils.h"

void render();
void renderCard(struct Card card, int x, int y);
void renderCardStack(struct Card stack[52], int originalSize, int beginX, int beginY, int spacing);
void renderPlayingField();

void handleMouseInput();
void handleKeyboardInput();

bool intersects(SDL_Rect rect1, SDL_Rect rect2);

SDL_Surface* screenSurface = NULL;
SDL_Renderer* renderer = NULL;
SDL_Rect temp;

SDL_Rect mouse = {0, 0, 32, 32};

TTF_Font* small_font = NULL;
TTF_Font* big_font = NULL;

SDL_Color YELLOW = {255, 255, 128};

struct Card deck[MAX_CARDS];
struct Card stacks[7][52];
struct Card pile[4][13];

struct Card deckDraw = {"", 0, false, true};

bool needsRender = true;

int mouseX, mouseY;

int main() {
	initialize_deck(deck);
	shuffle_deck(deck);
	initialize_stacks(deck, stacks);
	
	SDL_Window* window = NULL;
	SDL_Event e;
	bool close = false;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	window = SDL_CreateWindow("Solitaire", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	screenSurface = SDL_GetWindowSurface(window);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	small_font = TTF_OpenFont("arial.ttf", 24);
	big_font = TTF_OpenFont("arial.ttf", 72);

	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 50, 255, 90));
	
	move_card(stacks[6], 6, stacks[5], get_true_end(stacks[5]));
	move_card(stacks[6], 5, stacks[5], get_true_end(stacks[5]));
	move_card(stacks[6], 4, stacks[5], get_true_end(stacks[5]));
	
	//printf("%d\n", get_true_end(stacks[5]));
	//printf("%d\n", areCardsOpposite(stacks[5][5], stacks[5][6]));
	//printf("%d\n", get_difference(stacks[5][5], stacks[5][6]));

	while (!close) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				close = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
				SDL_GetMouseState(&mouseX, &mouseY);
				handleMouseInput();
			}
			if (e.type == SDL_KEYDOWN) {
				handleKeyboardInput();
			}
		}
		if (needsRender) {
			render();
		}
		SDL_UpdateWindowSurface(window);
	}
	//SDL_Delay(2000);
	SDL_DestroyWindow(window);

	TTF_CloseFont(small_font);
	TTF_CloseFont(big_font);
	TTF_Quit();
	
	SDL_Quit();

	return 0;
}

void render() {
	SDL_SetRenderDrawColor(renderer, 50, 255, 90, 0);
	SDL_RenderClear(renderer);
	renderPlayingField();
	int xIndex = 50;
	for (int i = 0; i < 7; i++) {
		renderCardStack(stacks[i], (i+1), xIndex, 100, 30);
		xIndex += 100;
	}
	SDL_RenderPresent(renderer);
	needsRender = false;
}

void renderCardStack(struct Card stack[52], int originalSize, int beginX, int beginY, int spacing) {
	int yIndex = beginY;
	//printf("%d\n", originalSize);
	for (int i = 0; i < get_true_end(stack); i++) {
		stack[i].renderX = beginX;
		stack[i].renderY = beginY;
		
		stack[originalSize-1].visible = true; // display the end card if the stack size hasn't been changed
		stack[get_true_end(stack)-1].visible = true; // display the end card if the stack size has been changed
		if (i > originalSize - 1 && i < get_true_end(stack)) {
			stack[i].visible = true; // display all cards that are larger than the original size of the stack but smaller than the end of the stack
		}
		if (i == (get_true_end(stack) - 1)) {
				stack[i].renderHeight = 128;
		} else {
				stack[i].renderHeight = 30;
		}
		renderCard(stack[i], beginX, beginY+yIndex);
		yIndex += spacing;
	}
}

void renderCard(struct Card card, int x, int y) {
	SDL_Rect topRect;
	SDL_Rect middleRect;
	SDL_Rect bottomRect;

	SDL_Texture* topTexture = NULL;
	SDL_Texture* middleTexture = NULL;
	SDL_Texture* bottomTexture;

	SDL_Surface* topSurface = NULL;
	SDL_Surface* middleSurface = NULL;
	SDL_Surface* bottomSurface;
	
	char renderSuit[13];
	
	temp.x = x;
	temp.y = y;
	temp.w = 96;
	temp.h = 128;
	
	if (card.visible) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	} else {
		SDL_SetRenderDrawColor(renderer, 128, 128, 255, 255);
	}
	SDL_RenderFillRect(renderer, &temp);
	
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &temp);
	
	if (card.visible) {
	switch (card.value) {
		case 1:
			strcpy(renderSuit, "A");
			break;
		case 11:
			strcpy(renderSuit, "J");
			break;
		case 12:
			strcpy(renderSuit, "Q");
			break;
		case 13:
			strcpy(renderSuit, "K");
			break;
		default:
			sprintf(renderSuit, "%d", card.value);
	}
	
	if (strcmp(card.suit, "Clubs") == 0) {
		topSurface = TTF_RenderUTF8_Solid(small_font, strcat(renderSuit, "\u2663"), getColor(card.suit));
		middleSurface = TTF_RenderUTF8_Solid(big_font, "\u2663", getColor(card.suit));
	} else if (strcmp(card.suit, "Spades") == 0) {
		topSurface = TTF_RenderUTF8_Solid(small_font, strcat(renderSuit, "\u2660"), getColor(card.suit));
		middleSurface = TTF_RenderUTF8_Solid(big_font, "\u2660", getColor(card.suit));		
	} else if (strcmp(card.suit, "Hearts") == 0) {
		topSurface = TTF_RenderUTF8_Solid(small_font, strcat(renderSuit, "\u2665"), getColor(card.suit));
		middleSurface = TTF_RenderUTF8_Solid(big_font, "\u2665", getColor(card.suit));		
	} else if (strcmp(card.suit, "Diamonds") == 0) {
		topSurface = TTF_RenderUTF8_Solid(small_font, strcat(renderSuit, "\u2666"), getColor(card.suit));
		middleSurface = TTF_RenderUTF8_Solid(big_font, "\u2666", getColor(card.suit));		
	}
	}
	
	if (!card.visible) {
		topSurface = TTF_RenderUTF8_Solid(small_font, "*", YELLOW);
		middleSurface = TTF_RenderUTF8_Solid(big_font, "*", YELLOW);
	}
	
	
	topTexture = SDL_CreateTextureFromSurface(renderer, topSurface);
	middleTexture = SDL_CreateTextureFromSurface(renderer, middleSurface);
	
	
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(topTexture, NULL, NULL, &texW, &texH);
	topRect.x = x + 2;
	topRect.y = y + 2;
	topRect.w = texW;
	topRect.h = texH;
	bottomRect.x = x+60;
	bottomRect.y = y+100;
	bottomRect.w = texW;
	bottomRect.h = texH;
	
	SDL_RenderCopy(renderer, topTexture, NULL, &topRect);
	SDL_RenderCopyEx(renderer, topTexture, NULL, &bottomRect, 0, NULL, SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);
	
	SDL_QueryTexture(middleTexture, NULL, NULL, &texW, &texH);
	middleRect.x = x + 24;
	middleRect.y = y + 24;
	middleRect.w = texW;
	middleRect.h = texH;
	
	SDL_RenderCopy(renderer, middleTexture, NULL, &middleRect);
	
	SDL_DestroyTexture(topTexture);
	SDL_DestroyTexture(middleTexture);
	SDL_FreeSurface(topSurface);
	SDL_FreeSurface(middleSurface);
}

void renderPlayingField() {
	SDL_Rect pileRect;
	SDL_Rect deckRect;
	
	deckRect.x = 50;
	deckRect.y = 30;
	deckRect.w = 96;
	deckRect.h = 128;
	
	SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
	SDL_RenderFillRect(renderer, &deckRect);
	renderCard(deckDraw, 50, 30);
	
	for (int i = 0; i < 4; i++) {
			pileRect.x = 350 + (i*100);
			pileRect.y = 30;
			pileRect.w = 96;
			pileRect.h = 128;
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &pileRect);
	}
	
	SDL_RenderPresent(renderer);
}

void handleMouseInput() {
	//SDL_Rect one = {100, 100, 32, 32};
	//SDL_Rect two = {100, 110, 32, 32};
	//printf("%d\n", intersects(one, two));
	
	mouse.x = mouseX;
	mouse.y = mouseY;
	/*
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < get_true_end(stacks[i]); j++) {
			temp.x = stacks[i][j].renderX;
			temp.y = stacks[i][j].renderY;
			temp.w = 96;
			temp.h = stacks[i][j].renderHeight;
			printf("%d\n", intersects(mouse, temp));
		}
	}
	*/
	temp.x = stacks[1][1].renderX;
	temp.y = stacks[1][1].renderY;
	temp.w = 96;
	temp.h = stacks[1][0].renderHeight;
	printf("C %d, %d, %d, %d\n", temp.x, temp.y, temp.w, temp.h);
	printf("M %d, %d, %d, %d\n", mouse.x, mouse.y, mouse.w, mouse.h);
	printf("I %d\n", intersects(mouse, temp));
}

void handleKeyboardInput() {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_RETURN]) {
		
		move_stack(stacks[5], 5, stacks[0], 1);
		needsRender = true;
		printf("<RETURN> is pressed.\n");
	}

}

bool intersects(SDL_Rect rect1, SDL_Rect rect2) {
	return rect2.w > 0 && rect2.h > 0 && rect1.w > 0 && rect1.h > 0 && rect2.x < rect1.x + rect1.w && rect2.x + rect2.w > rect1.x && rect2.y < rect1.y + rect1.h && rect2.y + rect2.h > rect1.y;
}
