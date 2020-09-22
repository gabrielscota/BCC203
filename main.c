#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    // 1 = IBC, 2 = IBC Selecao por Substituicao, 3 = Quicksort externo
    int metodo = atoi(argv[1]);
    // Quantidade de registros do arquivo
    int quantidade = atoi(argv[2]);
    // Situação de ordem do arquivo, 1 = Ascendente, 2 = Decrescente, 3 = Aleatorio
    int situacao = atoi(argv[3]);
    // Argumento [-P] = Apresentar os dados dos alunos a serem ordenados e o resultado da ordenação realizada na tela
    // implementar

    FILE *arquivoGerado;
    // Variável responsavel para receber o arquivo gerado
    if (quantidade > 0 && (situacao >= 1 && situacao <= 3) && (metodo >= 1 && metodo <= 3))
    {
        // Ordenar o provao de forma crescente
        if(situacao == 1){
            
        } 
        // Ordenar o provao de forma descrescente
        else if(situacao == 2){

        }
        // Caso seja aleatorio colocar no arquivo gerado apenas N registros
        else {

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


    // número de transferências (leitura) de registros da memória externa para a memória interna
    // número de transferências (escrita) de registros da memória interna para a memória externa
    // número de comparações entre valores do campo de ordenação dos registros
    // tempo de execução (tempo do término de execução menos o tempo do início de execução)


    // Atraves do primeiro paramento da linha comando é selecionado qual metodo executar
    switch (metodo)
    {
    case 1:
        printf("> Intercalacao Balanceada de varios caminhos - (Metodo de ordenacao)\n");
        clock_t tempoTotalDeExecucao;
        int numeroDeLeituras = 0;
        int numeroDeEscritas = 0;
        int numeroDeComparacoesEntreValores = 0;
        // Chama o metodo aqui
        printf("\n----------------------------------------------------------------------------\n");
        printf("\nNumero total de leituras da memoria externa para a interna: %d vezes!\n", numeroDeLeituras);
        printf("\nNumero total de escritas da memoria interna para a externa: %d vezes!\n", numeroDeEscritas);
        printf("\nNumero total de comparacoes entre os valores: %d vezes!\n", numeroDeComparacoesEntreValores);
        printf("\nTempo de execucao: %.4lf segundos!\n", ((double)tempoTotalDeExecucao) / CLOCKS_PER_SEC);
        printf("\n----------------------------------------------------------------------------\n");
        break;
    case 2:
        printf("> Intercalacao Balanceada de varios caminhos - Selecao por substituicao\n");
        clock_t tempoTotalDeExecucao;
        int numeroDeLeituras = 0;
        int numeroDeEscritas = 0;
        int numeroDeComparacoesEntreValores = 0;
        // Chama o metodo aqui
        printf("\n----------------------------------------------------------------------------\n");
        printf("\nNumero total de leituras da memoria externa para a interna: %d vezes!\n", numeroDeLeituras);
        printf("\nNumero total de escritas da memoria interna para a externa: %d vezes!\n", numeroDeEscritas);
        printf("\nNumero total de comparacoes entre os valores: %d vezes!\n", numeroDeComparacoesEntreValores);
        printf("\nTempo de execucao: %.4lf segundos!\n", ((double)tempoTotalDeExecucao) / CLOCKS_PER_SEC);
        printf("\n----------------------------------------------------------------------------\n");
        break;
    case 3:
        printf("> Quicksort externo\n");
        clock_t tempoTotalDeExecucao;
        int numeroDeLeituras = 0;
        int numeroDeEscritas = 0;
        int numeroDeComparacoesEntreValores = 0;
        // Chama o metodo aqui
        printf("\n----------------------------------------------------------------------------\n");
        printf("\nNumero total de leituras da memoria externa para a interna: %d vezes!\n", numeroDeLeituras);
        printf("\nNumero total de escritas da memoria interna para a externa: %d vezes!\n", numeroDeEscritas);
        printf("\nNumero total de comparacoes entre os valores: %d vezes!\n", numeroDeComparacoesEntreValores);
        printf("\nTempo de execucao: %.4lf segundos!\n", ((double)tempoTotalDeExecucao) / CLOCKS_PER_SEC);
        printf("\n----------------------------------------------------------------------------\n");
        break;
    }

    printf("\n");

    return 0;
}
