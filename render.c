#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>

SDL_Renderer* renderer = NULL;
SDL_Rect temp;

TTF_Font* small_font = NULL;
TTF_Font* big_font = NULL;

void initRenderer(SDL_Renderer* r) {
	small_font = TTF_OpenFont("arial.ttf", 24);
	big_font = TTF_OpenFont("arial.ttf", 72);
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
	printf("%d\n", originalSize);
	for (int i = 0; i < get_true_end(stack); i++) {
		stack[originalSize-1].visible = true; // display the end card if the stack size hasn't been changed
		stack[get_true_end(stack)-1].visible = true; // display the end card if the stack size has been changed
		if (i > originalSize - 1 && i < get_true_end(stack)) {
			stack[i].visible = true; // display all cards that are larger than the original size of the stack but smaller than the end of the stack
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
		SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
	}
	SDL_RenderFillRect(renderer, &temp);
	
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &temp);
	
	if (!card.visible) {
		return;
	}
	
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

void destroyRenderer() {
	TTF_CloseFont(small_font);
	TTF_CloseFont(big_font);
}
