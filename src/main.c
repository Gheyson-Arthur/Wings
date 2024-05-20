#include <string.h> // Biblioteca para manipulação de strings
#include "screen.h" // Inclui a biblioteca de funções de tela
#include "keyboard.h" // Inclui a biblioteca de funções de teclado
#include "timer.h" // Inclui a biblioteca de funções de temporizador

// Definição da struct para armazenar coordenadas
struct pos {
    int x;
    int y;
};

struct pos textPos = {34, 12}; // Coordenadas iniciais do texto
int incX = 1, incY = 1; // Incrementos para o movimento do texto

struct pos shipPos = {1, 12}; // Coordenada inicial da nave
char shipChar = '>'; // Caractere que representa a nave

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

int main() {
    static int ch = 0; // Variável para armazenar o código da tecla pressionada

    screenInit(1); // Inicializa a tela com bordas
    keyboardInit(); // Inicializa o teclado
    timerInit(50); // Inicializa o temporizador com atraso de 50 milissegundos

    printShip(); // Imprime a nave na posição inicial
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
    }

    keyboardDestroy(); // Restaura as configurações originais do teclado
    screenDestroy(); // Restaura a tela ao estado normal
    timerDestroy(); // Destrói o temporizador

    return 0; // Finaliza o programa
}
