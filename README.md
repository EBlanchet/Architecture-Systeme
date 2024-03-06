Architecture des systèmes informatiques

L'objectif est de proposer une implantation alternative de malloc et free.

Ce mini projet consiste à proposer deux fonctions qui ont le même comportement que malloc et free (c’est à dire allouer et libérer la mémoire) mais qui fait cela sur un segment statique et constant de 1 Giga de mémoire.
Tous les coups sont permis à partir du moment où vous acceptez le challenge (utiliser malloc et free de <stdlib.h>, ce n’est pas accepter le challenge par exemple...). Le but n’est pas de concurrencer la glibc, bien sûr que presque 50 ans d’expérience sur l’exploitation d’un segment contigu de mémoire, c’est plus fort que nous. Le but est plus de prendre ce segment contiguë et de tenter de l’exploiter avec une stratégie raisonnable. Vous connaissez suffisamment d’algorithmique pour adopter une démarche fonctionnelle.
Il est possible de déclarer des structures, il est possible de définir des variables locales statiques et des globales statiques si vous avez une très bonne raison.

Quelques stratégies (certaines sont vilaines, d’autres moins...): /n
• 1970 : L’allocateur primitif K&R. On définit une statique locale dans my_malloc (le haut du tas virtuel dans notre segment mémoire statique), et puis on alloue toujours en haut du tas virtuel qui ne peut être que croissant. Aucun mécanisme n’est prévu pour la libération et c’est très moche ! Mais c’est un allocateur très rapide et qui fonctionne pour donner la mémoire. Cet allocateur a fonctionné pas mal de temps car la plupart des programmes alloue sans jamais libérer (sauf en fin d’exécution). /n
• Avec table logique fixe : Vous définissez une constante ( #define ) qui donnera le nombre maximal de zones. Vous définissez une structure qui enregistre l’adresse, la taille et le statut d’une zone. Vous coupez votre memory pool en deux parties : un tableau de taille fixe d’informations de zones puis ensuite le reste du pool est pour la vraie mémoire de stockage. C’est une stratégie qui ressemble à ce qui peut être fait avec les processus (placés dans une table) ou encore les fichiers (tableau d’inodes). La grosse limitation de cette approche est cette limite de la fragmentation (nombre maximum de morceaux possibles). /n
• fragmentée avec informations contextuelles dans le fragment : On alloue et on libère en fragmentant le pool. Mais chaque fragment porte en tête de l’information contextuelle dans une structure. Les fragments sont donc des fragments logiques et contiennent en leur queue leur vraie zone de stockage. Cette stratégie est très aboutie dans le sens que le surcoût en mémoire contextuel est proportionnel à la fragmentation. Si on fait un seul fragment, on alloue la quasi-totalité du pool. D’un autre coté, cette stratégie n’est pas limité en terme de fragmentation. Il peut y avoir autant de morceaux que possible tant qu’il reste de la place dans le pool. La plupart du temps, les fragments sont organisés sous la forme d’une liste doublement chaînée enracinée au début du pool.
