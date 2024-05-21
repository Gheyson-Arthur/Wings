#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define ASTEROID_CHAR '*' // Caractere que representa o asteroide
#define MAX_ASTEROIDS 3   // Número máximo de asteroides

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

struct asteroid {
    struct pos pos;
    char matrix[3][5];
};

struct asteroid asteroids[MAX_ASTEROIDS];

int asteroidCounter = 0;

void initAsteroids() {
    for (int k = 0; k < 3; k++) {
        asteroids[k].pos.x = MAXX - 6; // Ajuste conforme necessário para evitar sair da tela
        asteroids[k].pos.y = MINY + 1 + rand() % (MAXY - MINY - 4); // Ajuste conforme necessário para evitar sair da tela
        memcpy(asteroids[k].matrix, (char[3][5]) {
            {' ', '*', '*', '*', ' '},
            {'*', '*', '*', '*', '*'},
            {' ', '*', '*', '*', ' '}
        }, sizeof(asteroids[k].matrix));
    }
}

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

void printAsteroids() {
    for (int k = 0; k < 3; k++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 5; j++) {
                screenSetColor(RED, DARKGRAY);
                screenGotoxy(MINX + asteroids[k].pos.x + j, asteroids[k].pos.y + i);
                printf("%c", asteroids[k].matrix[i][j]);
            }
        }
    }
}

void clearAsteroids() {
    for (int k = 0; k < 3; k++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 5; j++) {
                screenGotoxy(MINX + asteroids[k].pos.x + j, asteroids[k].pos.y + i);
                printf(" ");
            }
        }
    }
}

void printCounter() {
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(MAXX - 22, MINY + 1);
    printf("%d", asteroidCounter);
}

int checkCollision() {
    for (int k = 0; k < 3; k++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 5; j++) {
                int shipX = shipPos.x + j;
                int shipY = shipPos.y + i;
                int asteroidX = asteroids[k].pos.x + j;
                int asteroidY = asteroids[k].pos.y + i;

                if (shipX >= asteroids[k].pos.x && shipX < asteroids[k].pos.x + 5 &&
                    shipY >= asteroids[k].pos.y && shipY < asteroids[k].pos.y + 3) {
                    return 1; // Colisão detectada
                }
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

    initAsteroids();
    printShip();
    printAsteroids();
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

        clearAsteroids();
        for (int k = 0; k < 3; k++) {
            if (asteroids[k].pos.x > MINX + 1) {
                asteroids[k].pos.x--;
            } else {
                asteroids[k].pos.x = MAXX - 6;
                asteroids[k].pos.y = MINY + 1 + rand() % (MAXY - MINY - 4);
                asteroidCounter++;
                printCounter();
            }
        }
        printAsteroids();
        screenUpdate();

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
