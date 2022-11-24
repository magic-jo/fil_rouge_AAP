#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

// #define DEBUG
#include "check.h"

#define LEN 1000 //nombre de char max par string

//structure pour une matrice d'adjacence
typedef struct {
	unsigned int nbVertices;
	int **mat;
} T_graphMD;

//structure pour un noeud de liste d'adjacence ; contient : le sommet pointé, le poids de l'arête et un pointeur vers une autre arête partant du même sommet initial
typedef struct node {
    unsigned int vertexNumber;
    int weight;
    struct node * pNext;
} T_node, *T_list;

//contient le nombre de noeuds du graphe représenté et un pointeur vers le tableau des sommets de la liste d'adjacence
typedef struct {
    unsigned int nbVertices;
    T_list *tAdj; // Un tableau de T_list ! 
} T_graphLA;


typedef struct {
    int sommetPointe;
    int poidsArete;
}   T_sommetVise;

T_graphLA * newGraphLA(int n);
void addEdge(T_graphLA *g, int sommetInitial, int sommetPointe, int poidsArete);
unsigned int nbrNoeuds(char *filename_adj);
T_graphLA *adjtoLA(char *filename_adj);
T_graphLA *latoLA(char *filename_la);
void LAtodot(T_graphLA *g);
void LAtola(T_graphLA *g);
void LAtoadj(T_graphLA *g);
int getDigits(int x);
