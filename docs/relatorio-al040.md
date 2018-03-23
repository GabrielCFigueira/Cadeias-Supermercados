---
title: Relatório do projeto 1 de ASA - grupo AL040
documentclass: article
author:
- Gabriel Figueira, n. 86426
- Rafael Andrade, n. 86503
papersize: A4
fontsize: 12pt
geometry: margin=3cm
geometry: vmargin=2cm
---

## Introdução

O problema apresentado consiste em identificar sub-redes regionais numa rede de
distribuição de supermercados. A solução deve ler o número de supermercados, o
número de ligações entre eles e as ligações entre eles como *input* e devolver
como *output* o número de sub-redes regionais, o número de ligações entre
estas sub-redes e as ligações entre as sub-redes. As sub-redes regionais são
identificadas pelo menor identificador de supermercado que pertence à sub-rede.


## Solução

A rede de supermercados pode ser abstraída como um grafo dirigido, onde cada
supermercado é um vértice distinto, e cada ligação é um arco. Assim, cada
sub-rede é, na verdade, uma componente fortemente ligada[^SCC] e a ligação entre
sub-redes são as pontes entre SCCs.

[^SCC]: Daqui para a frente, em vez de "componente fortemente ligada" será
usada a sigla inglesa SCC, de *Strongly Connected Component*.


Em termos teóricos, para representar o grafo, foi usada uma lista de
adjacências, e utilizámos o algoritmo de Tarjan para identificar SCCs.
Em termos práticos, utilizámos a linguagem C, e a lista de adjacências é
um *array* de tamanho do número de ligações, em que cada posição guarda dois
inteiros (*int[2]*), ao qual se junta outro *array*, de tamanho do número de
vértices, que guarda em cada posição um inteiro correspondente ao *offset*
das suas ligações no primeiro array.


A solução pode ser descrita em:

  - Ler o *input* e criar o grafo
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
    senão, nada é feito.

No final, podemos afirmar que a lista de adjacências de tamanho *j* é uma lista
ordenada de arcos sem nenhum repetido. A razão para tal é que os arcos estão
ordenados, logo todos os arcos que sejam iguais estarão em posições consecutivas
na lista, logo ao percorrer desta forma a lista de adjacências iremos "saltar" por
cima desses repetidos, ficando com uma cópia do arco, e depois as restantes cópias
são ignoradas, quer porque *j* aumenta o suficiente para que sejam substituídas
por outros arcos, ou então porque no final de *i* percorrer a lista *j* ainda
está numa posição anterior a estes arcos repetidos.


Para ordenar a lista de adjacências, foi utilizado o algoritmo Radix sort
(versão LSD).



## Análise Teórica

O algoritmo de Tarjan tem, teoricamente, complexidade temporal $O(V+E)$ e
complexidade espacial $O(V)$, em que *V* é o número de vértices e *E* o número de
arcos.

Como a ordenação de ligações foi feita usando Radix sort, em princípio a
complexidade temporal seria $O(2E)$, mas como foi utilizada uma lista de
*offsets*[^O] de tamanho *V* para saber onde inserir os arcos na lista de
adjacências, e como para criar uma lista de *offsets* é necessário percorrê-la
pelo menos 1 vez, a complexidade temporal é $O(2(V+E)) = O(V+E)$. Em termos de
complexidade espacial, a nossa implementação ocupa $O(V+E)$, pois é necessário não
só criar uma nova lista de adjacências de tamanho *E* mas também a dita lista de
*offsets*, de tamanho *V*.

[^O]: A lista de *offsets* é criada em dois passos: primeiro, contamos quantos
      arcos é que têm um vértice *u*, e incrementamos essa posição na lista;
      depois, percorremos a lista para fazer uma soma cumulativa.


Por fim, a remoção de arcos repetidos é feita com uma só passagem na lista de
adjacências, como já foi descrito na secção anterior. No entanto, como é necessário
ordenar antes de poder remover arcos repetidos, somos forçados a admitir que a
complexidade neste passo é tanta quanto a de ordenar as ligações.

Resumindo, as complexidades dos principais passos da nossa solução são:

|                        | Tarjan    | Ordenar Ligações | Remover arcos repetidos |
|:----------------------:|:---------:|:----------------:|:-----------------------:|
| Complexidade Temporal  |  O(V + E) |  O(V + E)        |    O(V + E)             |
| Complexidade Espacial  |  O(V)     |  O(V + E)        |    O(V + E)             |


Isto permite concluir que, no total, o programa implementado tem uma complexidade
de $O(V+E)$, tanto temporal como espacialmente.


## Análise Prática

Para testar experimentalmente a eficiência do algoritmo, foram gerados
aleatoriamente 45 testes, com o número de vértices a variar entre 100 000 e
1 000 000, e o número de arcos entre 2 a 6 vezes maior que o número de vértices.
Os testes foram executados nos servidores do sistema Sigma da DSI, correndo num
computador com processadores Intel(R) Xeon(R) CPU E5-2620 @ 2.00GHz, 8-core, com
16 GB de RAM.

Para medir memória usada, foi utilizado o *Valgrind*, mais especificamente a
ferramenta *Massif*; para medir tempos de execução, foi utilizado o comando
*time*, somando o tempo de execução em modo utilizador e em modo sistema.


![](./docs/graphs/run-chart.png)


![](./docs/graphs/mem-chart.png)


Como é possível observar nos gráficos, tanto a complexidade temporal quanto a
espacial observadas estão em linha com o esperado. É possível observar que
a complexidade temporal não é exatamente linear, embora seja muito próxima disso;
a razão para tal acontecer pode ser atribuída a circunstâncias e limitações da vida
real, tais como erros de medição ou acessos não constantes a memória devido a
*cache misses*.



## Referências:

  - Wikipedia, Radix Sort: https://en.wikipedia.org/wiki/Radix_sort#Efficiency
  - Apontamentos pessoais tirados das aulas dos professores Luís Russo e
    Alexandre Francisco
