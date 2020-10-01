#define FF 40 // Numero de fitas magneticas
#define F (FF/2)

typedef struct Registro {
    long int numeroInscricao;
    double nota;
    char estado[3];
    char cidade[51];
    char curso[31];
} Registro;

void criarArquivoTemporario(FILE *arq, FILE *temp, int n)
{
    Registro registro;
    for (int i = 0; i < n; i++)
    {
        // // Fazendo a leitura do numero de inscrição, nota e um ' ' no final para começar a ler o proximo dado
        fscanf(arq, "%ld %lf ", &registro.numeroInscricao, &registro.nota);
        fgets(registro.estado, 3, arq);
        fseek(arq, 1, 1);
        fgets(registro.cidade, 50, arq);
        fseek(arq, 2, 1);
        fgets(registro.curso, 30, arq);
        // printf("%ld | %lf | %s | %s | %s\n", registro.numeroInscricao, registro.nota, registro.estado, registro.cidade, registro.curso);
        fwrite(&registro, sizeof(Registro), 1, temp);
    }
}