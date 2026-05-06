/* * 
 * Funcionalidade: Lê strings digitadas pelo usuário e as envia para outro processo através de um FIFO.
 * Autor(es): João Pedro Moraes Guedes
 * Data: Maio de 2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/ex1_fifo"
#define MAX_SIZE 256

/*
 * Função: main
 * Finalidade: Ponto de entrada do programa escritor. Cria o FIFO e envia os dados.
 * Parâmetros de entrada: Nenhum.
 * Parâmetros de saída: Retorna 0 em caso de sucesso ou 1 em caso de erro.
 */
int main() {
    int fd;
    char buffer[MAX_SIZE];

    // Cria o FIFO (named pipe) se ele não existir
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        perror("Aviso: FIFO já existe ou erro ao criar");
    }

    printf("Escritor iniciado. Aguardando o leitor se conectar...\n");
    
    // Abre o FIFO no modo de escrita (bloqueia até o leitor abrir)
    fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Erro ao abrir FIFO para escrita");
        return 1;
    }

    printf("Conectado! Digite suas strings (ou 'sair' para encerrar):\n");

    // Loop principal para ler do terminal e escrever no FIFO
    while (1) {
        printf("> ");
        if (fgets(buffer, MAX_SIZE, stdin) != NULL) {
            // Remove o \n do final da string, se houver
            buffer[strcspn(buffer, "\n")] = 0;
            
            // Escreve a string no FIFO
            write(fd, buffer, strlen(buffer) + 1);

            // Condição de parada
            if (strcmp(buffer, "sair") == 0) {
                break;
            }
        }
    }

    // Limpeza e finalização
    close(fd);
    printf("Encerrando escritor.\n");
    return 0;
}