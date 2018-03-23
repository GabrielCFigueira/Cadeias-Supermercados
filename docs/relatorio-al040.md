---
title: Relatório do projeto 1 de ASA - grupo AL040
documentclass: article
author:
- Gabriel Figueira, n. 86426
- Rafael Andrade, n. 86503
papersize: A4
fontsize: 12pt
geometry: margin=3cm
---

## Introdução

O problema consiste em identificar sub-redes regionais numa rede de
distribuição de supermercados. O programa recebe o numero de supermercados, o
número de ligações entre eles e as ligações entre eles como *input* e devolve
como *output* o número de sub-redes regionais, o número de ligações entre
estas sub-redes e as ligações entre as sub-redes. As sub-redes regionais são
identificadas pelo menor identificador de supermercado que pertence à sub-rede.

## Solução

A rede de supermercados pode ser abstraída como um grafo dirigido, onde cada
supermercado é um vértice distinto, e cada ligação é um arco. Assim, cada
sub-rede é, na verdade, uma componente fortemente ligada[^SCC] e a ligação entre
sub-redes são as pontes entre SCCs.

[^SCC]: Daqui para a frente, em vez de "componente fortemente ligada" será
usada a sigla inglesa *Strongly Connected Component* (SCC).


Em termos teóricos, para representar o grafo, foi usada uma lista de
adjacências, e utilizámos o algoritmo de Tarjan para identificar SCCs.
Em termos práticos, utilizámos a linguagem C, e a lista de adjacências é
um *array* de tamanho do número de ligações, em que cada posição guarda dois
inteiros (*int[2]*), ao qual se junta outro *array*, de tamanho do número de
vértices, que guarda em cada posição um inteiro correspondente ao *offset*
das suas ligações no primeiro array.


A solução pode ser descrita em:

  - Ler o *input*, ordenar as ligações e criar o grafo
  - Aplicar o algoritmo de Tarjan, obtendo assim as SCC
  - Identificar, para cada SCC, qual o menor dos identificadores
  - Eliminar os arcos redundantes (inter-SCC) e os que ligam vértices
    pertencentes ao mesmo SCC, e contar quantos arcos restantes existem
  - Ordenar e imprimir os ditos arcos


Para eliminar arcos redundantes, o processo utilizado foi:

  - Ordenar a lista de adjacências
  - Percorrer a lista com 2 ponteiros (chamemos *i* e *j*). É efetuado um ciclo,
    onde *i* é incrementado; e sempre que a *i*-ésima posição for diferente à
    posição *i+1*, colocar esse elemento na posição *j*, e incrementar *j*;
    senão, nada é feito. No final, podemos afirmar que a lista de adjacências de
    tamanho *j* é uma lista ordenada de arcos sem nenhum repetido.
    Como os arcos estão ordenados, todos os arcos que sejam iguais estarão em
    posições consecutivas na lista, logo ao percorrer desta forma iremos "saltar"
    por cima desses repetidos, ficando com uma cópia do arco.


Para ordenar a lista de adjacências, foi utilizado o algoritmo Radix sort
(versão LSD).


## Análise Teórica

O algoritmo de Tarjan tem, teoricamente, complexidade temporal O(V + E) e
complexidade espacial O(V), em que V é o número de vértices e E o número de
arcos.

As complexidades de outros passos na solução são estes:

|                        | Tarjan    | Ordenar Ligações | Remover arcos repetidos |
|:----------------------:|:---------:|:----------------:|:-----------------------:|
| Complexidade Temporal  |  O(V + E) |  O(V + E)        |    O(V + E)             |
| Complexidade Espacial  |  O(V)     |  O(V + E)        |    O(V + E)             |

## Análise prática

Para testar experimentalmente a eficiência do algoritmo, foram gerados 45 testes
aleatórios, com o número de vértices total a variar entre 100 000 e 1 000 000, e
o número de arcos entre 2 a 6 vezes maior que o número de vértices. Os testes
foram executados no sistema SIGMA, correndo num computador com processadores
Intel(R) Xeon(R) CPU E5-2620 @ 2.00GHz, 8-core, com 16 GB de RAM total.


![](./docs/graphs/run-chart.png)


![](./docs/graphs/mem-chart.png)


Como é possível observar nos gráficos, tanto a complexidade temporal quanto a
espacial observadas estão em linha com o esperado. É possível observar uma
ligeira concavidade no gráfico que mostra o tempo de execução; a razão para
tal acontecer pode ser atribuída a escalonamento de processos num ambiente
multi-utilizador como o de teste.
