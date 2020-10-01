#ifndef STRUCTS_H
#define STRUCTS_H

// Numero de fitas de armazenamento externo
#define FF 40
// Numero de fitas para ser armazenado memória interna disponivel
#define F (FF/2)

// Estrutura do registro que está no arquivo do PROVAO.txt, foi adicionado 1 caractere nas strings por conta do /0
typedef struct Registro {
    long int numeroInscricao;
    double nota;
    char estado[3];
    char cidade[51];
    char curso[31];
} Registro;

#endif