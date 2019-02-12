# ProgSys
Programmation Système

Exécution d'un programme en ligne de commande

$  gcc horlogue.c -o horlogue
$ ./horlogue

$ gcc essai.c -Wall Afficher les warnings à la compilation

Commandes de gestion des processus
Pour afficher ses propres processus en cours d'exécution
$ ps 

Pour afficher tous les processus en cours d'exécution, on peut utiliser l'option aux (a : processus de tous les utilisateurs ; u : affichage détaillé ; x : démons) :
$ ps aux

Création de nouveaux processus en C
Inclure la bibliothèque 
#include <unistd.h> pour fork()
<sys/types.h> pour pid_t
#include<errno.h> pour la variable globale errno
#include <assert.h> pour assert
#include <ys/wait.h> pour wait

pid_t fork(); // on peut rempacer pid_t par un int

La valeur renvoyée par fork est de :

    -1 si il y a eu une erreur ;

    0 si on est dans le processus fils ;

    Le PID du fils si on est dans le processus père. Cela permet ainsi au père de connaître le PID de son fils.

Dans le cas où la fonction a renvoyé -1 et donc qu'il y a eu une erreur, le code de l'erreur est contenue dans la variable globale  errno , déclarée dans le fichier  errno.h(n'oubliez pas le #include<errno.h>). Ce code peut correspondre à deux constantes :

    ENOMEM : le noyau n'a plus assez de mémoire disponible pour créer un nouveau processus ;

    EAGAIN : ce code d'erreur peut être dû à deux raisons : soit il n'y a pas suffisamment de ressources systèmes pour créer le processus, soit l'utilisateur a déjà trop de processus en cours d'exécution. Ainsi, que ce soit pour l'une ou pour l'autre raison, vous pouvez rééditer votre demande tant queforkrenvoie EAGAIN.

La fonction getpid retourne le PID du processus appelant
pid_t getpid();

La fonctiongetppidretourne le PPID du processus appelant
pid_t getppid();

Le cast
(int) getpid()
Pour afficher les variables de type pid_t dans printf();

Terminaison d'un processus

void exit(status);
status = EXIT_SUCCESS, EXIT_FAILURE, 0, -1 etc.

Pour quitter, de manière propre, un programme, lorsqu'un bug a été détecté, on utilise la fonction : 

void abort();

Un prototype simple pour une fonction qui possède un défaut majeur : il est difficile de savoir à quel endroit du programme le bug a eu lieu.
Pour y remédier, il est préférable d'utiliser la macro assert, déclarée qui fonctionne comme suit.

    Elle prend en argument une condition.

    Si cette condition est vraie,assertne fait rien.

    Si elle est fausse, la macro écrit un message contenant la condition concernée, puis quitte le programme. Cela permet d'obtenir une bonne gestion des erreurs.

assert(condition);
Si la condition est vraie, le programme continue sinon on quitte le programme avec un message d'erreur.

Synchronisation entre père et fils

Lorsque le processus fils se termine avant le processus père, il devient un zombie .
Pour permettre à un processus fils en état zombie de disparaître complètement, on utilise la fonction wait, qui se trouve dans la bibliothèque sys/wait.h, déclarée comme ceci :
    
    pid_t wait(int *status);
    wait(NULL); si on veut pas traiter le code de retour du processus terminé

Lorsque l'on appelle cette fonction, cette dernière bloque le processus à partir duquel elle a été appelée jusqu'à ce qu'un de ses fils se termine. Elle renvoie alors le PID de ce dernier (le pid du fils terminé).
En cas d'erreur, la fonction renvoie la valeur -1.

Le paramètre status correspond au code de retour du processus(processus fils terminé). Autrement dit, la variable que l'on y passera aura la valeur du code de retour du processus (ce code de retour est généralement indiquée avec la fonction exit).

Attention : il faut mettre autant dewaitqu'il y a de fils

Attendre la fin de n'importe quel processus
Il existe également une fonction qui permet de suspendre l'exécution d'un processus père jusqu'à ce qu'un de ses fils, dont on doit passer le PID en paramètre, se termine. Il s'agit de la fonction waitpid :
    
    pid_t waitpid(pid_t pid, int *status, int options);

Plus précisément, la valeur depidest interprétée comme suit :

    si pid > 0, le processus père est suspendu jusqu'à la fin d'un processus fils dont le PID est égal à la valeur pid ;

    si pid = 0, le processus père est suspendu jusqu'à la fin de n'importe lequel de ses fils appartenant à son groupe ;

    si pid = -1, le processus père est suspendu jusqu'à la fin de n'importe lequel de ses fils ;

    si pid < -1, le processus père est suspendu jusqu'à la mort de n'importe lequel de ses fils dont le GID est égal.

Le second argument,status, a le même rôle qu'avecwait.

Le troisième argument permet de préciser le comportement de waitpid. On peut utiliser deux constantes :

    WNOHANG : ne pas bloquer si aucun fils ne s'est terminé.

    WUNTRACED : recevoir l'information concernant également les fils bloqués si on ne l'a pas encore reçue.

Dans le cas où cela ne vous intéresse pas, il suffit de mettre le paramètre 0.

Notez que waitpid(-1, status, 0) correspond à la fonctionwait.