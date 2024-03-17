# Snake

## Description
This snake game is developed for the handheld video game console Playdate. The code is written in C and uses the Playdate APIs. 

The game is played using the directional buttons for controlling the snake. A new apple will be spawned on a random place in the begining and after it has been consumed by the snake. When the snakes head is in contact with an apple is will be consumed and the snake will grow. The main goal
of the game is to grow the snake as large as possible. The game will finish when the snake cant grow any bigger or if the snake is crossing its own body. 

The game consist of two c files, main.c and game_functions.c. The main.c file will initilize the game. The game_functions.c file includes all the necessary functions for the game to work and the update function which is the one that will be called in the beginning and uses a state machine.

## Getting Started
### Dependencies

- Playdate SDK
- A Playdate or the Playdate simulator 

## Authors
Noah Törnqvist
