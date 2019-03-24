#ifndef __DECLARATION_THREAD_
#define __DECLARATION_THREAD_
#include "image.h"


struct ParamThread{

int couche ; 
struct image *input ;
char ** args ; 
struct image *output ; 


};

struct ParamThreadRow{

int row ; 
struct image *input ;
char ** args ; 
struct image *output ; 


};

struct ParamThreadCol{

int col ; 
struct image *input ;
char ** args ; 
struct image *output ; 


};


typedef struct ParamThread ParamThread;
typedef struct ParamThreadRow paramThreadRow;
typedef struct ParamThreadCol paramThreadCol;


#endif