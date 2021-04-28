#include "enemy.h"

Enemy::Enemy(SDL_Renderer* renderer) {
	int w, h;
	SDL_Surface* surface = IMG_Load("./assets/enemy.png");
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL)
		printf("Unable to load image %s! SDL_image Error: %s\n", ".assets/enemy.png", IMG_GetError());
	SDL_FreeSurface(surface);	
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;
	posRect.h = h;
	Reset();
	xDir = 1;
	yDir = 0;
}

void Enemy::Reset()
{
	speed = rand() % (5) + 1;
	speed *= 100;
	posRect.x = 1024 - posRect.w;
	posRect.y = rand() % (768 - posRect.h) + 1;
	pos_X = posRect.x;
	pos_Y = posRect.y;
}

void Enemy::Update(float deltaTime)
{
	pos_X -= (speed * xDir) * deltaTime;
	posRect.x = (int)(pos_X + 0.5f);
	if (posRect.x < 0)
		Reset();
}

void Enemy::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

Enemy::~Enemy()
{
	//SDL_DestroyTexture(texture);
}