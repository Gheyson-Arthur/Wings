#include <string.h> // Biblioteca para manipulação de strings

#include "screen.h" // Inclui a biblioteca de funções de tela
#include "keyboard.h" // Inclui a biblioteca de funções de teclado
#include "timer.h" // Inclui a biblioteca de funções de temporizador

int x = 34, y = 12; // Coordenadas iniciais do texto
int incX = 1, incY = 1; // Incrementos para o movimento do texto

int shipY = 12; // Coordenada Y inicial da nave
char *shipChar = " <:*_|> "; // Caractere que representa a nave

// Função para imprimir a nave na posição atual
void printShip()
{
    screenSetColor(GREEN, DARKGRAY); // Define as cores da nave
    screenGotoxy(MINX + 1, shipY); // Move o cursor para a posição da nave
    printf("%s", shipChar); // Imprime o caractere da nave
}

// Função para limpar a nave da posição atual
void clearShip()
{
    // Limpa uma área maior do que o caractere da nave para garantir que qualquer parte da nave seja apagada
    for (int i = MINX + 1; i < MAXX; i++) { // Evita limpar as linhas da borda
        for (int j = shipY - 1; j <= shipY + 1; j++) {
            screenGotoxy(i, j);
            printf(" ");
        }
    }
}

int main() 
{
    static int ch = 0; // Variável para armazenar o código da tecla pressionada

    screenInit(1); // Inicializa a tela com bordas
    keyboardInit(); // Inicializa o teclado
    timerInit(50); // Inicializa o temporizador com atraso de 50 milissegundos

    printShip(); // Imprime a nave na posição inicial
    screenUpdate(); // Atualiza a tela

    while (ch != 10) // Enquanto a tecla 'Enter' não for pressionada
    {
        // Verifica se uma tecla foi pressionada
        if (keyhit()) 
        {
            ch = readch(); // Lê o código da tecla pressionada
            if (ch == 'w' || ch == 'W') // Se a tecla é 'W', move a nave para cima
            {
                if (shipY > MINY + 1) // Garante que a nave não saia da tela
                {
                    clearShip(); // Limpa a posição atual da nave
                    shipY--; // Move a nave para cima
                    printShip(); // Imprime a nave na nova posição
                }
            }
            else if (ch == 's' || ch == 'S') // Se a tecla é 'S', move a nave para baixo
            {
                if (shipY < MAXY - 1) // Garante que a nave não saia da tela
                {
                    clearShip(); // Limpa a posição atual da nave
                    shipY++; // Move a nave para baixo
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
