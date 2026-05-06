/* * 
 * Funcionalidade: Recebe strings via FIFO, calcula e exibe o tamanho, número de consoantes, vogais e espaços.
 * Autor(es): João Pedro Moraes Guedes
 * Data: Maio de 2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define FIFO_PATH "/tmp/ex1_fifo"
#define MAX_SIZE 256

/*
 * Função: processar_string
 * Finalidade: Analisa a string recebida e conta seus caracteres.
 * Parâmetros de entrada: const char *str (ponteiro para a string a ser analisada)
 * Parâmetros de saída: Nenhum (imprime os resultados na tela).
 */
void processar_string(const char *str) {
    int tamanho = strlen(str);
    int vogais = 0, consoantes = 0, espacos = 0;

    for (int i = 0; i < tamanho; i++) {
        char c = tolower(str[i]);
        if (c == ' ') {
            espacos++;
        } else if (isalpha(c)) {
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vogais++;
            } else {
                consoantes++;
            }
        }
    }

    printf("\n--- Resultados ---\n");
    printf("String: '%s'\n", str);
    printf("Tamanho: %d\n", tamanho);
    printf("Vogais: %d\n", vogais);
    printf("Consoantes: %d\n", consoantes);
    printf("Espaços: %d\n", espacos);
    printf("---------------------\n");
}

/*
 * Função: main
 * Finalidade: Ponto de entrada do programa leitor. Lê o FIFO e chama o processamento.
 * Parâmetros de entrada: Nenhum.
 * Parâmetros de saída: Retorna 0 em caso de sucesso ou 1 em erro.
 */
int main() {
    int fd;
    char buffer[MAX_SIZE];

    printf("Leitor iniciado. Aguardando o escritor...\n");

    // Abre o FIFO no modo de leitura (bloqueia até o escritor abrir)
    fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Erro ao abrir FIFO para leitura");
        return 1;
    }

    // Loop principal lendo do FIFO
    while (read(fd, buffer, MAX_SIZE) > 0) {
        if (strcmp(buffer, "sair") == 0) {
            printf("Comando de saída recebido. Encerrando...\n");
            break;
        }
        processar_string(buffer);
    }

    // Limpeza: fecha o descritor e remove o arquivo FIFO do sistema
    close(fd);
    unlink(FIFO_PATH); 
    return 0;
}