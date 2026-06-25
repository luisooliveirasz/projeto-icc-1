# Define o compilador
CC = gcc

# Flags de compilação (C99 exigido pelo PDF e avisos ativados)
CFLAGS = -std=c99 -Wall 

# Nome do executável gerado
TARGET = exec_mercadinho

# A regra 'all' é a padrão, executada quando se digita apenas 'make'
all: $(TARGET)

# Como compilar o programa (pega todos os arquivos .c do diretório)
$(TARGET): *.c
	$(CC) $(CFLAGS) *.c -o $(TARGET)

# Regra para rodar o programa 
run: $(TARGET)
	./$(TARGET)

# Regra para gerar o ZIP da entrega corretamente
zip:
	zip entrega.zip Makefile README.txt *.c

# Limpa o executável e o zip gerado (para não deixar lixo na pasta)
clean:
	rm -f $(TARGET) projeto_entrega.zip