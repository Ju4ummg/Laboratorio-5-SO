/*
 * Funcionalidade: Soma de vetores distribuída entre processos filhos.
 * Usa SHM para os vetores e sinalização, e Pipes para despacho.
 * Autor: Lucas Lemanski Maiolli | Data: 06/05/2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

// Estrutura para enviar o intervalo pelo pipe conforme o fluxo sugerido [cite: 42]
typedef struct {
    int inicio;
    int fim;
} Intervalo;

int main() {
    int n, p;
    printf("Digite o numero de elementos (n) e de processos (p): ");
    if (scanf("%d %d", &n, &p) != 2) return 1;

    // Aloca SHM: 3 vetores de tamanho 'n' + 1 vetor de sinalização de tamanho 'p' [cite: 38, 39, 40]
    int shmid = shmget(IPC_PRIVATE, (3 * n + p) * sizeof(int), IPC_CREAT | 0666);
    int *base = (int *)shmat(shmid, NULL, 0);
    
    int *v1 = base;
    int *v2 = base + n;
    int *v3 = base + (2 * n);
    int *sinalizacao = base + (3 * n); 

    // Inicialização dos vetores [cite: 30]
    for(int i = 0; i < n; i++) {
        v1[i] = i + 1;
        v2[i] = 10;
        v3[i] = 0;
    }
    for(int i = 0; i < p; i++) sinalizacao[i] = 0;

    int pipes[p][2];
    for(int i = 0; i < p; i++) {
        pipe(pipes[i]); // Pai cria os pipes [cite: 36]
        
        if(fork() == 0) { // FILHO [cite: 37]
            Intervalo tarefa;
            read(pipes[i][0], &tarefa, sizeof(Intervalo)); // Filho bloqueia no pipe [cite: 41, 47]
            
            // Filho acessa SHM e faz o trabalho [cite: 43, 48]
            for(int k = tarefa.inicio; k < tarefa.fim; k++) {
                v3[k] = v1[k] + v2[k];
            }
            
            sinalizacao[i] = 1; // Filho avisa que acabou via SHM [cite: 48]
            shmdt(base);
            exit(0); // Filho encerra [cite: 44, 49]
        }
    }

    // PAI: Despacha intervalos de índices [cite: 27, 42]
    int chunk = n / p;
    for(int i = 0; i < p; i++) {
        Intervalo t;
        t.inicio = i * chunk;
        t.fim = (i == p - 1) ? n : (i + 1) * chunk;
        write(pipes[i][1], &t, sizeof(Intervalo));
    }

    // PAI: Aguarda todos os filhos via sinalização na SHM [cite: 45]
    for(int i = 0; i < p; i++) {
        while(sinalizacao[i] == 0) usleep(1000);
    }

    printf("Soma concluida com sucesso!\n");
    printf("Exemplo: V1[0](%d) + V2[0](%d) = V3[0](%d)\n", v1[0], v2[0], v3[0]);

    shmctl(shmid, IPC_RMID, NULL); // Limpeza da SHM
    return 0;
}