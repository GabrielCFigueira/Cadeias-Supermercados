/*
 * adj.h
 * ASA 2018
 * Gabriel Figueira, Rafael Andrade
 * P1 (Sr. Joao Caracol)
 */

#pragma once


typedef struct graph * Graph;


Graph  buildGraph();
Graph  transposeGraph(Graph g);
Graph  reduceGraph(Graph g, int * trans);
void   showGraph(const Graph g);
void   printSccGraph(Graph g, int nScc);
void   freeGraph(Graph g);
void   doForEachAdjU(Graph g, int u, void (*func)(Graph, int, int));
int    nVertex(Graph g);
int    nConnection(Graph g);
