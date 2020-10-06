#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "listaMetodo1e2.c"

//*******************QuickSort***************************//

int partition(Registro a[], int l, int r, int *comp)
{
	int i, j;
	Registro pivot, t;
	pivot = a[l];
	i = l;
	j = r + 1;

	while (1)
	{
		do
			++i;
		while (a[i].nota >= pivot.nota && i <= r);
		do
			--j;
		while (a[j].nota < pivot.nota);
		if (i >= j)
			break;

		t = a[i];
		a[i] = a[j];
		a[j] = t;

		(*comp) += 2;
	}
	t = a[l];
	a[l] = a[j];
	a[j] = t;

	return j;
}

void quickSort(Registro a[], int l, int r, int *comp)
{
	int j;
	if (l < r)
	{
		(*comp)++;
		j = partition(a, l, r, comp);
		quickSort(a, l, j - 1, comp);
		quickSort(a, j + 1, r, comp);
	}
}

void sort(Registro v[], int n, int *comp)
{
	quickSort(v, 0, n - 1, comp);
}

//*******************QuickSort***************************//

//********************heapSort***************************//

void swapregistro(Registro v[], int a, int b)
{
	Registro aux = v[a];
	v[a] = v[b];
	v[b] = aux;
}

void swapInt(int v[], int a, int b)
{
	int aux = v[a];
	v[a] = v[b];
	v[b] = aux;
}

short ehMenor(Registro v[], int from[], short ativas[], int a, int b)
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

		return (v[b].nota < v[a].nota);
	}
}

void heapsort(Registro v[], short ativas[], int from[], int n, int *comp)
{
	short heap; // é heap?
	do
	{
		heap = 1;
		for (int i = 0; i < n / 2; i++)
		{
			(*comp) += 4;
			if (2 * i + 2 < n && ehMenor(v, from, ativas, 2 * i + 2, 2 * i + 1))
			{
				if (ehMenor(v, from, ativas, 2 * i + 2, i))
				{
					swapregistro(v, i, 2 * i + 2);
					swapInt(from, i, 2 * i + 2);
					heap = 0;
				}
			}
			else
			{
				if (ehMenor(v, from, ativas, 2 * i + 1, i))
				{
					swapregistro(v, i, 2 * i + 1);
					swapInt(from, i, 2 * i + 1);

					heap = 0;
				}
			}
		}
	} while (!heap);
}

//****************HeapSort********************//

// Procura por uma unica fita que tenha registros
// Condição de parada
// Retorna -1 se mais de uma ou nenhuma fita esteja preenchida.
int CondicaodaFita(int nQtd_elementos[], int n, int inicio, int *comp)
{
	int preenchida = -1;
	for (int i = 0; i < n; i++)
	{
		(*comp)++;
		if (nQtd_elementos[inicio + i] > 0)
		{
			if (preenchida == -1)
			{
				preenchida = i;
			}
			else
				return -1;
		}
	}

	if (preenchida == -1)
	{
		return preenchida;
	}
	else
	{
		return inicio + preenchida;
	}
}

// Verifica se todas as fitas foram percorridas
short TodasforamPercorridas(FILE **fitas, int Qtd_elementos[], int n, int inicio, int *comp)
{
	for (int i = 0; i < n; i++)
	{
		if (ftell(fitas[inicio + i]) / sizeof(Registro) < Qtd_elementos[inicio + i])
			return 0;
	}

	return 1;
}

// retorna 1 se todos os blocos estão esgotados
short Blocosacabaram(short ativas[], int n, int *comp)
{
	for (int i = 0; i < n; i++)
	{
		(*comp)++;
		if (ativas[i] != -1)
			return 0;
	}
	return 1;
}

void OrdenaFita(FILE *temp, int n, Registro memoria[], FILE **fitas, int numerodeblocos, Lista blocos[FF], int *Qtd_elementos, int fita, int Tamanho_Memoria, int *escritas, int *comp, int *ler)
{

	for (int i = 0; i < numerodeblocos; i++)
	{
		// adiciona um novo bloco -> fita
		adicionarItemNoInicioDaLista(&blocos[fita], Qtd_elementos[fita]);
		// calcula o numero de registros que tera no bloco
		int NumeroDeRegistros;
		if (i == numerodeblocos - 1 && n % Tamanho_Memoria != 0)
		{
			NumeroDeRegistros = n % Tamanho_Memoria;
		}
		else
		{
			NumeroDeRegistros = Tamanho_Memoria;
		}
		(*comp)++;
		//fazer a leitura dos Registros da fita
		fread(memoria, sizeof(Registro), NumeroDeRegistros, temp);

		(*ler)++;

		sort(memoria, NumeroDeRegistros, comp); // ordena a memoria
		fwrite(memoria, sizeof(Registro), NumeroDeRegistros, fitas[fita]);
		(*escritas)++;

		Qtd_elementos[fita] += NumeroDeRegistros;
		fita = (fita + 1) % (FF / 2); // Percorre somente entre as fitas de entrada
	}
}

void FuncaoPrincipal_1_Ordenacao(int n, FILE *temp, FILE **fitas, int Qtd_elementos[FF], Lista blocos[FF], int *escritas, int *comp, int *ler)
{

	// Tamanho da memoria a ser utilizado  -> proposto no enunciado -> usar um vetor de 20 registros
	int Tamanho_Memoria = F;
	//Vetor de Registros
	Registro memoria[F];
	// Coloca os ponteiros no inicio do arquivo
	for (int i = 0; i < FF; i++)
	{
		(*escritas)++;
		rewind(fitas[i]);
	}

	rewind(temp);
	(*escritas)++;
	// Numero de elementos na lista recebe 0
	for (int i = 0; i < FF; i++)
	{
		Qtd_elementos[i] = 0;
	}
	// fita que vai ser utilizada
	int fita = 0;
	// bloco total armazenado na fita
	for (int i = 0; i < FF; i++)
	{
		blocos[i].primeiroItem = NULL;
	}

	int numerodeblocos;
	// Criação dos blocos ordenados
	if ((n % Tamanho_Memoria == 0))
	{
		numerodeblocos = n / Tamanho_Memoria;
	}
	else
	{
		numerodeblocos = n / Tamanho_Memoria + 1;
	}

	//Ordenação
	OrdenaFita(temp, n, memoria, fitas, numerodeblocos, blocos, Qtd_elementos, fita, Tamanho_Memoria, escritas, comp, ler);
}

void FuncaoPrincipal_2_Intercalacao(int n, FILE *temp, FILE **fitas, int Qtd_elementos[FF], Lista blocos[FF], int *escritas, int *comp, int *ler)
{

	Registro memoria[F];
	int from[F];
	short ativas[F];
	short Fitas_Jump = 1;
	int fita;
	for (int i = 0; i < FF; i++)
	{
		(*escritas)++;
		rewind(fitas[i]);
	}
	// Essa variavel armazena o indice da única fita preenchida
	int f = CondicaodaFita(Qtd_elementos, F, F * Fitas_Jump, comp);
	while (f == -1)
	{

		// O alvo de torna a primeira fita de saida
		fita = F * Fitas_Jump;
		int bloco = 0;
		// a condição é -> ate percorrer todas as fitas de entrada
		while (!TodasforamPercorridas(fitas, Qtd_elementos, F, F * (!Fitas_Jump), comp))
		{
			// adiciona um novo bloco na fita que foi destinada
			adicionarItemNoInicioDaLista(&blocos[fita], ftell(fitas[fita]) / sizeof(Registro));
			// indice onde termina o bloco de cada fita
			int limiteblocos[F];
			for (int i = 0; i < F; i++)
			{
				if (tamanhoDaLista(&blocos[(F * !Fitas_Jump) + i]) > bloco + 1)
					limiteblocos[i] = posicaoNaLista(&blocos[(F * !Fitas_Jump) + i], bloco + 1);
				else
					limiteblocos[i] = Qtd_elementos[(F * !Fitas_Jump) + i];
			}
			// Pega o primeiro elemento de cada fita de entrada
			for (int i = 0; i < F; i++)
			{
				// tem elemento no bloco?
				if (ftell(fitas[(F * !Fitas_Jump) + i]) / sizeof(Registro) < limiteblocos[i])
				{
					// pega o primeiro elemento
					fread(&memoria[i], sizeof(Registro), 1, fitas[(F * !Fitas_Jump) + i]);
					(*ler)++;
					if (ftell(fitas[(F * !Fitas_Jump) + i]) / sizeof(Registro) >= limiteblocos[i])
					{
						// foi o ultimo elemento?
						ativas[i] = 0;
					}
					else
					{
						ativas[i] = 1;
					}
				}
				else
					ativas[i] = -1;
				from[i] = i + (!Fitas_Jump * F);
			}
			// enquanto um bloco nao estiver nulo -> valor igual a -1
			while (!Blocosacabaram(ativas, F, comp))
			{
				// ordena a memoria
				heapsort(memoria, ativas, from, F, comp);
				// joga o menor elemento na fita de saida
				fwrite(&memoria[0], sizeof(Registro), 1, fitas[fita]);
				(*escritas)++;
				// aumenta o tamanho da fita
				Qtd_elementos[fita]++;
				// se a fita do menor está ativa:
				if (ativas[from[0] % F])
				{
					// pega o registro seguinte
					fread(&memoria[0], sizeof(Registro), 1, fitas[from[0]]);
					(*ler)++;
					// se tiver sido o ultimo Registro:
					if (ftell(fitas[from[0]]) / sizeof(Registro) == limiteblocos[from[0] % F])
					{
						// a fita se torna inativa
						ativas[from[0] % F] = 0;
					}
				}
				else
				{
					// bloco recebe valor nulo
					ativas[from[0] % F] = -1;
				}
			}
			// Fita seguinte se torna o alvo
			fita = (fita % F + 1) % F + F * Fitas_Jump;
			(*comp)++;
			bloco++;
		}

		for (int i = 0; i < F; i++)
		{
			// Sett os ponteiros das fitas de saida
			rewind(fitas[F * Fitas_Jump + i]);
			// Sett os ponteiros das fitas de entrada
			rewind(fitas[F * (!Fitas_Jump) + i]);
			// zera as quantidades de elementos das fitas de entrada
			Qtd_elementos[F * (!Fitas_Jump) + i] = 0;
			// limpa os blocos das fitas de entrada
			limparLista(&blocos[F * (!Fitas_Jump) + i]);
			(*escritas)++;
		}
		// troca as fitas de entrada com as fitas de saida
		Fitas_Jump = !Fitas_Jump;
		// faz a verificação (se resta apenas uma fita com elementos)
		f = CondicaodaFita(Qtd_elementos, F, F * (!Fitas_Jump), comp);
	}

	rewind(fitas[f]);
	(*escritas)++;
	rewind(temp);
	(*escritas)++;
	Registro r;
	for (int i = 0; i < n; i++)
	{
		// joga todos os Registros da fita f na fita principal
		fread(&r, sizeof(Registro), 1, fitas[f]);
		(*ler)++;
		fwrite(&r, sizeof(Registro), 1, temp);
		(*escritas) += 2;
	}
}

// Metodo 1 possui duas funçoes principais: - FuncaoPrincipal_1_Ordenacao, FuncaoPrincipal_2_Intercalacao, quem fazem a ordenação e a intercalação respectivamente
void metodo1(int quantidade, FILE *arqTemporario, int *numerodeLeituras, int *numerodeEscritas, int *numerodeComparacoes)
{
	// Crio um vetor de fitas
	FILE **fitas = (FILE **)malloc(sizeof(FILE *) * FF);
	for (int i = 0; i < FF; i++)
	{
		char nome[8];
		sprintf(nome, "F%.2i.bin", i);
		fitas[i] = fopen(nome, "w+b");
	}
	// Inicialização
	int Qtd_elementos[FF];
	for (int i = 0; i < FF; i++)
	{
		Qtd_elementos[i] = 0;
	}
	// Inicialização
	Lista blocos[FF];
	for (int i = 0; i < FF; i++)
	{
		blocos[i].primeiroItem = NULL;
	}
	// chamada das funções principais
	FuncaoPrincipal_1_Ordenacao(quantidade, arqTemporario, fitas, Qtd_elementos, blocos, numerodeEscritas, numerodeComparacoes, numerodeLeituras);
	// => O arquivo se quebra em blocos do tamanho da memória interna disponível
	// => Ordenação de cada bloco na memória interna
	FuncaoPrincipal_2_Intercalacao(quantidade, arqTemporario, fitas, Qtd_elementos, blocos, numerodeEscritas, numerodeComparacoes, numerodeLeituras);
	// => Leitura do primeiro registro de cada fita
	// => Retirada do registro contendo a menor chave e armazenando esta em uma fita de saida
	// => Leitura de um novo registro da fita onde o registro foi retirado da Memoria principal
	// => Ao ler todos os registros de um dos blocos, a fita correspondente fica inativa
	// => Esta fita que ficou inativa é reativada quando o n-esimo registros das outras fitas forem lidos

	for (int i = 0; i < FF; i++)
	{
		fclose(fitas[i]);
	}

	for (int i = 0; i < FF; i++)
	{
		char nome[8];
		sprintf(nome, "F%.2i.bin", i);
		remove(nome);
	}
}