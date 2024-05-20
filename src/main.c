#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define ASTEROID_CHAR '*' // Caractere que representa o asteroide

struct pos {
    int x;
    int y;
};

struct pos textPos = {34, 12};
int incX = 1, incY = 1;

struct pos shipPos = {1, 12};
char shipMatrix[3][5] = {
    {' ', '/', '_', '\\', ' '},
    {'<', ' ', ' ', ' ', '>'},
    {' ', '\\', '_', '/', ' '}
};

struct pos asteroidPos = {MAXX - 7, 12};
char asteroidMatrix[3][5] = {
    {' ', '*', '*', '*', ' '},
    {'*', '*', '*', '*', '*'},
    {' ', '*', '*', '*', ' '}
};

int asteroidCounter = 0;

void printShip() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            screenSetColor(GREEN, DARKGRAY);
            screenGotoxy(MINX + shipPos.x + j, shipPos.y + i);
            printf("%c", shipMatrix[i][j]);
        }
    }
}

void clearShip() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            screenGotoxy(MINX + shipPos.x + j, shipPos.y + i);
            printf(" ");
        }
    }
}

void printAsteroid() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            screenSetColor(RED, DARKGRAY);
            screenGotoxy(MINX + asteroidPos.x + j, asteroidPos.y + i);
            printf("%c", asteroidMatrix[i][j]);
        }
    }
}

void clearAsteroid() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            screenGotoxy(MINX + asteroidPos.x + j, asteroidPos.y + i);
            printf(" ");
        }
    }
}

void printCounter() {
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(MAXX - 22, MINY + 1);
    printf("%d", asteroidCounter);
}

int checkCollision() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            int shipX = shipPos.x + j;
            int shipY = shipPos.y + i;
            int asteroidX = asteroidPos.x + j;
            int asteroidY = asteroidPos.y + i;

            if (shipX >= asteroidPos.x && shipX < asteroidPos.x + 5 &&
                shipY >= asteroidPos.y && shipY < asteroidPos.y + 3) {
                return 1; // Colisão detectada
            }
        }
    }
    return 0; // Nenhuma colisão
}


int main() {
    static int ch = 0;

    srand(time(NULL));
    screenInit(1);
    keyboardInit();
    timerInit(50);

    printShip();
    printAsteroid();
    screenUpdate();

    while (ch != 10) {
        if (keyhit()) {
            ch = readch();
            if (ch == 'w' || ch == 'W') {
                if (shipPos.y > MINY + 1) {
                    clearShip();
                    shipPos.y--;
                    printShip();
                }
            } else if (ch == 's' || ch == 'S') {
                if (shipPos.y < MAXY - 3) {
                    clearShip();
                    shipPos.y++;
                    printShip();
                }
            }
            screenUpdate();
        }

        clearAsteroid();
        if (asteroidPos.x > MINX + 1) {
            asteroidPos.x--;
            printAsteroid();
            screenUpdate();
        } else {
            asteroidPos.x = MAXX - 7;
            asteroidPos.y = MINY + 1 + rand() % (MAXY - MINY - 2);
            asteroidCounter++;
            printCounter();
        }

        if (checkCollision()) {
            screenSetColor(YELLOW, RED);
            screenGotoxy((MAXX - 13) / 2, (MAXY - 1) / 2);
            printf("Game Over!");
            screenUpdate();
            break;
        }

        usleep(50000);
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
