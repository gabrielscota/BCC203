#ifndef STRUCTS_H
#define STRUCTS_H

#define FF 40 // Numero de fitas magneticas
#define F (FF/2)

typedef struct Registro {
    long int numeroInscricao;
    double nota;
    char estado[3];
    char cidade[51];
    char curso[31];
} Registro;

#endif