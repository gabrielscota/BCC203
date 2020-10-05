#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>

// Item da lista que armazena um valor inteiro, o item anterior e o proximo
typedef struct listaitem ListaItem;
struct listaitem
{
    int valor;
    ListaItem *itemAnterior;
    ListaItem *proximoItem;
};

// LinkedList com o ponteiro do primeiro item
typedef struct Lista
{
    ListaItem *primeiroItem;
} Lista;

// Função para adicionar um item no inicio da lista
void adicionarItemNoInicioDaLista(Lista *lista, int valor)
{
    ListaItem *listaItem = malloc(1 * sizeof(ListaItem));
    listaItem->valor = valor;
    listaItem->itemAnterior = NULL;
    listaItem->proximoItem = NULL;

    if (lista->primeiroItem == NULL)
    {
        lista->primeiroItem = listaItem;
    }
    else
    {
        ListaItem *aux = lista->primeiroItem;
        while (aux->proximoItem != NULL)
        {
            aux = aux->proximoItem;
        }
        aux->proximoItem = listaItem;
        listaItem->itemAnterior = aux;
    }
}

// Função para remover o primeiro item da lista, caso consiga remover o valor do item removido é retornado
void removerPrimeiroItemDaLista(Lista *lista)
{
    if (lista->primeiroItem == NULL)
    {
        return;
    }
    ListaItem *aux = lista->primeiroItem;
    lista->primeiroItem = aux->proximoItem;
    aux = NULL;
}

// Função para calcular o numero de itens presente na lista
int tamanhoDaLista(Lista *lista)
{
    if (lista->primeiroItem == NULL)
    {
        return 0;
    }
    int tamanho;
    ListaItem *aux = lista->primeiroItem;
    for (tamanho = 0; aux != NULL; tamanho++)
    {
        aux = aux->proximoItem;
    }
    return tamanho;
}

// Função para retornar o valor do indice buscado
int posicaoNaLista(Lista *lista, int indice)
{
    if (lista->primeiroItem == NULL || indice >= tamanhoDaLista(lista) || indice < 0){
        return -1;
    }
    ListaItem *aux = lista->primeiroItem;
    for (int i = 0; i < indice; i++){
        aux = aux->proximoItem;
    }
    return aux->valor;
}

// Função para limpar uma lista
void limparLista(Lista *lista)
{
    // Caso a lista já esteja vazia, apenas retorna
    if (lista->primeiroItem == NULL)
    {
        return;
    }
    // Caso contrário ele ira remover o primeiro item da lista enquanto houver itens
    while (lista->primeiroItem != NULL)
    {
        removerPrimeiroItemDaLista(lista);
    }
}

#endif