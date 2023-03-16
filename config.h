#pragma once

//All the libraries used for the game
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <algorithm>
#include <ncurses.h>

//Namespace STD is used as we do not write another namespace that conflicts with our program
using namespace std;

//All the used functions throughout the program
void menu(int &menu, int x, int y);
void menuSelect(int &select, int x, int y);
void tryMenu(int &select, int x, int y);
void tryAgain(int &choice, int x, int y);
bool outofRange(int shipX, int shipY, int x, int y);
void loadShip(int &shipSize, int &enemyShipSize, vector<const char *> &shipMatrix, vector<const char *> &enemyShipMatrix);
void loadScore(int &highScore);
void saveScore(int highScore);
void printScore(int x, int score, int highScore);
void printNotify(int y, int hpRegen, int damageIncrease, int enemyHPIncrease, int enemyDamage);
void printStats(int y, int hp, int damage, int enemyHP, bool equipWeapon);
void printDamagePowerUp();
void printWeaponPowerUp();
void printHPUp();
void printEnemyShip();
void printShip(int shipX, int shipY, const char *matrix[3]);
void printBullets(bool equipWeapon);
void printEnemyBullets(int y);
void hitDetection(int &score, int &damage, int &hp, int enemyDamage, int hpRegen, int damageIncrease, bool &equipWeapon, std::chrono::system_clock::time_point &equipWeaponStartTime, int shipSize, int shipX, int shipY);
void spawnPowerUp(float period, int x, int y, std::chrono::system_clock::time_point &powerUpStartTime, vector<pair<int, int>> &powerUpPos);
void checkShipStatus(int &current_ship, int &score);
void spawnEnemyShip(int x, int y, int &current_ship, int enemy_hp, int enemyShipSize);
void addBullets(bool equipWeapon, int shipX, int shipY, std::chrono::system_clock::time_point &bulletStartTime);
void addEnemyBullets(float period, float sleepTime, int shipX, int shipY, std::chrono::system_clock::time_point &enemyBulletStartTime, std::chrono::system_clock::time_point &enemyBulletSleepTime);
void resetGameState();
void gameBoard(int x, int y);
