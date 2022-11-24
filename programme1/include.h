#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

// #define DEBUG
#include "check.h"

//structure pour une matrice d'adjacence
typedef struct {
	unsigned int nbVertices;
	int **mat;
} T_graphMD;

//structure contenant un vecteur des distances à sI et un vecteur contenant le prédecesseur pour chaque sommet
typedef struct {
	int *d;
	int *p;
} T_bf;

//un T_chemin contient un chemin d'un sommet à un autre ainsi que le nombre de sommets le composant
typedef struct {
	int * chemin;
	int nbrSommets;
} T_chemin;

//déclaration des prototypes des fonctions

T_graphMD * newGraphMD(unsigned int n); 
int * bellmanFord(T_graphMD * g,  int s); 
T_bf bellmanFord2(T_graphMD * g,  int s);
T_chemin showPath(T_bf bf, int sI, int sf);
void dumpGraph(FILE * fp, T_graphMD * g); 
void showGraph(const char * filename_dot, T_graphMD * g);
T_graphMD * adjtoMD(char *filename_adj);
unsigned int tailleMatrice(char *filename_adj);
void printpng(T_chemin chmn, char * filename_dot);
