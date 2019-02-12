#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "thread_traitement.h"


void * Traitement(void * args){
        paramThreadRow* par = (paramThreadRow*)args ; 
		blur_image_row(par->input, par->output, strtoul(par->args[3], 0, 0), par->row);
}
 
int main(int arg_count, char **args)
{
    if(arg_count < 4){
        printf("Nombre de parametre insuffisant");
        exit(-1);
    }
    int i ; 
    struct image input = make_image_from_file(args[1]);
	struct image output = make_image(input.type, input.row_count, input.column_count, input.max_value);
	paramThreadRow paramThread[input.row_count];
    pthread_t T_Thread[input.row_count];

    for(i = 0 ; i<input.row_count;i++){
        paramThread[i].row = i ; 
        paramThread[i].args = args; 
        paramThread[i].input = &input ;  
        paramThread[i].output = &output ; 

    }

    for(i = 0 ; i<input.row_count;i++){
        pthread_create(&T_Thread[i],0,&Traitement,&paramThread[i]);
    }
    for(i = 0 ; i<input.row_count;i++){
        pthread_join(T_Thread[i],NULL);
    }
    

     write_image_to_file(&output,args[2]);
			

}
