#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

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
    char name[50];
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
    int active;
};

char asteroidPattern[ASTEROID_HEIGHT][ASTEROID_WIDTH] = {
    {' ', '*', '*', '*', ' '},
    {'*', '*', '*', '*', '*'},
    {' ', '*', '*', '*', ' '}
};

struct asteroid asteroids[MAX_ASTEROIDS];
struct player currentPlayer;

int asteroidCounter = 0;
int asteroidSpawnTimer = 0;

void inicializarAsteroides() {
    for (int k = 0; k < MAX_ASTEROIDS; k++) {
        asteroids[k].pos.x = MAXX - 6;
        asteroids[k].pos.y = MINY + 1 + rand() % (MAXY - MINY - 4);
        asteroids[k].active = 0;
    }
}

void ativarAsteroide() {
    for (int k = 0; k < MAX_ASTEROIDS; k++) {
        if (!asteroids[k].active) {
            asteroids[k].pos.x = MAXX - 6;
            asteroids[k].pos.y = MINY + 1 + rand() % (MAXY - MINY - 4);
            asteroids[k].active = 1;
            break;
        }
    }
}

void imprimirNave() {
    screenSetColor(GREEN, DARKGRAY);
    for (int i = 0; i < SHIP_HEIGHT; i++) {
        for (int j = 0; j < SHIP_WIDTH; j++) {
            screenGotoxy(MINX + shipPos.x + j, shipPos.y + i);
            printf("%c", shipMatrix[i][j]);
        }
    }
}

void limparNave() {
    for (int i = 0; i < SHIP_HEIGHT; i++) {
        for (int j = 0; j < SHIP_WIDTH; j++) {
            screenGotoxy(MINX + shipPos.x + j, shipPos.y + i);
            printf(" ");
        }
    }
}

void imprimirAsteroides() {
    screenSetColor(RED, DARKGRAY);
    for (int k = 0; k < MAX_ASTEROIDS; k++) {
        if (asteroids[k].active) {
            for (int i = 0; i < ASTEROID_HEIGHT; i++) {
                for (int j = 0; j < ASTEROID_WIDTH; j++) {
                    screenGotoxy(MINX + asteroids[k].pos.x + j, asteroids[k].pos.y + i);
                    printf("%c", asteroidPattern[i][j]);
                }
            }
        }
    }
}

void limparAsteroides() {
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

void imprimirContador() {
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(MAXX - 22, MINY + 1);
    printf("%d", asteroidCounter);
}

int verificarColisao() {
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

void atualizarAsteroides() {
    for (int k = 0; k < MAX_ASTEROIDS; k++) {
        if (asteroids[k].active) {
            if (asteroids[k].pos.x > MINX + 1) {
                asteroids[k].pos.x--;
            } else {
                asteroids[k].active = 0;
                asteroidCounter++;
                imprimirContador();
            }
        }
    }
}

void tratarEntrada(int ch) {
    if (ch == 'w' || ch == 'W') {
        if (shipPos.y > MINY + 1) {
            limparNave();
            shipPos.y--;
            imprimirNave();
        }
    } else if (ch == 's' || ch == 'S') {
        if (shipPos.y < MAXY - SHIP_HEIGHT) {
            limparNave();
            shipPos.y++;
            imprimirNave();
        }
    }
    screenUpdate();
}

void salvarPontuacao(struct player *p) {
    FILE *file = fopen(SCORE_FILE, "a");
    if (file != NULL) {
        fprintf(file, "Player: %s, Score: %d\n", p->name, p->score);
        fclose(file);
    } 
}

void ordenarPontuacoes(struct player *scores, int count) {
    struct player temp;
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (scores[i].score < scores[j].score) {
                temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }
}

void exibirPrimeirasCincoPontuacoes() {
    FILE *file = fopen(SCORE_FILE, "r");
    if (file == NULL) {
        file = fopen(SCORE_FILE, "w"); 
        if (file == NULL) {
            printf("Error: Unable to create %s.\n", SCORE_FILE);
            return;
        }
        fclose(file);
        file = fopen(SCORE_FILE, "r"); 
    }

    int count = 0;
    char name[50]; 
    int score; 

    while (fscanf(file, "Player: %49s Score: %d\n", name, &score) == 2) {
        count++;
    }
    rewind(file); 

    struct player *scores = malloc(count * sizeof(struct player));

    count = 0;
    while (fscanf(file, "Player: %49s Score: %d\n", name, &score) == 2) {
        strcpy(scores[count].name, name);
        scores[count].score = score;
        count++;
    }
    fclose(file);

    ordenarPontuacoes(scores, count);

    screenClear();
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy((MAXX - 20) / 2, MINY + 1);
    printf("PÓDIO DE SCORES");
    int y = MINY + 3;

    int maxScores;
    if (count < MAX_TOP_SCORES) {
        maxScores = count;
    } else {
        maxScores = MAX_TOP_SCORES;
    }

    for (int i = 0; i < maxScores; i++) {
        screenGotoxy((MAXX - 50) / 2, y++);
        printf("Player: %s, Score: %d", scores[i].name, scores[i].score);
    }

    screenUpdate();
    sleep(5);

    free(scores); 
}


void exibirTelaBoasVindas() {
    screenClear();
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy((MAXX - 20) / 2, MINY + 3);
    printf("Welcome to Asteroid Game!");
    screenGotoxy((MAXX - 20) / 2, MINY + 5);
    printf("Enter your name: ");
    screenGotoxy((MAXX - 20) / 2, MINY + 7);
    printf(">");
    screenUpdate();
    screenGotoxy((MAXX - 20) / 2 + 2, MINY + 7); 
}

int main() {
    static int ch = 0;

    exibirTelaBoasVindas();

    char playerName[21];
    scanf("%20s", playerName);
    
    screenInit(0);
    keyboardInit();

    strcpy(currentPlayer.name, playerName);
    currentPlayer.score = 0;

    srand(time(NULL));
    screenClear(); 
    screenSetColor(WHITE, BLACK);
    inicializarAsteroides();
    imprimirNave();
    screenUpdate();

    while (ch != 10) { 
        if (keyhit()) {
            ch = readch();
            tratarEntrada(ch);
        }

        limparAsteroides();
        atualizarAsteroides();

        if (asteroidSpawnTimer >= ASTEROID_SPAWN_INTERVAL) {
            ativarAsteroide();
            asteroidSpawnTimer = 0;
        } else {
            asteroidSpawnTimer++;
        }

        imprimirAsteroides();
        screenUpdate();

        if (verificarColisao()) {
            screenSetColor(YELLOW, RED);
            screenGotoxy((MAXX - 13) / 2, (MAXY - 1) / 2);
            printf("Game Over!");
            screenUpdate();
            sleep(3); 
            break;
        }

        usleep(TIMER_INTERVAL * 1000); 
    }

    currentPlayer.score = asteroidCounter;
    salvarPontuacao(&currentPlayer);

    exibirPrimeirasCincoPontuacoes(); 

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
