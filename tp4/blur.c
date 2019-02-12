#include "image.h"
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int arg_count, char **args)
{
	if(arg_count == 4)
	{
		pid_t wpid;
		int status = 0;
		struct image input = make_image_from_file(args[1]);
		struct image output = make_image(input.type, input.row_count, input.column_count, input.max_value);
		FILE * file1 = tmpfile();
		FILE * file2 = tmpfile();
		FILE * file3 = tmpfile();
		
		
		int child = fork();
		if(!child){//fils1
		
				blur_image_layer(&input, &output, strtoul(args[3], 0, 0), 0);
				write_image_to_stream(&output, file1);
				fseek(file1,0,SEEK_SET);
				clear_image(&input);
				clear_image(&output);
				
				
		}else{
		
			int child2 = fork();
			if(!child2){//fils2
				
					blur_image_layer(&input, &output, strtoul(args[3], 0, 0), 1);
					write_image_to_stream(&output, file2);
					fseek(file2,0,SEEK_SET);
					clear_image(&input);
					clear_image(&output);
					
					
			}else{
			
				int child3 = fork();
				if(!child3){//fils3
				
						blur_image_layer(&input, &output, strtoul(args[3], 0, 0), 2);
						write_image_to_stream(&output, file3);
						fseek(file3,0,SEEK_SET);
						clear_image(&input);
						clear_image(&output);
						
						
				}else{
					//pere
						while((wpid = wait(&status)) > 0);

						struct image input1 = make_image_from_stream(file1);
						copy_image_layer(&input1,&output,0);
						clear_image(&input1);

						input1 = make_image_from_stream(file2);
						copy_image_layer(&input1,&output,1);
						clear_image(&input1);

						input1 = make_image_from_stream(file3);
						copy_image_layer(&input1,&output,2);
						clear_image(&input1);
						//dup2 
						//exec
						write_image_to_file(&output,args[2]);
						
				}
			}
		}
		
		
	}
	else
	{
		fprintf(stderr, "Essaie plutôt : %s input.ppm output.ppm 10\n", args[0]);
	}
}
