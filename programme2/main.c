#include "include.h"

int main(int argc, char **argv) {

    T_graphLA *g = NULL;

    for (int i=2; i < argc; i++) 
    {
        if (strcmp(argv[i],"-a") == 0)
            g = adjtoLA(argv[1]);
        if (strcmp(argv[i],"-l") ==0)
            g = latoLA(argv[1]);
    }

    for (int i=2; i < argc; i++)
    {
        if (strcmp(argv[i],"-dot") == 0)
            LAtodot(g);
        if (strcmp(argv[i],"-adj") == 0)
            LAtoadj(g);
        if (strcmp(argv[i],"-la") == 0)
            LAtola(g);
    }
}

//crée une liste d'ajacence composée de n sommets non reliés entre eux
T_graphLA * newGraphLA(int n) {
    T_graphLA * g;
    int i;
    g = (T_graphLA *) malloc( sizeof(T_graphLA) );
    g->nbVertices = n;
    g->tAdj = (T_list *) malloc( n * sizeof(T_list) );
    for (i=0; i < n; i++)
        g->tAdj[i] = NULL;
    return g;
}

//rajoute une arête entre sommetInitial et sommetPointe de poids poidsArete dans la liste d'adjacence g
void addEdge(T_graphLA *g, int sommetInitial, int sommetPointe, int poidsArete) {
    
    T_list p = (T_list) malloc( sizeof(T_node) );
    p->vertexNumber = sommetPointe;
    p->weight = poidsArete;
    p->pNext = g->tAdj[sommetInitial]; // ranger le T_node en première position, juste après le tAdj[i]

    g->tAdj[sommetInitial] = p;
}

//Prend en argument un fichier texte (matrice/liste d'adjacence) et renvoie le nombre de noeuds du graphe	
unsigned int nbrNoeuds(char *filename) {

	FILE *fp;
	CHECK_IF( fp = fopen(filename,"r"), NULL, "fopen nbrNoeuds");

	char *s = NULL;
	s = (char *) malloc( LEN * sizeof(char));

	int n;
	for (n = 0; fgets(s, LEN, fp) != NULL; n++ ) {}

	fclose(fp);
	return n;
}

//Renvoie le nombre de digits nécessaires pour écrire x en base 10
int getDigits(int x) {
    int k = 1;
    while ( x > 9 )
                {
					x /= 10;
					k++;                    //compter le nombre de digit de x
				}
    return k;
}

//Prend en argument une matrice d'adjacence et renvoie une liste d'adjacence
T_graphLA *adjtoLA(char *filename_adj) {

    T_graphLA *g = newGraphLA(nbrNoeuds(filename_adj));

    FILE *fp;
	CHECK_IF( fp = fopen(filename_adj,"r"), NULL, "fopen adjtoLA");

    char *stemp = NULL;  // string qui stocke temporairement chaque ligne de la MD
	CHECK_IF( stemp = (char *) malloc( LEN * sizeof(char) ), NULL, "malloc adjtoLA");

    int i, j, k, poids;

    for (i = 0; fgets(stemp, LEN, fp) != NULL; i++) 
    {
        j = 0;                                  // remise à 0 du compteur de colonnes
		while (stemp[0] != '\0') {
			k = 0;
			poids = atoi(stemp);                // poids correspond au prochain nombre à stocker, qu'il soit le prochain caractère ou pas           
			if ( stemp[0] != '\t' )             // ici, stemp[0] peut seulement valoir le caractère 0, i ou un char représentant un entier
            {                             
				if ( poids != 0 )               // si le caractère lu n'est pas i ou 0
                {         
					addEdge(g, i, j, poids);    // stocker la valeur du poids de l'arête IJ		
					while ( poids > 10 )
                    {
						poids /= 10;
						k++;                    //compter le nombre de digit de poids
					}
				}
			}
			else
				j++;			
			stemp += (k+1);
		}
	}
	return g;
}

//Prend en argument une liste d'adjacence sous format texte et renvoie une liste d'adjacence
T_graphLA *latoLA(char *filename_la) {

    T_graphLA *g = newGraphLA(nbrNoeuds(filename_la));

    FILE *fp;
	CHECK_IF( fp = fopen(filename_la,"r"), NULL, "fopen adjtoLA");

    char *stemp = NULL;  // string qui stocke temporairement chaque ligne de la MD
	CHECK_IF( stemp = (char *) malloc( LEN * sizeof(char) ), NULL, "malloc adjtoLA");

    int i, x;
    T_sommetVise *S = (T_sommetVise *) malloc( sizeof(T_sommetVise) );

    for (i = 0; fgets(stemp, LEN-1, fp) != NULL; i++) 
    {
		while (stemp[0] != '\0') 
        {
			if (stemp[0] != ' ')
            {   
                x = atoi(stemp);
                S->sommetPointe = x;
                stemp += getDigits(x);      // on avance du nombre de digits de x
                stemp++;                    // on avance d'un espace
                x = atoi(stemp);                     
				S->poidsArete = x;
                if (x != 0) 
                    addEdge(g, i, S->sommetPointe, S->poidsArete);    // stocker la valeur du poids de l'arête IJ	  
                stemp += getDigits(x);      // ici, on ne fait pas stemp++ car on pourrait être à la fin de la ligne 		
			}
            else		
                stemp++;
		}
	}
    fclose(fp);
	return g;   
}

//Prend en argument une liste d'adjacence et renvoie sa représentation dot dans stdout
void LAtodot(T_graphLA *g) {

    printf("digraph graphe {\n");
	printf("rankdir = LR;\n");
	printf("node [fontname=\"Arial\", shape = circle, color=lightblue, style=filled];\n");
	printf("edge [color=red];\n");

	int i;
    T_list p;
	for (i=0; i < g->nbVertices; i++) 
    {
        p = g->tAdj[i];
        while (p != NULL) 
        {           
            printf("\t%d -> %d [label = \"%d\"];\n", i, p->vertexNumber, p->weight);
            p = p->pNext;
        }
    }
	printf("}\n");
}

//Prend en argument une liste d'adjacence et renvoie sa représentation la dans stdout
void LAtola(T_graphLA *g) {

	int i;
    T_list p;
	for (i=0; i < g->nbVertices; i++) 
    {
        p = g->tAdj[i];
        while (p != NULL) 
        {           
            printf("%d %d ", p->vertexNumber, p->weight);
            p = p->pNext;
        }
        printf("\n");
    }
}

//Prend en argument une liste d'adjacence et renvoie sa représentation adj dans stdout
void LAtoadj(T_graphLA *g) {

	int i, k;
    T_list p;
    int *T = (int *) malloc(g->nbVertices * sizeof(int)); // tableau d'entiers pour stocker les poids


        
    for (i=0; i < g->nbVertices; i++)
    {
        for (k=0; k < g->nbVertices; k++) 
        {
            if (k == i)
                T[k] = 0;
            else
                T[k] = 105;
        } // "initialisation" d'une ligne d'un fichier adj : i partout, sauf un 0 sur la diagonale

        p = g->tAdj[i];
        while (p != NULL) 
        {
            T[p->vertexNumber] = p->weight;
            p = p->pNext;    
        }
        for (int l=0; l < g->nbVertices; l++) 
        {
            if (T[l] == 105)
                printf("i	");
            else
                printf("%d	", T[l]);   
        }        
        printf("\n");
    }    
}
