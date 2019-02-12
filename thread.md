Le mot « thread » est un terme anglais qui peut se traduire par « fil d'exécution ». L'appellation de « processus léger » est également utilisée.

Toutes les fonctions relatives aux threads sont incluses dans le fichier d'en-tête <pthread.h> et dans la bibliothèque libpthread.a (soit -lpthread à la compilation en ligne de commande).

Exemple :

Écrivez la ligne de commande qui vous permet de compiler votre programme sur les threads constitué d'un seul fichier main.c et avoir en sortie un exécutable nommé monProgramme.

Correction :

gcc -lpthread main.c -o monProgramme

Et n'oubliez pas d'ajouter #include <pthread.h> au début de vos fichiers.

Créer un thread

Pour créer un thread, il faut déjà déclarer une variable le représentant. Celle-ci sera de type pthread_t (qui est, sur la plupart des systèmes, un typedef d'unsigned long int).
#include <pthread.h>
    pthread_t monthread;

Ensuite, pour créer la tâche elle-même, il suffit d'utiliser la fonction :

int pthread_create(pthread_t * thread, pthread_attr_t * attr, void *(*start_routine) (void *), void *arg)

    La fonction renvoie une valeur de type int : 0 si la création a été réussie ou une autre valeur si il y a eu une erreur.

    Le premier argument est un pointeur vers l'identifiant du thread (valeur de type pthread_t).

    Le second argument désigne les attributs du thread. Vous pouvez choisir de mettre le thread en état joignable (par défaut) ou détaché, et choisir sa politique d'ordonnancement (usuelle, temps-réel...). Dans nos exemple, on mettra généralement NULL.

    Le troisième argument est un pointeur vers la fonction à exécuter dans le thread. Cette dernière devra être de la forme void *fonction(void* arg) et contiendra le code à exécuter par le thread.

    Enfin, le quatrième et dernier argument est l'argument à passer au thread.

Supprimer un thread
    void pthread_exit(void *ret);
    Elle prend en argument la valeur qui doit être retournée par le thread, et doit être placée en dernière position dans la fonction concernée.
    pthread_exit(NULL); si on a va pas traiter la valeur de retour

Attendre la fin d'un thread
    int pthread_join(pthread_t th, void **thread_return);
    Elle prend donc en paramètre l'identifiant du thread et son second paramètre, un pointeur, permet de récupérer la valeur retournée par la fonction dans laquelle s'exécute le thread (c'est-à-dire l'argument de pthread_exit).