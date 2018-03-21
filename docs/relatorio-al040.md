---
title: Relatório do projeto 1 de ASA - grupo AL040
documentclass: article
author:
- Gabriel Figueira, n. 86426
- Rafael Andrade, n. 86503
papersize: A4
fontsize: 12pt
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

O problema reduz-se então a:

  - Encontrar todas as SCCs presentes no grafo, e contar
  - Identificar, para cada SCC, qual o menor dos identificadores
  - Eliminar os arcos redundantes (inter-SCC) e os que ligam vértices
    pertencentes ao mesmo SCC, e contar quantos arcos restantes existem
  - Imprimir de forma ordenada os ditos arcos


Em termos teóricos, para representar o grafo, foi usada uma lista de
adjacências, e utilizámos o algoritmo de Tarjan para identificar SCCs.
Em termos práticos, utilizámos a linguagem C, e a lista de adjacências é
um *array* de tamanho do número de ligações, em que cada posição guarda dois
inteiros (*int[]*), ao qual se junta outro *array*, de tamanho do número de
vértices, que guarda em cada posição um inteiro correspondente ao *offset*
das suas ligações no primeiro array.

## Análise teórica

## Análise prática
