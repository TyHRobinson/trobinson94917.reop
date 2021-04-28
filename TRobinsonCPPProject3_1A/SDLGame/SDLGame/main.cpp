#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "bullet.h"
#include "enemy.h"


using namespace std;

Mix_Chunk* blaster, * explosion;
Mix_Music* music;

bool quit = false;
float deltaTime = 0.0, playerSpeed = 500.0f, yDir, pos_X, pos_Y;
int thisTime = 0, lastTime = 0, playerScore, oldScore, playerLives, oldLives;
vector<Enemy> enemyList;
vector<Bullet> bulletList;
SDL_Rect playerPos, scorePos, livesPos, backgroundPos;;
TTF_Font* font;
SDL_Color colorP1 = { 225, 225, 10 ,0 };
SDL_Surface* scoreSurface, * livesSurface;
SDL_Texture* scoreTexture, * livesTexture;
string tempScore, tempLives;

void CreateBullet()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active == false)
		{
			Mix_PlayChannel(-1, blaster, 0);
			bulletList[i].active = true;
			bulletList[i].posRect.y = (pos_Y + (playerPos.h / 2));
			bulletList[i].posRect.y = (bulletList[i].posRect.y - (bulletList[i].posRect.h / 2));
			bulletList[i].posRect.x = playerPos.x;
			bulletList[i].pos_Y = pos_Y;
			bulletList[i].pos_X = playerPos.x + 45;
			break;
		}
	}
}
void UpdateScore(SDL_Renderer* renderer)
{
	tempScore = "Player Score: " + std::to_string(playerScore);
	scoreSurface = TTF_RenderText_Solid(font, tempScore.c_str(), colorP1);
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	SDL_QueryTexture(scoreTexture, NULL, NULL, &scorePos.w, &scorePos.h);
	SDL_FreeSurface(scoreSurface);
	oldScore = playerScore;
}
void UpdateLives(SDL_Renderer* renderer)
{
	tempLives = "Player Lives: " + std::to_string(playerLives);
	livesSurface = TTF_RenderText_Solid(font, tempLives.c_str(), colorP1);
	livesTexture = SDL_CreateTextureFromSurface(renderer, livesSurface);
	SDL_QueryTexture(livesTexture, NULL, NULL, &livesPos.w, &livesPos.h);
	SDL_FreeSurface(livesSurface);
	oldLives = playerLives;
}


int main(int argc, char* argv[])
{
	SDL_Window* window;
	SDL_Renderer* renderer = NULL;
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow(
		"Space Game",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		1024,
		768,
		SDL_WINDOW_OPENGL
	);

	// A message if the windo errors out.
	if (window == NULL) {
		printf("Window could not be created: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface* surface = IMG_Load("./assets/background.png");
	SDL_Texture* background;
	background = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	backgroundPos.x = 0;
	backgroundPos.y = 0;
	backgroundPos.w = 1024;
	backgroundPos.h = 768;

	surface = IMG_Load("./assets/player.png");
	SDL_Texture* player;
	player = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	playerPos.x = 10;
	playerPos.y = 384;
	playerPos.w = 70;
	playerPos.h = 100;

	SDL_Event event;

	for (int i = 0; i < 10; i++)
	{
		Bullet tempBullet(renderer, i + 5, i + 5);
		bulletList.push_back(tempBullet);
	}
	enemyList.clear();
	for (int i = 0; i < 10; i++)
	{
		Enemy tempEnemy(renderer);
		enemyList.push_back(tempEnemy);
	}

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	blaster = Mix_LoadWAV("./assets/blaster.wav");
	explosion = Mix_LoadWAV("./assets/explosion.wav");
	music = Mix_LoadMUS("./assets/music.wav");
	if (!Mix_PlayingMusic())
		Mix_PlayMusic(music, -1);

	oldScore = 0;
	playerScore = 0;
	oldLives = 0;
	playerLives = 5;

	TTF_Init();
	font = TTF_OpenFont("./assets/Aero.ttf", 20);
	 
	scorePos.x = scorePos.y = 10;
	livesPos.x = 10;
	livesPos.y = 40;

	UpdateScore(renderer);
	UpdateLives(renderer);

	enum GameState { GAME, WIN, LOSE };

	GameState gameState = GAME;
	bool game, win, lose;

	while (!quit)
	{
		switch (gameState)
		{
		case GAME:
		{
			game = true;

			std::cout << "The Game State is GAME" << endl;
			std::cout << "The Game has started!" << endl;
			std::cout << endl;
			surface = IMG_Load("./assets/background.png");
			background = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
			enemyList.clear();
			for (int i = 0; i < 10; i++)
			{
				Enemy tempEnemy(renderer);
				enemyList.push_back(tempEnemy);
			}
			playerScore = 0;
			playerLives = 5;

			while (game)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				if (SDL_PollEvent(&event)) {
					if (event.type == SDL_QUIT) {
						quit = true;
						game = false;
						break;
					}

					switch (event.type)
					{
					case SDL_KEYUP:
						switch (event.key.keysym.sym)
						{
						case SDLK_SPACE:
							CreateBullet();
							break;
						default:
							break;
						}
					}
				}
				const Uint8* currentKeyState = SDL_GetKeyboardState(NULL);
				if (currentKeyState[SDL_SCANCODE_UP])
					yDir = -1;
				else if (currentKeyState[SDL_SCANCODE_DOWN])
					yDir = 1;
				else
					yDir = 0;

				pos_Y += (playerSpeed * yDir) * deltaTime;
				playerPos.y = (int)(pos_Y + 0.5f);
				if (playerPos.y < 0)
				{
					playerPos.y = 0;
					pos_Y = 0;
				}
				if (playerPos.y > 768 - playerPos.h)
				{
					playerPos.y = 768 - playerPos.h;
					pos_Y = 768 - playerPos.h;
				}
				for (int i = 0; i < bulletList.size(); i++)
				{
					if (bulletList[i].active == true)
					{
						bulletList[i].Update(deltaTime);
					}
				}

				for (int i = 0; i < 10; i++)
				{
					enemyList[i].Update(deltaTime);
				}

				for (int i = 0; i < bulletList.size(); i++)
				{
					if (bulletList[i].active == true)
					{
						for (int j = 0; j < enemyList.size(); j++)
						{
							if (SDL_HasIntersection(&bulletList[i].posRect, &enemyList[j].posRect))
							{
								Mix_PlayChannel(-1, explosion, 0);
								enemyList[j].Reset();
								bulletList[i].Reset();
								playerScore += 10;
								if (playerScore >= 500)
								{
									game = false;
									gameState = WIN;
								}

							}
						}
					}
				}
				for (int i = 0; i < enemyList.size(); i++)
				{
					if (SDL_HasIntersection(&playerPos, &enemyList[i].posRect))
					{
						Mix_PlayChannel(-1, explosion, 0);
						enemyList[i].Reset();
						playerLives -= 1;
						if (playerLives <= 0)
						{
							game = false;
							gameState = LOSE;
						}
					}
				}
				if (playerScore != oldScore)
				{
					UpdateScore(renderer);
				}
				if (playerLives != oldLives)
				{
					UpdateLives(renderer);
				}


				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, background, NULL, &backgroundPos);
				for (int i = 0; i < bulletList.size(); i++)
				{
					if (bulletList[i].active == true)
					{
						bulletList[i].Draw(renderer);
					}
				}
				SDL_RenderCopy(renderer, player, NULL, &playerPos);
				for (int i = 0; i < 10; i++)
				{
					enemyList[i].Draw(renderer);
				}
				SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePos);
				SDL_RenderCopy(renderer, livesTexture, NULL, &livesPos);

				SDL_RenderPresent(renderer);
			}
			break;
		}
		case WIN:
			win = true;

			std::cout << "The Game State is WIN" << endl;
			std::cout << "The R key to Replay Game" << endl;
			std::cout << "The Q key to Quit Game" << endl;
			std::cout << endl;
			surface = IMG_Load("./assets/winscreen.png");
			background = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);

			while (win)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;
				if (SDL_PollEvent(&event))
				{
					if (event.type == SDL_QUIT)
					{
						quit = true;
						win = false;
						break;
					}
					switch (event.type)
					{
					case SDL_KEYUP:
						switch (event.key.keysym.sym)
						{
						case SDLK_r:
							win = false;
							gameState = GAME;
							break;
						case SDLK_q:
							win = false;
							quit = true;
							break;
						default:
							break;
						}
					}
				}
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, background, NULL, &backgroundPos);
				SDL_RenderPresent(renderer);
			}
			break;
		case LOSE:
			lose = true;

			std::cout << "The Game State is LOSE" << endl;
			std::cout << "The R key to Replay Game" << endl;
			std::cout << "The Q key to Quit Game" << endl;
			std::cout << endl;
			surface = IMG_Load("./assets/losescreen.png");
			background = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);

			while (lose)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;
				if (SDL_PollEvent(&event))
				{
					if (event.type == SDL_QUIT)
					{
						quit = true;
						lose = false;
						break;
					}
					switch (event.type)
					{
					case SDL_KEYUP:
						switch (event.key.keysym.sym)
						{
						case SDLK_r:
							lose = false;
							gameState = GAME;
							break;
						case SDLK_q:
							lose = false;
							quit = true;
							break;
						default:
							break;
						}
					}
				}
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, background, NULL, &backgroundPos);
				SDL_RenderPresent(renderer);

			}
			break;
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
