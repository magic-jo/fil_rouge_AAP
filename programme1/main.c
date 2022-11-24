#include "include.h"

#define LEN 1000 // nombre de caractères max pour une chaîne de caractères
#define MAX_VERTICES 100 // nombre de sommets max dans une matrice

int main(int argc, char **argv) { 		

	char * filename_adj = argv[1];
	int sI = atoi(argv[2]);
	int sf = atoi(argv[3]);

	T_graphMD *g = adjtoMD(filename_adj);
	T_bf bf = bellmanFord2(g, sI);

	T_chemin chmn = showPath(bf, sI, sf);

	const char * dot_path = getenv("DOT_PATH");
	if (dot_path != NULL) {

		char filename_dot[LEN] = "";
		strcat(filename_dot, dot_path);
		strcat(filename_dot, filename_adj+1); // écrire le fichier adj en argument ./nomdufichier.adj !
											  // on ne veut pas du point dans ./nomdufichier.adj , d'où le +1
		int lng = strlen(filename_adj) + strlen(dot_path) - 1;
		filename_dot[lng-3] = 'd';
		filename_dot[lng-2] = 'o';
		filename_dot[lng-1] = 't'; // on a créé la chaîne de caractères DOT_PATH/filename.dot
	
		char clean[LEN] = "rm -r ";
		strcat(clean, dot_path);
		system(clean); // exécution de la commande rm -r DOT_PATH
		
		char s[LEN] = "mkdir "; 
		strcat(s, dot_path);
		system(s); // exécution de la commande mkdir DOT_PATH

		showGraph(filename_dot, g); // crée le fichier dot du graphique dans le même répertoire que le fichier .adj
									// filename_dot s'écrit en réalité DOT_PATH/filename_dot
		printpng(chmn, filename_dot);
	}
}

//Crée et renvoie une matrice initialisée à INT_MAX partout
T_graphMD * newGraphMD(unsigned int n) {

	int i, j;

	T_graphMD * g = NULL;
	CHECK_IF( g = (T_graphMD *) malloc( sizeof(T_graphMD) ), NULL, "malloc g newGraph");

	g->nbVertices = n;
	CHECK_IF( g->mat = (int **)malloc(n * sizeof(int *)), NULL, "malloc g->mat newGraph");

	for (i = 0; i < n; i++) 
		g->mat[i] = (int *)malloc(n * sizeof(int));
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			g->mat[i][j] = INT_MAX;
	return g;
}

//Prend en argument une matrice d'adjacence et renvoie sa taille	
unsigned int tailleMatrice(char *filename_adj) {

	FILE *fp;
	CHECK_IF( fp = fopen(filename_adj,"r"), NULL, "fopen tailleMatrice");

	char *s = NULL;
	s = (char *) malloc( LEN * sizeof(char));
	int n;
	for (n = 0; fgets(s, LEN, fp) != NULL; n++ ) {}

	fclose(fp);
	printf("tailleMat : n = %d\n", n);
	return n;
}


	// Fonctions servant à trouver le plus court chemin et sa longueur puis les affiche

//Transforme une matrice dans le fichier .adj filename.adj en un T_graphMD
T_graphMD * adjtoMD(char *filename_adj) {

	int i, j, k, buffer;
	// i : indice de ligne, j : indice de caractère, k : nombre de digits du poids de l'arête

	unsigned int noeudsMax = tailleMatrice(filename_adj);

	T_graphMD *g = newGraphMD(noeudsMax);
	
	FILE *fp;
	CHECK_IF( fp = fopen(filename_adj,"r"), NULL, "fopen adjtoMD"); 
	
	char *stemp = NULL;  // string qui stocke temporairement chaque ligne de la mat d'adj
	CHECK_IF( stemp = (char *) malloc( LEN * sizeof(char) ), NULL, "malloc adjtoMD");

	for (i = 0; fgets(stemp, LEN, fp) != NULL; i++ ) { // parcours les lignes de la matrice, fgets incrémente sur les lignes toute seule
		// on stocke la ligne dans stemp (de longueur maximale 2*taille : hypothèse que la ligne 1 est au minimum 2x plus courte que la ligne la plus longue)

		j = 0;  // remise à 0 du compteur de colonnes

		while (stemp[0] != '\0') {

			k = 0;

			buffer = atoi(stemp); // buffer correspond au prochain nombre à stocker, qu'il soit le prochain caractère ou pas
			
			if ( stemp[0] != '\t' ) { // ici, stemp[j] peut seulement valoir le caractère 0, i ou un char représentant un entier
				if ( buffer != 0 ) { // si le caractère lu n'est pas i ou 0
					g->mat[i][j] = buffer; // stocker la valeur du poids de l'arête IJ		
					while ( buffer > 10 ) {
						buffer /= 10;
						k++; //compter le nombre de digit de buffer (le poids de l'arête)
					}
				}	
			}
			else
				j++;
				
			stemp += (k+1);
		}
	}
	g->nbVertices = i;
	return g;
}

//Prend un graphe de type T_graphMD et un sommet ; renvoie la longueur des plus courts chemins à ce sommet et les prédecesseurs de chaque sommet
T_bf bellmanFord2(T_graphMD * g,  int s) {
	T_bf bf;
	int i, j, k;
	CHECK_IF(bf.d = malloc( g->nbVertices * sizeof(int) ), NULL, "malloc vecteur d bellmanFord2");
	CHECK_IF(bf.p = malloc( g->nbVertices * sizeof(int) ), NULL, "malloc vecteur p bellmanFord2");

	//initialisation de bf : tous les sommets ont pour prédécesseur s
	//et tous les chemins ont un poids "infini" sauf celui de s à s qui est nul
	for (i = 0; i < g->nbVertices; i++) { 
	bf.d[i] = INT_MAX;
	bf.p[i] = s;
	}
	bf.d[s] = 0;

	for (k = 1; k < g->nbVertices; k++) //on répète la boucle (nombredenoeuds-1) fois
		for (i = 0; i < g->nbVertices; i++) //parcours de la matrice adjacente mat
			for (j = 0; j < g->nbVertices; j++)
				if (g->mat[i][j] != INT_MAX)
					if ( (bf.d[j] > bf.d[i] + g->mat[i][j]) && (bf.d[i]!=INT_MAX) ) { //relâchement
						bf.d[j] = bf.d[i] + g->mat[i][j];	
						bf.p[j] = i; //les sommets prédecesseurs sont indentés à partir de 0 (écrire sI pour parler du sommet 1)
					}
	return bf;
}

//Affiche le plus court chemin d'un T_bf ainsi que sa longueur et renvoie le chemin sous forme d'un T_chemin
T_chemin showPath(T_bf bf, int sI, int sf) {

	// déclaration et initialisation des variables
	unsigned int longueur = 0; 
	int i = sf, j = 0, k;

	T_chemin chmn;
	CHECK_IF(chmn.chemin = (int *) malloc (MAX_VERTICES * sizeof(int)), NULL, "\nmalloc chmn.chemin showPath"); // chmn.chemin = vecteur qui stocke les sommets successifs
	chmn.nbrSommets = 2; // il y a au moins deux sommets dans le chemin, on incrémente par la suite ce nombre pdt le parcours du chemin
	int l = 1; // indice qui augmente et permet de remplir chmn.chemin dans l'ordre "croissant"

	int *T; // T : pile qui stocke les prédecesseurs successifs de sf
	CHECK_IF(T = (int *)malloc( 100 * sizeof(int) ), NULL, "malloc tableauT showPath"); // il y a au maximum 100 prédecesseurs
	
	// on évalue le prédecesseur jusqu'à remonter au 1er sommet
	while (bf.p[i] != sI) {
		T[j] = bf.p[i]; // on stocke dans T le prédecesseur et on incrémente j, le compteur de sommets dans le chemin, par la même occasion
		j++;
		i = bf.p[i];
	}
	longueur = bf.d[sf]; 

	// affichage de la longueur
	printf("Le plus court chemin du sommet %d au sommet %d a pour longueur %d.\n",sI,sf,longueur);

	// affichage du chemin
	printf("%d -> ",sI); // début du chemin
	chmn.chemin[0] = sI;

	for (k = j-1; k >= 0; k--) { // parcours de la pile T de haut en bas : on dépile le chemin 
		printf("%d -> ", T[k]); // +1 car s(0) correspond au sommet s1, ...
		chmn.chemin[l++] = T[k];
		(chmn.nbrSommets)++;
	}

	printf("%d\n",sf); // fin du chemin
	chmn.chemin[l] = sf;

	return chmn;
}


	// Fonctions servant à afficher des images png si la variable DOT_PATH est déclarée

	// 1. Création du fichier .dot

//Ecrit le format .dot complet (entête+structure) dans le fichier filename_dot à partir du T_graphMD g
void showGraph(const char * filename_dot, T_graphMD * g) {

	FILE * fp = NULL;
	CHECK_IF(fp = fopen(filename_dot,"w"), NULL, "fopen ShowGraph");

	fprintf(fp,"digraph graphe {\n");
	fprintf(fp,"rankdir = LR;\n");
	fprintf(fp,"node [fontname=\"Arial\", shape = circle, color=lightblue, style=filled];\n");
	fprintf(fp,"edge [color=red];\n");
	dumpGraph(fp,g);
	fprintf(fp,"}\n");
	
	fclose(fp);
}

//Ecrit la structure du graphe dans le fichier fp
void dumpGraph(FILE * fp, T_graphMD * g) {

	int i,j;

	for (i=0; i < g->nbVertices; i++) 
		for (j=0; j<g->nbVertices; j++) 
			if ( g->mat[i][j] != INT_MAX ) 
				fprintf( fp, "\t%d -> %d [label = \"%d\"];\n", i, j, g->mat[i][j]);
}

	// 2. Création des images png

//Prend en argument un T_chemin et un fichier dot, produit les images png successives en allumant les sommets du chemin
void printpng(T_chemin chmn, char * filename_dot) {

	int sommetActuel;
	int len = strlen(filename_dot);
	char filename_i_dot[LEN];

	for (int i = 0; i <= chmn.nbrSommets; i++ ) {

		// initialisation : copier le filename_dot et allumer le premier sommet en vert

		strcpy(filename_i_dot, filename_dot);
		filename_i_dot[len - 4] = '_';
		filename_i_dot[len - 3] = '0'+i; // ça ne fonctionne que si le sommet s'écrit avec un seul digit
		filename_i_dot[len - 2] = '.';
		filename_i_dot[len - 1] = 'd';
		filename_i_dot[len] = 'o';
		filename_i_dot[len + 1] = 't';
		filename_i_dot[len + 2] = '\0'; // création de la chaîne filename_i.dot

		char s[LEN] = "cp ";
		strcat(s, filename_dot);
		strcat(s, " ");
		strcat(s, filename_i_dot); // création de la chaîne "cp filename_dot filename_i.dot"

		system(s); // exécution de la commande cp filename_dot filename_i.dot

		sommetActuel = chmn.chemin[i];
		FILE *fp = NULL;
		CHECK_IF(fp = fopen(filename_i_dot,"r+"), NULL, "fopen printpng");

		fseek(fp, -2, SEEK_END);
		fprintf(fp, "\t");

		if (i != chmn.nbrSommets) // condition servant à détecter si on est arrivés au sommet final
			fprintf(fp, "\"%d\" [color = green];\n", sommetActuel); // on colorie le sommetActuel en vert
		//else
		//	fprintf(fp, "\"%d\" [color = red];\n", chmn.chemin[i-1]); // on colorie le sommetActuel en rouge pour la dernière image

		for (int j=0; j < i; j++) {
			fprintf(fp, "\t\"%d\" [color = red];\n", chmn.chemin[j]); // on colorie les sommets précédents en rouge
		}

		fprintf(fp, "}\n"); 

		fclose(fp); 

		int len_i = strlen(filename_i_dot);
		char filename_i_png[LEN];
		strcpy(filename_i_png, filename_i_dot);
		filename_i_png[ len_i -3 ] = 'p';
		filename_i_png[ len_i -2 ] = 'n';
		filename_i_png[ len_i -1 ] = 'g';

		char s2[LEN] = "dot -T png ";
		strcat(s2, filename_i_dot);
		strcat(s2, " -o ");
		strcat(s2, filename_i_png);
		system(s2); //exécution de la commande : dot -T png filename_i_dot -o filename_i_png
		printf("Production de l'image png %s.\n", filename_i_png);
	}
}
