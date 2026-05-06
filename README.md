# Laboratorio-5-SO
Atividades práticas da matéria Sistemas Operacionais que tratam de comunicação de processos.

Essa atividade foi feita em dupla com o Lucas Lemanski, fiquei responsável pelos exercícios 1 e 2

## Exercício 1:
O exercício 1 foi a criação de um programa em que dois processos comuniquem entre si através de uma fifo, sendo um desses processos o escritor que lê strings digitadas pelo usuário e envia o para o segundo processo, sendo ele o leitor. O leitor recebe a string e exibe as seguintes informações sobre a string:

- A string digitada;
- O tamanho;
- O número de consoantes;
- O número de vogais;
- Quantidade de espaços.

## Exercício 2:
Esse exercício 2 pedia que o programa criado manipule arquivos enquanto estiver rodando e quando receber os sinais *SIGINT* ou *SIGTERM* o programa realiza uma graceful stop (armazena as informações pendentes e fecha o arquivo.)