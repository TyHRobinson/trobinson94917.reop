#include "bullet.h"

Bullet::Bullet(SDL_Renderer* renderer, float x, float y)
{
	int w, h;
	active = false;
	speed = 800.0;
	SDL_Surface* surface = IMG_Load("./assets/blaster.png");
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL)
		printf("Unable to load image %s! SDL_image Error: %s\n", ".assets/bullet.png", IMG_GetError());
	SDL_FreeSurface(surface);	
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;
	posRect.h = h;
	posRect.x = x;
	posRect.y = y - (posRect.h / 2);;
	pos_X = x;
	pos_Y = y;
	xDir = 1;
	yDir = 0;
}
void Bullet::Reset()
{
	active = false;
	posRect.x - 1000;
	posRect.y - 1000;
	pos_X = posRect.x;
	pos_Y = posRect.y;
}
void Bullet::Update(float deltaTime)
{
	if (active)
	{
		pos_X += (speed * xDir) * deltaTime;
		posRect.x = (int)(pos_X + 0.5f);
		if (posRect.x > (1024 - posRect.h))
		{
			posRect.y = -1000;
			pos_Y = posRect.y;
			active = false;
		}
	}
}
void Bullet::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}
Bullet::~Bullet()
{
	//SDL_DestroyTexture(texture);
}