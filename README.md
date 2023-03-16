# Raiden

## Table of content

-   [Group members](#group-members)
-   [Gameplay](#gameplay)
-   [Program Feature](#program-feature)
    -   [1. Generation of random game elements](#1-generation-of-random-game-elements)
    -   [2. Data structures for storing game status](#2-data-structures-for-storing-game-status)
    -   [3. Dynamic Memory Management](#3-dynamic-memory-management)
    -   [4. File input/output (e.g., for loading/saving game status)](#4-file-inputoutput-eg-for-loadingsaving-game-status)
    -   [5. Program codes in multiple files](#5-program-codes-in-multiple-files)
    -   [6. External Libraries](#6-external-libraries)
-   [Visual Examples](#visual-examples)
-   [How to run the game](#how-to-run-the-game)

## Group members

1. [pvpswaghd](https://github.com/pvpswaghd)
2. [terry3041](https://github.com/terry3041)

## Gameplay

Raiden is a vertically scrolling shoot 'em up game with progressive difficulty. The player controls the Fighting Thunder aircraft, battling and evading various opponents. The player can shoot special projectiles that damage enemies while moving to avoid enemy attacks. The difficulty gradually increases by increasing the enemy's HP.

Random power-up goods would be distributed around the field to boost the player's performance. Collectables include weapon power-ups, healing items, and score-increasing awards. If all of a player's lives are lost during a game session, the game is over.

We use the traditional gaming controls, which are WASD, to control the movement of the ship. Arrow keys are also supported.

This is a modification of the original game Raiden, released in 1990.  
[Raiden Wikipedia](<https://en.wikipedia.org/wiki/Raiden_(video_game)>)

## Program Feature

### 1. Generation of random game elements

-   Enemy ships are generated randomly in the field
    -   `spawnEnemyShip()` function
-   Power up elements are generated randomly
    -   `spawnPowerUp()` function

### 2. Data structures for storing game status

-   `int`
-   `char` & `char` pointers
-   `string`
-   `std::vector`
-   `std::pair`
-   `struct`

### 3. Dynamic Memory Management

-   Enemy ships are stored in a dynamic array (`std::vector`) and removed once they are below the HP level.
    -   `hitDetection()` function in game.cpp
-   Bullets are removed once they reach the border of the game or hit the ships.
    -   `printBullets()` function in game.cpp

### 4. File input/output (e.g., for loading/saving game status)

-   `.txt` files are used to store the design of the player and enemy ship and the highest score attained in a local record.

### 5. Program codes in multiple files

-   `main.cpp` file is the main instruction program to execute all the functions.
-   `game.cpp` file is the main file to write all the functions dedicated to the game, for instance, UI and controls.
-   `menu.cpp` file is the main file to write the functions related to the game menus.
-   `Makefile` is used to link all .cpp files and link them together.

### 6. External Libraries

-   `ncurses` is used for better printing of game elements.

## Visual Examples

| Menu                                                                                                                          | Gameplay                                                                                                                      | Gameover                                                                                                                      |
| ----------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------- |
| <img width="413" src="https://user-images.githubusercontent.com/19218518/166943056-93b21857-2920-49d6-9930-a63c4aa7e2da.png"> | <img width="400" src="https://user-images.githubusercontent.com/19218518/166943115-08f5919e-2b42-4ef8-91e2-cd198be7c92c.png"> | <img width="400" src="https://user-images.githubusercontent.com/19218518/166943136-59ce62e8-b2cd-4307-b4ca-9cf080cb8f1f.png"> |

## How to run the game

Type `make && ./game` to compile and execute the game. If you already compiled the game, just type `./game` to execute it.
