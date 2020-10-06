#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "listaMetodo1e2.c"

// --- Inicio da estrutura do HeapSort --- //

int verificaMenor(Registro registros[], int from[], int ativas[], int a, int b)
{
    if (ativas[from[a] % F] == -1)
    {
        return 0;
    }
    else if (ativas[from[b] % F] == -1)
    {
        return 1;
    }
    else
    {
        return (registros[b].nota < registros[a].nota);
    }
}

void heapsortRegistros(Registro registros[], int ativas[], int from[], int n, int *numeroDeComparacoes)
{
    int isHeap;
    do
    {
        isHeap = 1;
        for (int i = 0; i < n / 2; i++)
        {
            (*numeroDeComparacoes) += 4;
            if (2 * i + 2 < n && verificaMenor(registros, from, ativas, 2 * i + 2, 2 * i + 1))
            {
                if (verificaMenor(registros, from, ativas, 2 * i + 2, i))
                {
                    Registro registroAux = registros[i];
                    registros[i] = registros[2 * i + 2];
                    registros[2 * i + 2] = registroAux;

                    int aux = from[i];
                    from[i] = from[2 * i + 2];
                    from[2 * i + 2] = aux;
                    isHeap = 0;
                }
            }
            else
            {
                if (verificaMenor(registros, from, ativas, 2 * i + 1, i))
                {
                    Registro registroAux = registros[i];
                    registros[i] = registros[2 * i + 1];
                    registros[2 * i + 1] = registroAux;

                    int aux = from[i];
                    from[i] = from[2 * i + 1];
                    from[2 * i + 1] = aux;
                    isHeap = 0;
                }
            }
        }
    } while (!isHeap);
}

int verificaMenorMarcados(Registro registros[], int registrosMarcados[], int a, int b)
{
    if (registrosMarcados[a % F] != 0)
    {
        return 0;
    }
    else if (registrosMarcados[b % F] != 0)
    {
        return 1;
    }
    else
    {
        return (registros[b].nota < registros[a].nota);
    }
}

void heapsortRegistrosMarcados(Registro registros[], int registrosMarcados[], int n, int *numeroDeComparacoes)
{
    int isHeap;
    do
    {
        isHeap = 1;
        for (int i = 0; i < n / 2; i++)
        {
            (*numeroDeComparacoes) += 4;
            if (2 * i + 2 < n && verificaMenorMarcados(registros, registrosMarcados, 2 * i + 2, 2 * i + 1))
            {
                if (verificaMenorMarcados(registros, registrosMarcados, 2 * i + 2, i))
                {
                    Registro registroAux = registros[i];
                    registros[i] = registros[2 * i + 2];
                    registros[2 * i + 2] = registroAux;

                    int aux = registrosMarcados[i];
                    registrosMarcados[i] = registrosMarcados[2 * i + 2];
                    registrosMarcados[2 * i + 2] = aux;
                    isHeap = 0;
                }
            }
            else
            {
                if (verificaMenorMarcados(registros, registrosMarcados, 2 * i + 1, i))
                {
                    Registro registroAux = registros[i];
                    registros[i] = registros[2 * i + 1];
                    registros[2 * i + 1] = registroAux;

                    int aux = registrosMarcados[i];
                    registrosMarcados[i] = registrosMarcados[2 * i + 1];
                    registrosMarcados[2 * i + 1] = aux;
                    isHeap = 0;
                }
            }
        }
    } while (!isHeap);
}

// --- Fim da estrutura do HeapSort --- //

// --- Inicio da estrutura para a intercalação dos blocos ordenados --- //

// Função para verificar se todos os registros da memória estao marcados, caso estejam é retornado o valor 1
int verificarTodosMarcados(int marcados[], int n)
{
    for (int i = 0; i < n; i++)
    {
        if (marcados[i] == 0)
        {
            return 0;
        }
    }
    return 1;
}

// Retorna 1 se tem elementos na memoria (se tem algum elemento que nao está marcado com '-1')
int temElementoNaMemoria(int marcados[], int n)
{
    for (int i = 0; i < n; i++)
    {
        if (marcados[i] != -1)
        {
            return 1;
        }
    }
    return 0;
}

// Função para verificar se todas as fitas foram percorridas
int percorreuTodasFitas(FILE **fitas, int elementos[], int n, int inicio, int *numeroDeLeituras)
{
    for (int i = 0; i < n; i++)
    {
        (*numeroDeLeituras)++;
        if (ftell(fitas[inicio + i]) / sizeof(Registro) < (unsigned)elementos[inicio + i])
        {
            return 0;
        }
    }
    return 1;
}

// Função para verificar se todos os blocos estão esgotados, caso esteja é retornado 1
int todosBlocosEsgotados(int ativas[], int n, int *numeroComparacoes)
{
    for (int i = 0; i < n; i++)
    {
        (*numeroComparacoes)++;
        if (ativas[i] != -1)
        {
            return 0;
        }
    }
    return 1;
}

// Método responsável por intercalar os bloco de cada fita de entrada e saida até que reste só uma fita de saída preenchida
void intercalarBlocos(int n, FILE *temp, FILE **fitas, int numeroDeRegistrosNaFita[FF], Lista blocos[FF], int *numeroDeLeituras, int *numeroDeEscritas, int *numeroComparacoes)
{
    // Vetor que armazena os registros do bloco na memória interna
    Registro *memoriaInterna = malloc(F * sizeof(Registro));

    // Vetor de inteiros que referencia para cada registro da memória de qual fita ele veio
    int fitaDoRegistro[F];

    // Vetor responsável por armazenar a situação do bloco que está sendo lido de cada fita de entrada
    int ativas[F];

    // Se for igual a '1', as primeiras F fitas são as de entrada e as outras de saida
    int distribuicaoFitas = 1;

    // Variável responsável por armazenar o indice da fita de saida alvo
    int fitaAlvo;
    for (int i = 0; i < FF; i++)
    {
        rewind(fitas[i]);
        (*numeroDeEscritas)++;
    }

    // Variável utilizada para armazenar o indice da unica fita preenchida, caso tenha mais de uma fita preenchida é retornado -1
    int indiceDaUnicaFitaPreenchida;

    // Buscando se tem alguma fita preenchida ou mais de uma
    int preenchida = -1;
    for (int i = 0; i < F; i++)
    {
        if (numeroDeRegistrosNaFita[F * distribuicaoFitas + i] > 0)
        {
            if (preenchida == -1)
            {
                preenchida = i;
            }
            else
            {
                indiceDaUnicaFitaPreenchida = -1;
            }
        }
        (*numeroComparacoes)++;
    }
    if (preenchida == -1)
    {
        indiceDaUnicaFitaPreenchida = preenchida;
    }
    else
    {
        indiceDaUnicaFitaPreenchida = F * distribuicaoFitas + preenchida;
    }

    // Enquanto estiver mais de uma fita preenchida
    while (indiceDaUnicaFitaPreenchida == -1)
    {
        // Define a fita alvo como a primeira fita de saída de acordo com a situacao de distribuição
        fitaAlvo = F * distribuicaoFitas;
        int bloco = 0;

        // Executa o while enquanto todas as fitas de entrada não forem percorridas
        while (percorreuTodasFitas(fitas, numeroDeRegistrosNaFita, F, F * (!distribuicaoFitas), numeroDeLeituras) == 0)
        {
            adicionarItemNoInicioDaLista(&blocos[fitaAlvo], ftell(fitas[fitaAlvo]) / sizeof(Registro));

            // Vetor responsável por armazenar o indice onde termina o bloco de cada fita
            int indiceOndeTerminaOsBlocos[F];
            for (int i = 0; i < F; i++)
            {
                if (tamanhoDaLista(&blocos[(F * !distribuicaoFitas) + i]) > bloco + 1)
                {
                    indiceOndeTerminaOsBlocos[i] = posicaoNaLista(&blocos[(F * !distribuicaoFitas) + i], bloco + 1);
                }
                else
                {
                    indiceOndeTerminaOsBlocos[i] = numeroDeRegistrosNaFita[(F * !distribuicaoFitas) + i];
                }
            }

            // Pega o primeiro elemento de cada fita de entrada
            for (int i = 0; i < F; i++)
            {
                // Verificação para saber se tem registro no bloco
                if (ftell(fitas[(F * !distribuicaoFitas) + i]) / sizeof(Registro) < indiceOndeTerminaOsBlocos[i])
                {
                    // É armazenado na memória interna o primeiro registro
                    fread(&memoriaInterna[i], sizeof(Registro), 1, fitas[(F * !distribuicaoFitas) + i]);
                    (*numeroDeLeituras)++;

                    // Verificação para saber esse registro lido foi o ultimo do bloco
                    if (ftell(fitas[(F * !distribuicaoFitas) + i]) / sizeof(Registro) >= indiceOndeTerminaOsBlocos[i])
                    {
                        ativas[i] = 0;
                    }
                    else
                    {
                        ativas[i] = 1;
                    }
                }
                else
                {
                    ativas[i] = -1;
                }
                fitaDoRegistro[i] = i + (!distribuicaoFitas * F);
            }

            // Executa o laço enquanto um bloco não estiver esgotado
            while (todosBlocosEsgotados(ativas, F, numeroComparacoes) == 0)
            {
                // Realiza a ordenação da memória interna
                heapsortRegistros(memoriaInterna, ativas, fitaDoRegistro, F, numeroComparacoes);

                // Pega o menor registro e armazena na memória interna
                fwrite(&memoriaInterna[0], sizeof(Registro), 1, fitas[fitaAlvo]);
                (*numeroDeEscritas)++;

                numeroDeRegistrosNaFita[fitaAlvo]++;

                // Verifica se a fita do menor registro está ativa
                if (ativas[fitaDoRegistro[0] % F])
                {
                    // Caso esteja ativa, o próximo registro é armazenado na memória interna
                    fread(&memoriaInterna[0], sizeof(Registro), 1, fitas[fitaDoRegistro[0]]);
                    (*numeroDeLeituras)++;

                    // Caso tenha sido o utlimo registro a ser lido, a fita se torna inativa
                    if (ftell(fitas[fitaDoRegistro[0]]) / sizeof(Registro) == indiceOndeTerminaOsBlocos[fitaDoRegistro[0] % F])
                    {
                        ativas[fitaDoRegistro[0] % F] = 0;
                    }
                    (*numeroDeLeituras)++;
                }
                else
                {
                    // Caso não esteja ativa, significa que o bloco esgotou
                    ativas[fitaDoRegistro[0] % F] = -1;
                }
            }

            // Define a próxima fita alvo
            fitaAlvo = (fitaAlvo % F + 1) % F + F * distribuicaoFitas;
            bloco++;
        }

        for (int i = 0; i < F; i++)
        {
            // Faz o rewind nas fitas de entrada e saida para executar a próxima intercalação
            rewind(fitas[F * distribuicaoFitas + i]);
            rewind(fitas[F * (!distribuicaoFitas) + i]);
            (*numeroDeEscritas)+=2;

            // Define o número de registros nas fitas de entrada como 0
            numeroDeRegistrosNaFita[F * (!distribuicaoFitas) + i] = 0;

            // Reseta os blocos das fitas de entradas
            limparLista(&blocos[F * (!distribuicaoFitas) + i]);
        }

        // Agora a distruibuição das fitas de entrada são trocadas com as fitas de saídas
        distribuicaoFitas = !distribuicaoFitas;

        // Caso reste apenas uma única fita preenchida significa que a intercalção chegou ao fim
        preenchida = -1;
        for (int i = 0; i < F; i++)
        {
            if (numeroDeRegistrosNaFita[F * (!distribuicaoFitas) + i] > 0)
            {
                if (preenchida == -1)
                {
                    preenchida = i;
                }
                else
                {
                    indiceDaUnicaFitaPreenchida = -1;
                }
            }
            (*numeroComparacoes)++;
        }
        if (preenchida == -1)
        {
            indiceDaUnicaFitaPreenchida = preenchida;
        }
        else
        {
            indiceDaUnicaFitaPreenchida = F * (!distribuicaoFitas) + preenchida;
        }
    }

    rewind(fitas[indiceDaUnicaFitaPreenchida]);
    rewind(temp);
    (*numeroDeEscritas)+=2;

    // Método responsável por armazenar todos os registros da ultima fita preenchida no arquivo temporário
    Registro *registro = malloc(1 * sizeof(Registro));
    for (int i = 0; i < n; i++)
    {
        // joga todos os elementos da fita f na fita principal
        fread(&registro, sizeof(Registro), 1, fitas[indiceDaUnicaFitaPreenchida]);
        (*numeroDeLeituras)++;
        fwrite(&registro, sizeof(Registro), 1, temp);
        (*numeroDeEscritas)++;
    }
}

// --- Fim da estrutura para a intercalação dos blocos ordenados --- //

// --- Inicio da estrutura para a geração dos blocos ordenados --- //

// Método responsável por pegar o menor registro da mamória na fita alvo até pegar todos os registros do arquivo temporário
void gerarBlocosOrdenados(int quantidade, FILE *arqTemporario, FILE **fitas, int numeroDeRegistrosNaFita[FF], Lista blocos[FF], int *numeroDeLeituras, int *numeroDeEscritas, int *numeroComparacoes)
{
    // Variavel responsavel por definir o tamanho disponível para ordenação dos blocos na memória interna
    int tamanhoDaMemoriaPrincipalDisponivel = F;
    // Vetor que armazena os registros do bloco na memória interna
    Registro *memoriaInterna = malloc(tamanhoDaMemoriaPrincipalDisponivel * sizeof(Registro));
    // Vetor para representar os registros marcados
    int *registrosMarcados = calloc(tamanhoDaMemoriaPrincipalDisponivel, sizeof(int));

    // pega a primeira fita de entrada como fita alvo
    int fitaSelecionada = 0;
    // Variável responsável por guardar o proximo registro lido do arqTemporario antes que ele seja jogado na memória
    Registro proximoRegistro;

    // Slide 14 - Pega os 'tamanhoDaMemoriaPrincipalDisponivel' primeiros registros do arquivo e joga na memoria interna
    fread(memoriaInterna, sizeof(Registro), tamanhoDaMemoriaPrincipalDisponivel, arqTemporario);
    (*numeroDeLeituras)++;

    // Enquanto nao percorrer todo o arquivo ou houver elementos na memoria
    while ((ftell(arqTemporario) / sizeof(Registro) < quantidade) || temElementoNaMemoria(registrosMarcados, tamanhoDaMemoriaPrincipalDisponivel))
    {
        (*numeroDeLeituras)++;
        for (int i = 0; i < tamanhoDaMemoriaPrincipalDisponivel; i++)
        {
            if (registrosMarcados[i] == 1)
            {
                // Desmarca todos que estavam marcados na memoria
                registrosMarcados[i] = 0;
            }
            (*numeroComparacoes)++;
        }

        adicionarItemNoInicioDaLista(&blocos[fitaSelecionada], numeroDeRegistrosNaFita[fitaSelecionada]); // adiciona um novo bloco para a fita alvo

        // Enquanto houver um elemento na memoria não-marcado
        while (verificarTodosMarcados(registrosMarcados, tamanhoDaMemoriaPrincipalDisponivel) == 0)
        {
            // Faz a ordenação da memória em busca do menor registro
            heapsortRegistrosMarcados(memoriaInterna, registrosMarcados, tamanhoDaMemoriaPrincipalDisponivel, numeroComparacoes);

            // Escreve o menor registro da memoria da fita alvo
            fwrite(&memoriaInterna[0], sizeof(Registro), 1, fitas[fitaSelecionada]);
            (*numeroDeEscritas)++;
            numeroDeRegistrosNaFita[fitaSelecionada]++;

            if (ftell(arqTemporario) / sizeof(Registro) < quantidade)
            {
                fread(&proximoRegistro, sizeof(Registro), 1, arqTemporario);
                (*numeroDeLeituras)++;

                if (proximoRegistro.nota > memoriaInterna[0].nota)
                {
                    registrosMarcados[0] = 1;
                }
                (*numeroComparacoes)++;

                memoriaInterna[0] = proximoRegistro;
            }
            else
            {
                // Marca o primeiro elemento (nao tem elemento na memoria)
                registrosMarcados[0] = -1;
            }
        }
        // Percorre somente entre as fitas de entrada (as F primeiras do vetor de fitas)
        fitaSelecionada = (fitaSelecionada + 1) % (FF / 2);
    }
}

// --- Fim da estrutura para a geração dos blocos ordenados --- //

// Função principal para realizar a ordenação utilizando intercalação de varios caminhos
int metodo2(int quantidade, FILE *arqTemporario, int *numeroDeLeituras, int *numeroDeEscritas, int *numeroDeComparacoes)
{
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

    // Fase de criação dos blocos ordenados, quebrando o arquivo em blocos do tamanho da memoria
    gerarBlocosOrdenados(quantidade, arqTemporario, fitas, numeroDeRegistrosNaFita, blocos, numeroDeLeituras, numeroDeEscritas, numeroDeComparacoes);
    // Fase de intercalação dos blocos com as fitas
    intercalarBlocos(quantidade, arqTemporario, fitas, numeroDeRegistrosNaFita, blocos, numeroDeLeituras, numeroDeEscritas, numeroDeComparacoes);

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

    return 0;
}
