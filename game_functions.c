/**
 * @file main.c
 * @brief Implementation of a simple Snake game for the Playdate handheld console.
 * @author Noah Törnqvist
 */

#include <stdio.h>
#include <stdlib.h>
#include "pd_api.h"
#include "game_functions.h"

/** @brief Pointer to the Playdate API instance */
static PlaydateAPI* pd = NULL;

/** @brief Speed of the game */
int speed = 1000000;
/** @brief Loop variable */
int loop;

/** @brief Set the pointer to the PlaydateAPI instance
 *  @param p Pointer to the PlaydateAPI instance
 */
void setPDPtr(PlaydateAPI* p) {
    pd = p;
}

/** @brief Enum representing different states of the game */
typedef enum {
    MENU,        /**< Menu state */
    START,       /**< Start state */
    MOVE_UP,     /**< Move up state */
    MOVE_DOWN,   /**< Move down state */
    MOVE_LEFT,   /**< Move left state */
    MOVE_RIGHT,  /**< Move right state */
    GAME_OVER    /**< Game over state */
} States;

/** @brief Current state of the game */
static States state, nextState;

/** @brief Structure representing the snake */
struct Snake snake = {{0}, {0}, 1};

/** @brief Structure representing the apple */
struct Apple apple = {0, 0};

/** @brief Check for collision
 *  @return 1 if collision occurs, otherwise 0
 */
uint8_t isCollision() {
    if ((snake.x[0] < 0 || snake.x[0] > (DISPLAY_WIDTH / SCALE)) ||
        (snake.y[0] < 0 || snake.y[0] > (DISPLAY_HEIGHT / SCALE))) {
        return 1;
    }
    return 0;
}

/** @brief Setup the game */
void setupGame() {
    snake.x[0] = DISPLAY_WIDTH / SCALE / 2;
    snake.y[0] = DISPLAY_HEIGHT / SCALE / 2;
    snake.len = 1;
    nextState = MENU;
    newApple();
}

/** @brief Generate a new apple */
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

/** @brief Check if the given coordinates are for an apple
 *  @param x X coordinate
 *  @param y Y coordinate
 *  @return 1 if the coordinates match the apple's position, otherwise 0
 */
uint8_t isApple(uint16_t x, uint16_t y) {
    if (apple.x == x && apple.y == y) {
        return 1;
    }
    return 0;
}

/** @brief Check if the given coordinates are part of the snake
 *  @param x X coordinate
 *  @param y Y coordinate
 *  @return 1 if the coordinates match the snake's position, otherwise 0
 */
uint8_t isSnake(uint16_t x, uint16_t y) {
    for (uint16_t i = 0; i < snake.len; i++) {
        if (snake.x[i] == x && snake.y[i] == y) {
            return 1;
        }
    }
    return 0;
}

/** @brief Check if the given coordinates are part of the snake
 *  @param x X coordinate
 *  @param y Y coordinate
 *  @return 1 if the coordinates match the snake's position, otherwise 0
 */
void drawSnake() {

    for (uint16_t i = 0; i < snake.len; i++)
    {	
        pd->graphics->fillRect(snake.x[i] * SCALE, snake.y[i] * SCALE, SCALE, SCALE, kColorBlack);
    }
}

/** @brief Draw the apple on the screen */
void drawApple() {
    pd->graphics->fillEllipse(apple.x * SCALE, apple.y * SCALE, SCALE, SCALE, 0.0f, 0.0f, kColorBlack);
}

/** @brief Grow the snake by one segment
 *  @param x X coordinate of the new segment
 *  @param y Y coordinate of the new segment
 */
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

/** @brief Shift the snake's body segments */
void shiftSnake() {
    for (uint8_t i = snake.len; i > 0; i--)
    {
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }
}

/** @brief Move the snake upwards */
void moveUp() {
    shiftSnake();
    snake.y[0] -= 1;
}

/** @brief Move the snake downwards */
void moveDown() {
    shiftSnake();
    snake.y[0] += 1;
}

/** @brief Move the snake leftwards */
void moveLeft() {
    shiftSnake();
    snake.x[0] -= 1;
}

/** @brief Move the snake rightwards */
void moveRight() {
    shiftSnake();
    snake.x[0] += 1;
}

/** @brief Update the game logic
 *  @param ud User data (unused)
 *  @return 1 if the update is successful
 */
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
