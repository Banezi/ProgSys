#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "thread_traitement.h"

#define NB_THREAD 3 

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
