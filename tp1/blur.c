#include "image.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int arg_count, char **args)
{
	if(arg_count == 4)
	{
		struct image input = make_image_from_file(args[1]);
		struct image output = make_image(input.type, input.row_count, input.column_count, input.max_value);

		FILE *fr, *fg, *fb;
		struct image output1;
		struct image output2;
		struct image output3;
		fr = tmpfile();
		int childr = fork();
		int childg = fork();
		int childb = fork();
        int status = 0;
        if(childr)
        {   waitpid(childr, &status, 0);
            printf("Je suis dans le processus childr\n");
            if(childg)
            {
                waitpid(childg, &status, 0);
                output1 = make_image_from_stream(fr);

                printf("Je suis dans le processus childg\n");
                if(childb)
                {
                    waitpid(childb, &status, 0);
                    printf("Je suis dans le processus childb\n");
                }
            }
        }
        else
        {
            //waitpid(childr, &status, 0);
            //waitpid(childg, &status, 0);
            //waitpid(childb, &status, 0);
            printf("Je suis dans le processus pere\n");
        }

		blur_image(&input, &output, strtoul(args[3], 0, 0));
		write_image_to_file(&output, args[2]);
	}
	else
	{
		fprintf(stderr, "Essaie plutÃ´t : %s input.ppm output.ppm 10\n", args[0]);
	}
}
