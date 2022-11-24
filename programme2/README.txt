Compte rendu du programme2 du Fil Rouge d'AAP
Date de rendu : mardi 12/01/2021
Auteurs : Pauline Alfaro, Yvan Marcadet, Joseph Robineau


Contenu :
le dossier programme 2 contient :
- un fichier main.c contenant la quasi-totalité du code source
- un fichier include.h contenant les en-têtes des fonctions, les définitions des structures utilisées,
  les constantes symboliques et les bibliothèques employées
- un fichier check.h contenant une fonction permettant de débugger aisément le code
- un fichier makefile
- le présent fichier texte de présentation




Presentation
Ce programme a pour objectif d'utiliser différentes représentations des informations contenues dans un graphe sous 3 formes : une matrice d'adjacence, une liste d'adjacence et un fichier dot.
Le programme doit être capable de passer d'un format à un autre en utilisant des fichiers textes en entrée et les résultats sont produits sur la sortie standard.


Forme d'un fichier .la (exemple) :


2 9 3 5                <-- ligne 0 : s0 pointe vers s2 avec poids 9 ; s0 pointe vers s3 avec poids 5
3 3                <-- ligne 1 : s1 pointe vers s3 avec poids 3
                <-- ligne 2 : s2 ne pointe vers personne
1 8                <-- ligne 3 : s3 pointe vers s1 avec poids 8


Consigne pour utiliser le programme 2 correctement : il faut specifier le nom du fichier adj ou la qu’on veut lire en premier argument de la ligne de commande