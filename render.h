void initRenderer(SDL_Renderer* r);
void render();
void renderCard(struct Card card, int x, int y);
void renderCardStack(struct Card stack[52], int originalSize, int beginX, int beginY, int spacing);
void renderPlayingField();
void destroyRenderer();
