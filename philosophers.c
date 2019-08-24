#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N_FIL 4
#define SLP_TIME 250
#define HGR_TIME 250

pthread_mutex_t tenedor1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tenedor2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tenedor3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tenedor4 = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t *arrMutex[4]; 


void *funcion_th(void *i){

	int *id = (int *)i;
	int hambre = 0;

	for(;;){

	switch(*id){

		case 0:

				if(hambre == 0){

					for(int i = 0; i < rand() %11; i++){

      					printf("%d piensa\n",*id );
						usleep(SLP_TIME);
					}
					hambre += 1;
			 	}else if(hambre >= 4){

			 		exit(EXIT_FAILURE);

			 	}else{

			 		for(int i = 0; i < rand()%11;i++){

			 			printf("%d hambrea\n", *id);
			 			usleep(HGR_TIME);
			 		}
			 	hambre += 1;

			 	}


				if(pthread_mutex_trylock(arrMutex[0])){//Si puede bloquear el izquierdo

					if(pthread_mutex_trylock(arrMutex[1])){//Si puede bloquear el derecho
											
							for(int i = 0; i < rand() %11; i++){
								printf("Filósofo %d comiendo\n",*id);
								usleep(SLP_TIME);
							}
					hambre = 0;

					}
				}



		case 1:

				if(hambre == 0){

					for(int i = 0; i < rand() %11; i++){

      					printf("%d piensa\n",*id );
						usleep(SLP_TIME);
					}
					hambre += 1;
			 	}else if(hambre >= 4){

			 		exit(EXIT_FAILURE);

			 	}else{

			 		for(int i = 0; i < rand()%11;i++){

			 			printf("%d hambrea\n", *id);
			 			usleep(HGR_TIME);
			 		}
			 		hambre += 1;

			 	}


				if(pthread_mutex_trylock(arrMutex[1])){//Si puede bloquear el izquierdo

					if(pthread_mutex_trylock(arrMutex[2])){//Si puede bloquear el derecho
											
							for(int i = 0; i < rand() % 11; i++){
								printf("Filósofo %d comiendo\n",*id);
								usleep(SLP_TIME);
							}
					hambre = 0;

					}
				}



		case 2:

				if(hambre == 0){

					for(int i = 0; i < rand() %11; i++){

      					printf("%d piensa\n",*id );
						usleep(SLP_TIME);
					}
					hambre += 1;
			 	}else if(hambre >= 4){

			 		exit(EXIT_FAILURE);

			 	}else{

			 		for(int i = 0; i < rand()%11;i++){

			 			printf("%d hambrea\n", *id);
			 			usleep(HGR_TIME);
			 		}
			 		hambre += 1;

			 	}


				if(pthread_mutex_trylock(arrMutex[2])){//Si puede bloquear el izquierdo

					if(pthread_mutex_trylock(arrMutex[3])){//Si puede bloquear el derecho
											
							for(int i = 0; i < rand() % 11; i++){
								printf("Filósofo %d comiendo\n",*id);
								usleep(SLP_TIME);
							}
					hambre = 0;

					}
				}

 

		case 3:
				if(hambre == 0){

					for(int i = 0; i < rand() %11; i++){

      					printf("%d piensa\n",*id );
						usleep(SLP_TIME);
					}
					hambre += 1;
			 	}else if(hambre >= 4){

			 		exit(EXIT_FAILURE);

			 	}else{

			 		for(int i = 0; i < rand()%11;i++){

			 			printf("%d hambrea\n", *id);
			 			usleep(HGR_TIME);
			 		}
			 		hambre += 1;

			 	}

				if(pthread_mutex_trylock(arrMutex[3])){//Si puede bloquear el izquierdo

					if(pthread_mutex_trylock(arrMutex[0])){//Si puede bloquear el derecho
											
							for(int i = 0; i < rand() % 11; i++){
								printf("Filósofo %d comiendo\n",*id);
								usleep(SLP_TIME);
							}
					hambre = 0;

					}
				}


		default:
		;

	}

	}

	return 0;
}

int main(){



	pthread_t id[N_FIL];
	int i[N_FIL];
	arrMutex[0] = &tenedor1;
	arrMutex[1] = &tenedor2;
	arrMutex[2] = &tenedor3;
	arrMutex[3] = &tenedor4;

	//Crear procesos
	for (int j=0; j< N_FIL; j++)
	{
		i[j]=j;
		pthread_create (&(id[j]), NULL, funcion_th, (void *)&i[j]);
	}

	for (int j=0; j<N_FIL; j++)
	{
    	pthread_join (id[j], NULL);
    }



	exit (EXIT_SUCCESS);

}
