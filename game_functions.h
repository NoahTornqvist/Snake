/**
 * @file game_functions.h
 * @brief Header file containing declarations of functions used in the Snake game.
 * @author Noah Törnqvist
 */

#ifndef GAME_FUNCTIONS_H_
#define GAME_FUNCTIONS_H_

#include "pd_api.h"

/** @brief Height of the display */
#define DISPLAY_HEIGHT 240
/** @brief Width of the display */
#define DISPLAY_WIDTH 400
/** @brief Scale factor for the game */
#define SCALE 16

/** @brief Macro to generate random x-coordinate for an apple */
#define RANDOM_X_COORDINATE rand() % (DISPLAY_HEIGHT / SCALE)
/** @brief Macro to generate random y-coordinate for an apple */
#define RANDOM_Y_COORDINATE rand() % (DISPLAY_WIDTH / SCALE)

/** @brief Structure representing an apple */
typedef struct Apple {
    uint16_t x; /**< X-coordinate of the apple */
    uint16_t y; /**< Y-coordinate of the apple */
} Apple;

/** @brief Structure representing a snake */
typedef struct Snake {
    uint8_t x[100]; /**< X-coordinates of the snake segments */
    uint8_t y[100]; /**< Y-coordinates of the snake segments */
    uint8_t len;    /**< Length of the snake */
} Snake;

/** @brief Set the pointer to the PlaydateAPI instance
 *  @param p Pointer to the PlaydateAPI instance
 */
void setPDPtr(PlaydateAPI* p);

/** @brief Update function for the game
 *  @param ud User data (unused)
 *  @return 1 if update is successful
 */
int update(void* ud);

/** @brief Setup the game */
void setupGame();

/** @brief Generate a new apple */
void newApple();

/** @brief Check if the given coordinates are part of the snake
 *  @param x X-coordinate to check
 *  @param y Y-coordinate to check
 *  @return 1 if the coordinates are part of the snake, 0 otherwise
 */
uint8_t isSnake(uint16_t x, uint16_t y);

/** @brief Check if the given coordinates match the apple's position
 *  @param x X-coordinate to check
 *  @param y Y-coordinate to check
 *  @return 1 if the coordinates match the apple's position, 0 otherwise
 */
uint8_t isApple(uint16_t x, uint16_t y);

/** @brief Increase the length of the snake and move it to the given position
 *  @param x X-coordinate to move the snake to
 *  @param y Y-coordinate to move the snake to
 */
void grow(uint8_t x, uint8_t y);

/** @brief Move the snake upwards */
void moveUp();

/** @brief Move the snake downwards */
void moveDown();

/** @brief Move the snake leftwards */
void moveLeft();

/** @brief Move the snake rightwards */
void moveRight();

#endif /* GAME_FUNCTIONS_H_ */
