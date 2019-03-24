#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "thread_traitement.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define NB_THREAD 3 


//Ecriture dans un fichier binaire
void do_write(char const *file)
{
	int fd = open(file, O_RDWR | O_CREAT, 0600); // ouverture
	ftruncate(fd,100); //redimenionnement
	int *memory = mmap(0,100, PROT_WRITE, MAP_SHARED, fd, 0); //attachement
	memory[2] = 4; // ecriture
	memory[5] = 25; // ecriture
	munmap(memory,100); // détachement
	close(fd);// fermeture
}

//Lecture dans un fichier bianaire
void do_read(char const *file)
{
	int fd = open(file, O_RDONLY); //ouverture
	int *memory = mmap(0,100,PROT_READ, MAP_SHARED, fd,0); //attachement
	//traitement
	printf("2² = %d\n5² = %d\n", memory[2], memory[5]); //lecture
	munmap(memory,100); //detachement
	close(fd); //fermeture
}

//Memoire partagée publique
void parent(int child)
{
	int fd = open("image1.pmm", O_RDWR | O_CREAT, 0600); // ouverture
	char *memory = mmap(0, 100, PROT_READ, MAP_SHARED, fd, 0);
	sleep(1);
	printf("%d: message de %d: %s\n", getpid(), child, memory);// lecture
	unlink("image1.pmm"); // suppression
}
void child()
{
	int fd = open("image1.pmm", O_RDWR | O_CREAT, 0600);// ouverture
	char *memory = mmap(0, 100, PROT_WRITE, MAP_SHARED, fd, 0);
	ftruncate(fd, 100);// redimensionnement
	snprintf(memory, 100,"je suis %d", getpid());// écriture
}


void * Traitement(void * args){
        ParamThread* par = (ParamThread*)args ; 
		blur_image_layer(par->input, par->output, strtoul(par->args[3], 0, 0), par->couche);

}
 
int main(int arg_count, char **args)
{
    if(arg_count < 4){
        printf("Nombre de parametre insuffisant");
        exit(-1);
    }
	//partage de la memoire
	int fd = open("image1blur.pmm", O_RDWR | O_CREAT, 0600); // ouverture
	char *memory = mmap(0, 100, PROT_READ, MAP_SHARED, fd, 0);

    int i ; 
    pthread_t T_Thread[NB_THREAD];
    ParamThread paramThread[NB_THREAD];
    struct image input = make_image_from_file(args[1]);
	struct image output = make_image(input.type, input.row_count, input.column_count, input.max_value);
	
    for(i = 0 ; i<NB_THREAD;i++){
        paramThread[i].couche = i ; 
        paramThread[i].args = args; 
        paramThread[i].input = &input ;  
        paramThread[i].output = &output ; 

    }

    for(i = 0 ; i<NB_THREAD;i++){
        pthread_create(&T_Thread[i],0,&Traitement,&paramThread[i]);
    }
    for(i = 0 ; i<NB_THREAD;i++){
        pthread_join(T_Thread[i],NULL);
    }
    

     write_image_to_file(&output,args[2]);
			

}

//Commande d'exécution du programme
/* 
	gcc -lpthread blurmapping.c -o mapping
	./mapping image1.ppm image1modif.ppm 10 10
*/