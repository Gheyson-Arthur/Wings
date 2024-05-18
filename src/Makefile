CC = gcc

# Caminho para os arquivos de cabeçalho
CFLAGS = -I../include

# Nome do arquivo executável final
TARGET = Wings

# Arquivos objeto correspondentes aos arquivos fonte
OBJ = main.o screen.o timer.o keyboard.o

# A regra all depende do TARGET
all: $(TARGET)

# Regra para criar o executável final
$(TARGET): $(OBJ)
	$(CC) -o $@ $^

# Regras para compilar os arquivos fonte para arquivos objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Limpa os arquivos gerados pelo makefile utilizando o comando --> make clean
clean: 
	rm -f $(TARGET) $(OBJ)
