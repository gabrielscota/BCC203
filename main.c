#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "structs.c"
// #include "metodo2.c"

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
        printf("%ld\n", ftell(arq));
        // printf("%ld | %lf | %s | %s | %s\n", registro.numeroInscricao, registro.nota, registro.estado, registro.cidade, registro.curso);
        fwrite(&registro, sizeof(Registro), 1, temp);
    }
}

int main(int argc, char *argv[])
{
    // 1 = IBC, 2 = IBC Selecao por Substituicao, 3 = Quicksort externo
    int metodo = atoi(argv[1]);
    // Quantidade de registros do arquivo
    int quantidade = atoi(argv[2]);
    // Situação de ordem do arquivo, 1 = Ascendente, 2 = Decrescente, 3 = Aleatorio
    int situacao = atoi(argv[3]);
    // Argumento [-P] = Apresentar os dados dos alunos a serem ordenados e o resultado da ordenação realizada na tela
    int p;
    // Se o numero de argumentos for igual a 5 ele verifica se o quinto argumento é igual a "-P"
    if (argc == 5)
    {
        p = !strcmp(argv[4], "-P");
    }
    else
    {
        p = 0;
    }

    // Arquivo contendo todos os dados dos alunos
    FILE *arqProvao;

    // Arquivo temporario para conter apenas os N primeiros alunos
    FILE *arqTemporario;

    // Verifica se respeita todas as condições e ordena o arquivo temporario de acordo com a situacao
    if (quantidade > 0 && (situacao >= 1 && situacao <= 3) && (metodo >= 1 && metodo <= 3))
    {
        // Abre o arquivo provao no modo de leitura após passar todos os testes
        if ((arqProvao = fopen("PROVAO.txt", "r")) == NULL)
        {
            puts("> Nao foi possivel abrir o arquivo!\n");
        }

        // Abre o arquivo temporario que ira ser utilizado para a ordenação
        if ((arqTemporario = fopen("arqTemporario.bin", "w+b")) == NULL)
        {
            puts("> Nao foi possivel abrir o arquivo!\n");
        }

        // Metodo responsavel por criar o arquivo temporario com a quantidade de itens informada através do PROVAO
        criarArquivoTemporario(arqProvao, arqTemporario, quantidade);
        rewind(arqTemporario);

        // Ordenar o arquivo temporario de forma crescente
        if (situacao == 1)
        {
        }
        // Ordenar o arquivo temporario de forma descrescente
        else if (situacao == 2)
        {
        }
    }
    else if (metodo < 1 || metodo > 4)
    {
        printf("Informe um método válido!\n");
        return 0;
    }
    else if (quantidade <= 0)
    {
        printf("Informe uma quantide de registros valida!\n");
        return 0;
    }
    else if (situacao < 1 || situacao > 3)
    {
        printf("Informe uma situacao valida!\n");
        return 0;
    }

    // Variaveis responsaveis para analisar complexidade de cada metodo
    clock_t tempoInicialDeExecucao;
    clock_t tempoTotalDeExecucao;
    int numeroDeLeituras = 0;
    int numeroDeEscritas = 0;
    int numeroDeComparacoesEntreValores = 0;

    // Atraves do primeiro paramento da linha comando é selecionado qual metodo executar
    switch (metodo)
    {
    case 1:
        printf("> Intercalacao Balanceada de varios caminhos - (Metodo de ordenacao)\n");
        tempoInicialDeExecucao = clock();
        // Chama o metodo aqui
        tempoTotalDeExecucao = clock() - tempoInicialDeExecucao;
        break;
    case 2:
        printf("> Intercalacao Balanceada de varios caminhos - Selecao por substituicao\n");
        tempoInicialDeExecucao = clock();
        // metodo2(quantidade, arqTemporario, &numeroDeLeituras, &numeroDeEscritas, &numeroDeComparacoesEntreValores);
        tempoTotalDeExecucao = clock() - tempoInicialDeExecucao;
        break;
    case 3:
        printf("> Quicksort externo\n");
        tempoInicialDeExecucao = clock();
        // Chama o metodo aqui
        tempoTotalDeExecucao = clock() - tempoInicialDeExecucao;
        break;
    }

    printf("\n----------------------------------------------------------------------------\n");
    printf("\nNumero total de leituras da memoria externa para a interna: %d vezes!\n", numeroDeLeituras);
    printf("\nNumero total de escritas da memoria interna para a externa: %d vezes!\n", numeroDeEscritas);
    printf("\nNumero total de comparacoes entre os valores: %d vezes!\n", numeroDeComparacoesEntreValores);
    printf("\nTempo de execucao: %.4lf segundos!\n", ((double)tempoTotalDeExecucao) / CLOCKS_PER_SEC);
    printf("\n----------------------------------------------------------------------------\n");

    printf("\n");

    fclose(arqProvao);
    fclose(arqTemporario);

    return 0;
}
