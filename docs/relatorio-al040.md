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

Neste relatório é abordada a solução desenvolvida pelo grupo para o problema proposto.

O problema consiste em identificar sub-redes regionais numa rede de distribuição de supermercados.  
O programa recebe o numero de supermercados, o número de ligações entre eles e as ligações entre
eles como *input* e devolve como *output* o número de sub-redes regionais, o número de ligações entre
estas sub-redes e as ligações entre as sub-redes.

## Solução

A solução foi implementada na linguagem C.

Foi usado um grafo dirigido, com supermercados a serem vértices e as ligações entre supermercados a
serem arestas. Este grafo era representado num *array* 2xE (em que E é o número de ligações), em que
na primeira linha estão os vértices e na segunda as suas respetivas ligações. Nesta solução as sub-redes
são as componentes fortemente ligadas do grafo, logo foi utilizado o algoritmo de Tarjan para obter essas
mesmas compenentes.

## Análise teórica

## Análise prática
