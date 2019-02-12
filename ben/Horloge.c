#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define secondePerMinute 60
#define MinutePerHoure 60

int seconde = 0 , minute = 0 , heure = 0 , temps = 0 ; 
 
pthread_cond_t cond_Seconde = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_Minute = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_Heure = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* initialisation_time(void* args){
    pthread_mutex_lock(&mutex);
    time_t t;
	temps = time(&t);
    printf("Temps : %d",temps);
    pthread_cond_signal(&cond_Seconde);
    pthread_mutex_unlock(&mutex);

}
void* CalculeSeconde(void* args){

    while(1)
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond_Seconde,&mutex);
        printf("%d:%d:%d",heure,minute,seconde);
        if(seconde >= secondePerMinute){
            pthread_cond_signal(&cond_Minute);
            seconde = 0 ; 
        }
        
        else 
            seconde++;
        
         pthread_mutex_unlock(&mutex);   
    }

}

void* CalculeMinute(void* args){
       while(1){ 
            pthread_mutex_lock(&mutex);
            pthread_cond_wait(&cond_Minute,&mutex);
            printf("%d:%d:%d",heure,minute,seconde);
            if(minute >= MinutePerHoure){
                pthread_cond_signal(&cond_Heure);
                minute = 0 ; 
            }
            
            else 
                minute++;
            
            pthread_mutex_unlock(&mutex); 
         }
         
}

void* CalculeHeure(void* args){
       while(1)
       { 
            pthread_mutex_lock(&mutex);
            pthread_cond_wait(&cond_Heure,&mutex);
            printf("BYE BYE");

            pthread_mutex_unlock(&mutex); 
            }
          
}
int main(){
    pthread_t t_seconde , t_minute,t_heure , t_initialisationTime ; 


    pthread_create(&t_initialisationTime , NULL , &initialisation_time , NULL);
    pthread_create(&t_seconde , NULL , &CalculeSeconde , NULL);
    pthread_create(&t_minute , NULL , &CalculeMinute , NULL);
    pthread_create(&t_heure , NULL , &CalculeHeure , NULL);

    pthread_join(t_initialisationTime,NULL);
    pthread_join(t_seconde,NULL);
    pthread_join(t_minute,NULL);
    pthread_join(t_heure,NULL);


    


        return 0  ; 

}