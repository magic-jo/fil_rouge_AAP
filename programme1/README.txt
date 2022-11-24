Consignes pour exécuter le programme 1 correctement :

 - ranger les fichiers .adj directement dans le répertoire de travail

 - ne pas mettre de point (.) dans le nom du fichier .adj sauf dans la particule finale .adj

 - ligne de commande type pour exécuter le programme et afficher les images png :
    ./programme1.exe ./graph.adj sommetInitial sommetFinal
        --> ne surtout pas oublier le ./ devant graph.adj

 - les fichiers dot et leur image png correspondante seront produits ensemble dans le répertoire spécifié par DOT_PATH

 - spécifier la valeur de DOT_PATH en utilisant la commande suivante :
    export DOT_PATH=./nomdudossier

 - les graphes sont numérotés à partir de 1 ; ne pas mettre 0 en argument de la commande

 - attention au cas où un sommet n'a pas de prédecesseur ; si c'est  le cas, ne pas passer ce sommet comme paramètre pour sommetFinal
