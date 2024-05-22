#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define ASTEROID_CHAR '*' // Caractere que representa o asteroide
#define MAX_ASTEROIDS 5   // Número máximo de asteroides
#define SHIP_WIDTH 5
#define SHIP_HEIGHT 3
#define ASTEROID_WIDTH 5
#define ASTEROID_HEIGHT 3
#define TIMER_INTERVAL 25 // Intervalo do timer em milissegundos
#define ASTEROID_SPAWN_INTERVAL 20 // Intervalo para surgimento dos asteroides
#define SCORE_FILE "scores.txt"
#define MAX_TOP_SCORES 5

struct pos {
    int x;
    int y;
};

struct player {
    char *name;
    int score;
};

struct pos shipPos = {1, 12};
char shipMatrix[SHIP_HEIGHT][SHIP_WIDTH] = {
    {' ', '/', '_', '\\', ' '},
    {'<', ' ', ' ', ' ', '>'},
    {' ', '\\', '_', '/', ' '}
};

struct asteroid {
    struct pos pos;
    char matrix[ASTEROID_HEIGHT][ASTEROID_WIDTH];
    int active;
};

struct asteroid asteroids[MAX_ASTEROIDS];
struct player currentPlayer;

int asteroidCounter = 0;
int asteroidSpawnTimer = 0;

void initAsteroids() {
    for (int k = 0; k < MAX_ASTEROIDS; k++) {
        asteroids[k].pos.x = MAXX - 6;
        asteroids[k].pos.y = MINY + 1 + rand() % (MAXY - MINY - 4);
        memcpy(asteroids[k].matrix, (char[ASTEROID_HEIGHT][ASTEROID_WIDTH]) {
            {' ', '*', '*', '*', ' '},
            {'*', '*', '*', '*', '*'},
            {' ', '*', '*', '*', ' '}
        }, sizeof(asteroids[k].matrix));
        asteroids[k].active = 0;
    }
}

void activateAsteroid() {
    for (int k = 0; k < MAX_ASTEROIDS; k++) {
        if (!asteroids[k].active) {
            asteroids[k].pos.x = MAXX - 6;
            asteroids[k].pos.y = MINY + 1 + rand() % (MAXY - MINY - 4);
            asteroids[k].active = 1;
            break;
        }
    }
}

void printShip() {
    screenSetColor(GREEN, DARKGRAY);
    for (int i = 0; i < SHIP_HEIGHT; i++) {
        for (int j = 0; j < SHIP_WIDTH; j++) {
            screenGotoxy(MINX + shipPos.x + j, shipPos.y + i);
            printf("%c", shipMatrix[i][j]);
        }
    }
}

void clearShip() {
    for (int i = 0; i < SHIP_HEIGHT; i++) {
        for (int j = 0; j < SHIP_WIDTH; j++) {
            screenGotoxy(MINX + shipPos.x + j, shipPos.y + i);
            printf(" ");
        }
    }
}

void printAsteroids() {
    screenSetColor(RED, DARKGRAY);
    for (int k = 0; k < MAX_ASTEROIDS; k++) {
        if (asteroids[k].active) {
            for (int i = 0; i < ASTEROID_HEIGHT; i++) {
                for (int j = 0; j < ASTEROID_WIDTH; j++) {
                    screenGotoxy(MINX + asteroids[k].pos.x + j, asteroids[k].pos.y + i);
                    printf("%c", asteroids[k].matrix[i][j]);
                }
            }
        }
    }
}

void clearAsteroids() {
    for (int k = 0; k < MAX_ASTEROIDS; k++) {
        if (asteroids[k].active) {
            for (int i = 0; i < ASTEROID_HEIGHT; i++) {
                for (int j = 0; j < ASTEROID_WIDTH; j++) {
                    screenGotoxy(MINX + asteroids[k].pos.x + j, asteroids[k].pos.y + i);
                    printf(" ");
                }
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
    for (int k = 0; k < MAX_ASTEROIDS; k++) {
        if (asteroids[k].active) {
            for (int i = 0; i < SHIP_HEIGHT; i++) {
                for (int j = 0; j < SHIP_WIDTH; j++) {
                    int shipX = shipPos.x + j;
                    int shipY = shipPos.y + i;
                    int asteroidX = asteroids[k].pos.x;
                    int asteroidY = asteroids[k].pos.y;

                    if (shipX >= asteroidX && shipX < asteroidX + ASTEROID_WIDTH &&
                        shipY >= asteroidY && shipY < asteroidY + ASTEROID_HEIGHT) {
                        return 1; // Colisão detectada
                    }
                }
            }
        }
    }
    return 0; // Nenhuma colisão
}

void updateAsteroids() {
    for (int k = 0; k < MAX_ASTEROIDS; k++) {
        if (asteroids[k].active) {
            if (asteroids[k].pos.x > MINX + 1) {
                asteroids[k].pos.x--;
            } else {
                asteroids[k].active = 0;
                asteroidCounter++;
                printCounter();
            }
        }
    }
}

void handleInput(int ch) {
    if (ch == 'w' || ch == 'W') {
        if (shipPos.y > MINY + 1) {
            clearShip();
            shipPos.y--;
            printShip();
        }
    } else if (ch == 's' || ch == 'S') {
        if (shipPos.y < MAXY - SHIP_HEIGHT) {
            clearShip();
            shipPos.y++;
            printShip();
        }
    }
    screenUpdate();
}

void saveScore(struct player *p) {
    FILE *file = fopen(SCORE_FILE, "a");
    if (file != NULL) {
        fprintf(file, "Player: %s, Score: %d\n", p->name, p->score);
        fclose(file);
    } else {
        printf("Error: Unable to open %s for writing.\n", SCORE_FILE);
    }
}

void displayTopScores() {
    struct player topScores[MAX_TOP_SCORES];
    for (int i = 0; i < MAX_TOP_SCORES; i++) {
        topScores[i].name = NULL;
        topScores[i].score = 0;
    }

    FILE *file = fopen(SCORE_FILE, "r");
    if (file != NULL) {
        char line[100];
        int scoreIndex = 0; // Índice para controlar o número de pontuações lidas
        while (fgets(line, sizeof(line), file) != NULL && scoreIndex < MAX_TOP_SCORES) {
            printf("Debug: Line read from file: %s\n", line); // Debugging: Print the line read from the file
            struct player tempPlayer;
            tempPlayer.name = (char *)malloc(50 * sizeof(char));
            if (tempPlayer.name == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                exit(1);
            }
            int result = sscanf(line, "Player: %49s, Score: %d", tempPlayer.name, &tempPlayer.score);

            // Verifica se a leitura foi bem-sucedida
            if (result == 2) {
                topScores[scoreIndex++] = tempPlayer;
            } else {
                free(tempPlayer.name); // Libera a memória alocada para o nome
            }
        }
        fclose(file);

        screenClear();
        screenSetColor(YELLOW, DARKGRAY);
        screenGotoxy((MAXX - 9) / 2, MINY + 1);
        printf("Top Scores");
        for (int i = 0; i < MAX_TOP_SCORES && topScores[i].name != NULL; i++) {
            screenGotoxy((MAXX - 20) / 2, MINY + 3 + i);
            printf("%d. %s - %d", i + 1, topScores[i].name, topScores[i].score);
            free(topScores[i].name);
        }
        screenUpdate();
        sleep(5); // Pause for 5 seconds to allow the player to see the top scores
    } else {
        printf("Error: Unable to open %s for reading.\n", SCORE_FILE);
    }
}

void displayWelcomeScreen() {
    screenClear();
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy((MAXX - 20) / 2, MINY + 3);
    printf("Welcome to Asteroid Game!");
    screenGotoxy((MAXX - 20) / 2, MINY + 5);
    printf("Enter your name: ");
    screenGotoxy((MAXX - 20) / 2, MINY + 7);
    printf(">");
    screenUpdate();
    screenGotoxy((MAXX - 20) / 2 + 2, MINY + 7); // Move cursor to input position
}

int main() {
    static int ch = 0;

    timerInit(TIMER_INTERVAL);

    displayWelcomeScreen();

    char playerName[50];
    if (scanf("%49s", playerName) != 1) {
        fprintf(stderr, "Error reading player name.\n");
        return 1;
    }

    screenInit(0);
    keyboardInit();

    currentPlayer.name = strdup(playerName);
    currentPlayer.score = 0;

    srand(time(NULL));
    screenClear(); // Clear the screen after reading the player's name
    screenSetColor(WHITE, BLACK);
    initAsteroids();
    printShip();
    screenUpdate();

    while (ch != 10) { // 10 is the ASCII code for Enter key
        if (keyhit()) {
            ch = readch();
            handleInput(ch);
        }

        clearAsteroids();
        updateAsteroids();

        if (asteroidSpawnTimer >= ASTEROID_SPAWN_INTERVAL) {
            activateAsteroid();
            asteroidSpawnTimer = 0;
        } else {
            asteroidSpawnTimer++;
        }

        printAsteroids();
        screenUpdate();

        if (checkCollision()) {
            screenSetColor(YELLOW, RED);
            screenGotoxy((MAXX - 13) / 2, (MAXY - 1) / 2);
            printf("Game Over!");
            screenUpdate();
            sleep(3); // Pause for 3 seconds to allow the player to see the Game Over message
            break;
        }

        usleep(TIMER_INTERVAL * 1000); // Convert milliseconds to microseconds
    }

    currentPlayer.score = asteroidCounter;
    saveScore(&currentPlayer);

    free(currentPlayer.name);
    displayTopScores();

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
