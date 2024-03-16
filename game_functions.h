#include <stdio.h>
#include <stdlib.h>
#include "pd_api.h"

#define DISPLAY_HEIGHT 240
#define DISPLAY_WIDTH 400
#define SCALE 16

#define RANDOM_X_CORDINATE rand() % (DISPLAY_HEIGHT / SCALE);
#define RANDOM_Y_CORDINATE rand() % (DISPLAY_WIDTH / SCALE);

typedef struct Apple {
	uint16_t x;
	uint16_t y;
} Apple;

typedef struct Snake {
	uint8_t x[100];
	uint8_t y[100];
	uint8_t len;
} Snake;

void setPDPtr(PlaydateAPI* p);

int update(void* ud);

void setupGame();

void newApple();

uint8_t isSnake(uint16_t x, uint16_t y);

uint8_t isApple(uint16_t x, uint16_t y);

void grow(uint8_t x, uint8_t y);

void moveUp();

void moveDown();

void moveLeft();

void moveRight();

