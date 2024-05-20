#include <stdio.h>
#include <stdlib.h> // Biblioteca para funções de aleatoriedade
#include <string.h> // Biblioteca para manipulação de strings
#include <unistd.h> // Para usar a função usleep()
#include <time.h>   // Biblioteca para funções de tempo (necessária para srand e time)
#include "screen.h" // Inclui a biblioteca de funções de tela
#include "keyboard.h" // Inclui a biblioteca de funções de teclado
#include "timer.h" // Inclui a biblioteca de funções de temporizador

#define ASTEROID_CHAR '*' // Caractere que representa o asteroide

// Definição da struct para armazenar coordenadas
struct pos {
    int x;
    int y;
};

struct pos textPos = {34, 12}; // Coordenadas iniciais do texto
int incX = 1, incY = 1; // Incrementos para o movimento do texto

struct pos shipPos = {1, 12}; // Coordenada inicial da nave
char shipChar = '>'; // Caractere que representa a nave

struct pos asteroidPos = {MAXX - 7, 12}; // Coordenada inicial do asteroide

int asteroidCounter = 0; // Contador de vezes que o asteroide chega à extremidade esquerda sem colidir com a nave

// Função para imprimir a nave na posição atual
void printShip() {
    screenSetColor(GREEN, DARKGRAY); // Define as cores da nave
    screenGotoxy(MINX + shipPos.x, shipPos.y); // Move o cursor para a posição da nave
    printf("%c", shipChar); // Imprime o caractere da nave
}

// Função para limpar a nave da posição atual
void clearShip() {
    // Limpa apenas a área ocupada pela nave
    screenGotoxy(MINX + shipPos.x, shipPos.y);
    printf(" ");
}

// Função para imprimir o asteroide na posição atual
void printAsteroid() {
    screenSetColor(RED, DARKGRAY); // Define as cores do asteroide
    screenGotoxy(MINX + asteroidPos.x, asteroidPos.y); // Move o cursor para a posição do asteroide
    printf("%c", ASTEROID_CHAR); // Imprime o caractere do asteroide
}

// Função para limpar o asteroide da posição atual
void clearAsteroid() {
    // Limpa apenas a área ocupada pelo asteroide
    screenGotoxy(MINX + asteroidPos.x, asteroidPos.y);
    printf(" ");
}

// Função para imprimir o contador no canto superior direito
void printCounter() {
    screenSetColor(YELLOW, DARKGRAY); // Define as cores do contador
    screenGotoxy(MAXX - 22, MINY + 1); // Move o cursor para o canto superior direito
    printf("%d", asteroidCounter); // Imprime o contador
}

int main() {
    static int ch = 0; // Variável para armazenar o código da tecla pressionada

    srand(time(NULL)); // Inicializa a semente do gerador de números aleatórios

    screenInit(1); // Inicializa a tela com bordas
    keyboardInit(); // Inicializa o teclado
    timerInit(50); // Inicializa o temporizador com atraso de 50 milissegundos

    printShip(); // Imprime a nave na posição inicial
    printAsteroid(); // Imprime o asteroide na posição inicial
    screenUpdate(); // Atualiza a tela

    while (ch != 10) // Enquanto a tecla 'Enter' não for pressionada
    {
        // Verifica se uma tecla foi pressionada
        if (keyhit()) {
            ch = readch(); // Lê o código da tecla pressionada
            if (ch == 'w' || ch == 'W') { // Se a tecla é 'W', move a nave para cima
                if (shipPos.y > MINY + 1) { // Garante que a nave não saia da tela
                    clearShip(); // Limpa a posição atual da nave
                    shipPos.y--; // Move a nave para cima
                    printShip(); // Imprime a nave na nova posição
                }
            } else if (ch == 's' || ch == 'S') { // Se a tecla é 'S', move a nave para baixo
                if (shipPos.y < MAXY - 1) { // Garante que a nave não saia da tela
                    clearShip(); // Limpa a posição atual da nave
                    shipPos.y++; // Move a nave para baixo
                    printShip(); // Imprime a nave na nova posição
                }
            }
            screenUpdate(); // Atualiza a tela
        }

        // Move o asteroide da direita para a esquerda
        clearAsteroid(); // Limpa a posição atual do asteroide
        if (asteroidPos.x > MINX + 1) { // Garante que o asteroide não saia da tela
            asteroidPos.x--; // Move o asteroide para a esquerda
            printAsteroid(); // Imprime o asteroide na nova posição
            screenUpdate(); // Atualiza a tela
        } else {
            // Reposiciona o asteroide para a posição inicial
            asteroidPos.x = MAXX - 7;
            // Gera uma nova coordenada y aleatória para o asteroide
            asteroidPos.y = MINY + 1 + rand() % (MAXY - MINY - 2);
            // Incrementa o contador de asteroide se não houver colisão com a nave
            asteroidCounter++;
            printCounter(); // Atualiza o contador na tela
        }

        // Verifica se houve colisão com a nave
        if (shipPos.x == asteroidPos.x && shipPos.y == asteroidPos.y) {
            printf("Game Over!\n");
            break;
        }

        usleep(50000); // Pausa o programa por 50 milissegundos
    }

    keyboardDestroy(); // Restaura as configurações originais do teclado
    screenDestroy(); // Restaura a tela ao estado normal
    timerDestroy(); // Destrói o temporizador

    return 0; // Finaliza o programa
}
