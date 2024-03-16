#include <stdio.h>
#include <stdlib.h>
#include "pd_api.h"
#include "game_functions.h"

static PlaydateAPI* pd = NULL;

int speed = 1000000;
int loop;

void setPDPtr(PlaydateAPI* p) {
	pd = p;
}

typedef enum {
	MENU,
	START,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	GAME_OVER
} States;

static States state, nextState;

struct Snake snake = {{0}, {0}, 1};

struct Apple apple = {0, 0};

PDButtons current;
PDButtons pushed;
PDButtons released;

uint8_t isCollision() {

	if (
		(snake.x[0] < 0 || snake.x[0] > (DISPLAY_WIDTH / SCALE)) ||
		(snake.y[0] < 0 || snake.y[0] > (DISPLAY_HEIGHT / SCALE))
		)
	{
		return 1;
	}

	return 0;
}

void setupGame() {

	snake.x[0] = DISPLAY_WIDTH / SCALE / 2;
	snake.y[0] = DISPLAY_HEIGHT / SCALE / 2;
	snake.len = 1;
	nextState = MENU;
	newApple();

}

void newApple() {
	uint16_t x;
	uint16_t y;
	do {
		x = pd->system->getCurrentTimeMilliseconds() % (DISPLAY_WIDTH / SCALE);
		y = pd->system->getCurrentTimeMilliseconds() % (DISPLAY_HEIGHT / SCALE);
	} while (isSnake(x, y));
	apple.x = x;
	apple.y = y;
}

uint8_t isApple(uint16_t x, uint16_t y) {
	
	if (apple.x == x && apple.y == y)
	{
		return 1;
	}
	return 0;
}

uint8_t isSnake(uint16_t x, uint16_t y) {
	
	for (uint16_t i = 0; i < snake.len; i++)
	{
		if (snake.x[i] == x && snake.y[i] == y)
		{
			return 1;
		}
	}
	return 0;

}

void drawSnake() {

	for (uint16_t i = 0; i < snake.len; i++)
	{	
		pd->graphics->fillRect(snake.x[i] * SCALE, snake.y[i] * SCALE, SCALE, SCALE, kColorBlack);
	}
}


void drawApple() {
	//pd->graphics->fillRect(apple.x * SCALE, apple.y * SCALE, SCALE, SCALE, kColorBlack);
	pd->graphics->fillEllipse(apple.x * SCALE, apple.y * SCALE, SCALE, SCALE, 0.0f, 0.0f, kColorBlack);
}

void grow(uint8_t x, uint8_t y) {
	for (int i = snake.len; i >= 0; i--)
	{
		snake.x[i + 1] = snake.x[i];
		snake.y[i + 1] = snake.y[i];
	}
	snake.x[0] = x;
	snake.y[0] = y;
	snake.len += 1;
	speed *= 0.9;
}

void shiftSnake() {
	for (uint8_t i = snake.len; i > 0; i--)
	{
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}
}

void moveUp() {

	shiftSnake();
	snake.y[0] -= 1;

}

void moveDown() {
	shiftSnake();
	snake.y[0] += 1;
}

void moveLeft() {
	shiftSnake();
	snake.x[0] -= 1;
}

void moveRight() {
	shiftSnake();
	snake.x[0] += 1;
}

int update(void* ud)
{

	state = nextState;

	switch (state)
	{
		case MENU:
			pd->system->getButtonState(&current, &pushed, &released);
			if (pushed & kButtonA) {
				setupGame();
				nextState = START;
			}
			break;
		case START:
			speed = 500000;
			nextState = MOVE_RIGHT;
			break;
		case MOVE_UP:
			loop = speed;
			while (loop != 0) {
				pd->system->getButtonState(&current, &pushed, &released);
				if (pushed & kButtonRight) {
					nextState = MOVE_RIGHT;
				}
				if (pushed & kButtonLeft) {
					nextState = MOVE_LEFT;
				}
				loop--;
			}
			
			if (isApple(snake.x[0], snake.y[0] - 1))
			{
				grow(apple.x, apple.y);
				newApple();
			}
			else {
				moveUp();
				if (isCollision())
				{
					nextState = GAME_OVER;
				}
			}
			

			break;

		case MOVE_DOWN:
			loop = speed;
			while (loop != 0) {
				pd->system->getButtonState(&current, &pushed, &released);

				if (pushed & kButtonRight) {
					nextState = MOVE_RIGHT;
				}
				if (pushed & kButtonLeft) {
					nextState = MOVE_LEFT;
				}
				loop--;
			}
			
			if (isApple(snake.x[0], snake.y[0] + 1))
			{
				grow(apple.x, apple.y);
			}
			else {
				moveDown();
				if (isCollision())
				{
					nextState = GAME_OVER;
				}
			}
			
			break;


		case MOVE_LEFT:

			loop = speed;
			while (loop != 0) {
				pd->system->getButtonState(&current, &pushed, &released);

				if (pushed & kButtonUp) {
					nextState = MOVE_UP;
				}
				if (pushed & kButtonDown) {
					nextState = MOVE_DOWN;
				}
				loop--;
			}
			if (isApple(snake.x[0] - 1, snake.y[0]))
			{
				grow(apple.x, apple.y);
			}
			else {
				moveLeft();
				if (isCollision())
				{
					nextState = GAME_OVER;
				}
			}
			
			break;

		case MOVE_RIGHT:

			loop = speed;
			while (loop != 0) {
				pd->system->getButtonState(&current, &pushed, &released);

				if (pushed & kButtonUp) {
					nextState = MOVE_UP;
				}
				if (pushed & kButtonDown) {
					nextState = MOVE_DOWN;
				}
				loop--;
			}
			if (isApple(snake.x[0] + 1, snake.y[0]))
			{
				grow(apple.x, apple.y);
			}
			else {
				moveRight();
				if (isCollision())
				{
					nextState = GAME_OVER;
				}
			}
			
			break;

		case GAME_OVER:
			nextState = MENU;
			break;

		default:
			break;
	}
	pd->graphics->clear(kColorWhite);
	drawApple();
	drawSnake();
	
	return 1;
}






