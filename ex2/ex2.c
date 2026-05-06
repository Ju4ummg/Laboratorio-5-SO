/* * Funcionalidade: Manipula um arquivo e realiza uma finalização limpa (graceful stop) 
 * ao receber os sinais SIGINT ou SIGTERM.
 * Autor(es): João Pedro Moraes Guedes
 * Data: Maio de 2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

// Variável global atômica para controle do loop principal de forma segura
volatile sig_atomic_t executando = 1;

/*
 * Função: tratador_sinal
 * Finalidade: Captura os sinais do SO e altera a flag de execução para iniciar a parada.
 * Parâmetros de entrada: int sinal (o código do sinal recebido, ex: 2 ou 15).
 * Parâmetros de saída: Nenhum.
 */
void tratador_sinal(int sinal) {
    if (sinal == SIGINT || sinal == SIGTERM) {
        // Escreve no terminal de forma segura em tratadores de sinais usando write
        const char msg[] = "\n[Sinal recebido! Iniciando graceful stop...]\n";
        write(STDOUT_FILENO, msg, sizeof(msg) - 1);
        executando = 0; // Sinaliza o loop para parar
    }
}

/*
 * Função: main
 * Finalidade: Ponto de entrada do programa. Configura os sinais, abre o arquivo e simula o trabalho.
 * Parâmetros de entrada: Nenhum.
 * Parâmetros de saída: Retorna 0.
 */
int main() {
    FILE *arquivo;
    int contador = 1;
    char buffer_pendente[256] = "";

    // Associa os sinais SIGINT e SIGTERM à função tratador_sinal
    struct sigaction sa;
    sa.sa_handler = tratador_sinal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);  // Signal 2
    sigaction(SIGTERM, &sa, NULL); // Signal 15

    // Abre o arquivo para simular escrita contínua (append mode)
    arquivo = fopen("log_execucao.txt", "a");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    printf("Programa em execução. Pressione Ctrl+C (SIGINT) ou envie SIGTERM (kill -15) para finalizar.\n");

    // Loop de simulação de processamento
    while (executando) {
        // Simula a geração de uma informação que fica pendente em memória
        snprintf(buffer_pendente, sizeof(buffer_pendente), "Registro da operacao %d gravado com sucesso.\n", contador);
        printf("Processando operacao %d...\n", contador);
        
        // Simula um delay (1 segundo por iteração)
        sleep(1);
        contador++;
    }

    // --- GRACEFUL STOP ---
    printf("\nRealizando finalização limpa...\n");
    
    // Armazena as informações pendentes antes de fechar o arquivo
    if (strlen(buffer_pendente) > 0) {
        fprintf(arquivo, "%s", buffer_pendente);
        printf("Informações pendentes salvas no arquivo.\n");
    }

    // Fecha o arquivo adequadamente
    fclose(arquivo);
    printf("Arquivo fechado. Programa encerrado com sucesso.\n");

    return 0;
}