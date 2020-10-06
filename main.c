#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "structs.h"
#include "listaMetodo1e2.c"

#include "metodo1.c"
#include "metodo2.c"
#include "metodo3.c"

// Função responsável por criar o arquivo temporario que irá ser utilizado para ser ordenado
void criarArquivoTemporario(FILE *arqProvao, FILE *arqTemporario, int quantidade)
{
    Registro registro;
    for (int i = 0; i < quantidade; i++)
    {
        // Fazendo a leitura do numero de inscrição, nota e um ' ' no final para começar a ler o proximo dado
        fscanf(arqProvao, "%ld %lf ", &registro.numeroInscricao, &registro.nota);
        // Lê o estado e da o fseek para pular o espaço
        fgets(registro.estado, 3, arqProvao);
        fseek(arqProvao, 1, 1);
        // Lê a cidade e da o fseek para pular o espaço
        fgets(registro.cidade, 50, arqProvao);
        fseek(arqProvao, 2, 1);
        // Lê o curso
        fgets(registro.curso, 30, arqProvao);
        fwrite(&registro, sizeof(Registro), 1, arqTemporario);
    }
}

int main(int argc, char *argv[])
{
    // 1 = IBVC, 2 = IBVC Seleção por Substituição, 3 = Quicksort externo
    int metodo = atoi(argv[1]);
    // Quantidade de registros do arquivo
    int quantidade = atoi(argv[2]);
    // Situação de ordem do arquivo, 1 = Ascendente, 2 = Decrescente, 3 = Aleatório
    int situacao = atoi(argv[3]);
    // Argumento [-P] = Apresentar no console os dados dos alunos a serem ordenados e o resultado da ordenação realizado
    int imprimirDados;
    // Se o número de argumentos for igual a 5 ele verifica se o quinto argumento é igual a "-P", se for verdadeiro a função strcmp retorna 1
    if (argc == 5)
    {
        imprimirDados = !strcmp(argv[4], "-P");
    }
    else
    {
        imprimirDados = 0;
    }

    // Arquivo contendo todos os dados dos alunos
    FILE *arqProvao;

    // Arquivo temporário para conter apenas os N primeiros alunos
    FILE *arqTemporario;

    // Verifica se respeita todas as condições e ordena o arquivo temporário de acordo com a situção
    if (quantidade > 0 && (situacao >= 1 && situacao <= 3) && (metodo >= 1 && metodo <= 3))
    {
        // Abre o arquivo PROVAO.txt no modo de leitura após passar todos os testes
        if ((arqProvao = fopen("PROVAO.txt", "r")) == NULL)
        {
            puts("> Nao foi possivel abrir o arquivo!\n");
        }

        // Abre o arquivo temporário que irá ser utilizado para a ordenação
        if ((arqTemporario = fopen("arqTemporario.bin", "w+b")) == NULL)
        {
            puts("> Nao foi possivel abrir o arquivo!\n");
        }

        // Método responsável por criar o arquivo temporário com a quantidade de itens informada através do arquivo PROVAO.txt
        criarArquivoTemporario(arqProvao, arqTemporario, quantidade);
        rewind(arqTemporario);

        // ------- Parte reutilizada do metodo 2 para ordenar o arquivo de acordo com a situação -------

        // Criando um vetor para armazenar os ponteiros das fitas
        FILE **fitas = malloc(FF * sizeof(FILE *));
        for (int i = 0; i < FF; i++)
        {
            char nomeFita[8];
            // Armazena no buffer do nome a concatenação do F com o inteiro i
            sprintf(nomeFita, "F%d.bin", i);
            fitas[i] = fopen(nomeFita, "w+b");
        }

        // Vetor responsavel por amazernar em cada posicao o numero de registros dentro de cada fita respectivamente, inicializando com 0
        int *numeroDeRegistrosNaFita = calloc(FF, sizeof(int));

        // Vetor de uma LinkedList para armazenar os blocos de uma fita, onde cada posicao 'i' desse vetor corresponde à fita 'i'
        Lista blocos[FF];
        for (int i = 0; i < FF; i++)
        {
            blocos[i].primeiroItem = NULL;
        }

        int temp1, temp2, temp3;

        // Ordenar o arquivo temporario de forma crescente
        if (situacao == 1)
        {
            // Fase de criação dos blocos ordenados, quebrando o arquivo em blocos do tamanho da memoria
            gerarBlocosOrdenados(quantidade, arqTemporario, fitas, numeroDeRegistrosNaFita, blocos, &temp1, &temp2, &temp3);
            // Fase de intercalação dos blocos com as fitas
            intercalarBlocos(quantidade, arqTemporario, fitas, numeroDeRegistrosNaFita, blocos, &temp1, &temp2, &temp3);
            Registro r1, r2;
            for (int i = 0; i < quantidade / 2; i++)
            {
                fseek(arqTemporario, sizeof(Registro) * i, 0);
                fread(&r1, sizeof(Registro), 1, arqTemporario);
                fseek(arqTemporario, -sizeof(Registro) * (i + 1), 2);
                fread(&r2, sizeof(Registro), 1, arqTemporario);

                fseek(arqTemporario, sizeof(Registro) * i, 0);
                fwrite(&r2, sizeof(Registro), 1, arqTemporario);
                fseek(arqTemporario, -sizeof(Registro) * (i + 1), 2);
                fwrite(&r1, sizeof(Registro), 1, arqTemporario);
            }
            rewind(arqTemporario);
        }
        // Ordenar o arquivo temporario de forma descrescente
        else if (situacao == 2)
        {
            // Fase de criação dos blocos ordenados, quebrando o arquivo em blocos do tamanho da memoria
            gerarBlocosOrdenados(quantidade, arqTemporario, fitas, numeroDeRegistrosNaFita, blocos, &temp1, &temp2, &temp3);
            // Fase de intercalação dos blocos com as fitas
            intercalarBlocos(quantidade, arqTemporario, fitas, numeroDeRegistrosNaFita, blocos, &temp1, &temp2, &temp3);
            rewind(arqTemporario);
        }
        for (int i = 0; i < FF; i++)
        {
            fclose(fitas[i]);
        }
        for (int i = 0; i < FF; i++)
        {
            char nomeFita[8];
            // Armazena no buffer do nome a concatenação do F com o inteiro i
            sprintf(nomeFita, "F%d.bin", i);
            remove(nomeFita);
        }

        // ------- Fim da parte reutilizada do metodo 2 para ordenar o arquivo de acordo com a situação -------
    }
    // Verificação para saber se foi digitado um método válido
    else if (metodo < 1 || metodo > 4)
    {
        printf("Informe um método válido!\n");
        return 0;
    }
    // Verificação para saber se foi digitado uma quantidade positiva de registros a serem ordenados
    else if (quantidade <= 0)
    {
        printf("Informe uma quantide de registros valida!\n");
        return 0;
    }
    // Verificação para saber se foi digitado uma situação válida
    else if (situacao < 1 || situacao > 3)
    {
        printf("Informe uma situacao valida!\n");
        return 0;
    }

    // Caso a variável 'imprimirDados' seja igual a 1 é impresso no console os dados antes de serem ordenados
    if (imprimirDados)
    {
        printf("\n");
        printf("> Arquivo temporario ANTES dos dados serem ordenados:\n\n");
        for (int i = 0; i < quantidade; i++)
        {
            Registro registro;
            fread(&registro, sizeof(Registro), 1, arqTemporario);
            printf("\t%d - %ld\t%.2lf\t%s\t%s\t%s\n", i + 1, registro.numeroInscricao, registro.nota, registro.estado, registro.cidade, registro.curso);
        }
    }
    rewind(arqTemporario);

    // Variáveis responsáveis para analisar complexidade de cada método
    clock_t tempoInicialDeExecucao;
    clock_t tempoTotalDeExecucao;
    int numeroDeLeituras = 0;
    int numeroDeEscritas = 0;
    int numeroDeComparacoesEntreValores = 0;

    // Através do primeiro paramento da linha comando é selecionado qual metodo executar
    switch (metodo)
    {
    // Método 1: Intercalação Balanceada de Vários Caminhos - (Método de ordenação)
    case 1:
        printf("\n> Intercalacao Balanceada de varios caminhos - (Metodo de ordenacao)\n");
        tempoInicialDeExecucao = clock();
        metodo1(quantidade, arqTemporario, &numeroDeLeituras, &numeroDeEscritas, &numeroDeComparacoesEntreValores);
        tempoTotalDeExecucao = clock() - tempoInicialDeExecucao;
        break;
    // Método 2: Intercalação Balanceada de Vários Caminhos - Seleção por Substituição
    case 2:
        printf("\n> Intercalacao Balanceada de varios caminhos - Selecao por substituicao\n");
        tempoInicialDeExecucao = clock();
        metodo2(quantidade, arqTemporario, &numeroDeLeituras, &numeroDeEscritas, &numeroDeComparacoesEntreValores);
        tempoTotalDeExecucao = clock() - tempoInicialDeExecucao;
        break;
    // Método 3: Quicksort Externo
    case 3:
        printf("\n> Quicksort externo\n");
        tempoInicialDeExecucao = clock();

        FILE *ArqLEs = fopen("tempQuickSort.bin", "r+b");
        FILE *ArqEi = fopen("tempQuickSort.bin", "r+b");
        QuickSortExterno(&arqTemporario, &ArqEi, &ArqLEs, 1, quantidade, &numeroDeEscritas, &numeroDeComparacoesEntreValores);
        fflush(arqTemporario);
        fclose(ArqEi);
        fclose(ArqLEs);

        tempoTotalDeExecucao = clock() - tempoInicialDeExecucao;
        break;
    }

    rewind(arqTemporario);
    // Caso a variável 'imprimirDados' seja igual a 1 é impresso no console os dados apos serem ordenados
    if (imprimirDados)
    {
        printf("\n");
        printf("\t> Arquivo temporario DEPOIS dos dados serem ordenados\n\n");
        for (int i = 0; i < quantidade; i++)
        {
            Registro registro;
            fread(&registro, sizeof(Registro), 1, arqTemporario);
            printf("\t\t%d - %ld\t%.2lf\t%s\t%s\t%s\n", i + 1, registro.numeroInscricao, registro.nota, registro.estado, registro.cidade, registro.curso);
        }
    }

    printf("\n\t> Numero total de leituras da memoria externa para a interna: %d vezes!\n", numeroDeLeituras);
    printf("\n\t> Numero total de escritas da memoria interna para a externa: %d vezes!\n", numeroDeEscritas);
    printf("\n\t> Numero total de comparacoes entre os valores: %d vezes!\n", numeroDeComparacoesEntreValores);
    printf("\n\t> Tempo de execucao: %.4lf segundos!\n", ((double)tempoTotalDeExecucao) / CLOCKS_PER_SEC);

    printf("\n");

    remove("tempQuickSort.bin");
    
    fclose(arqProvao);
    fclose(arqTemporario);

    return 0;
}
