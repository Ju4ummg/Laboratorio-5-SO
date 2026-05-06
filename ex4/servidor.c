/*
 * Funcionalidade: Servidor de tradução pt-en usando Sockets UNIX.
 * Cada conexão gera um processo filho para processamento.
 * Autor: Lucas Lemanski Maiolli | Data: 06/05/2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define SOCK_PATH "/tmp/tradutor_socket"

void processar_traducao(int socket_cli) {
    char buffer[128];
    while(read(socket_cli, buffer, 128) > 0) {
        // Se receber "NO-NO", encerra o diálogo [cite: 54]
        if(strncmp(buffer, "NO-NO", 5) == 0) break;

        printf("Solicitação recebida: %s\n", buffer);

        // Lógica simples de tradução [cite: 51, 53]
        if(strcmp(buffer, "pt-en:cachorro") == 0) 
            write(socket_cli, "dog", 4);
        else if(strcmp(buffer, "pt-en:gato") == 0)
            write(socket_cli, "cat", 4);
        else 
            write(socket_cli, "ERROR: UNKNOWN", 15); // [cite: 52]
    }
    close(socket_cli);
    exit(0);
}

int main() {
    int sock_serv, sock_cli;
    struct sockaddr_un endereco;

    sock_serv = socket(AF_UNIX, SOCK_STREAM, 0);
    endereco.sun_family = AF_UNIX;
    strcpy(endereco.sun_path, SOCK_PATH);
    unlink(SOCK_PATH);

    bind(sock_serv, (struct sockaddr *)&endereco, sizeof(endereco));
    listen(sock_serv, 5);

    printf("Servidor de Tradução Online...\n");

    while(1) {
        sock_cli = accept(sock_serv, NULL, NULL);
        if(fork() == 0) { // Delega para processo filho [cite: 54]
            processar_traducao(sock_cli);
        }
        close(sock_cli); // Pai fecha o socket duplicado
    }
    return 0;
}