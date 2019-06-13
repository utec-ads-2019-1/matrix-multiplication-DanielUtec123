//
// Created by usuario on 13/06/2019.
//

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>
#include<sys/time.h>

#define NUM_HILOS 32
double **matriz1;
double **matriz2;
double **matriz3;

struct datos
{
    long thread_id;
    int num_filas;
    int num_columnas;
};

struct datos datos_pasar[NUM_HILOS];

void allocar_memoria(int filas,int columnas);
void imprimir_matrix(int filas,int columnas);
void llenar_numeros(int filas,int columnas);

void *calcular (void *parametros)
{
    int i,j,k,filas,columnas,lim_inf,lim_sup;
    long t;
    struct datos *mis_datos;
    mis_datos=(struct datos *) parametros;

    t= mis_datos->thread_id;
    filas= mis_datos->num_filas;
    columnas=mis_datos->num_columnas;

    lim_inf=t*(filas/NUM_HILOS);
    lim_sup=(t+1)*(filas/NUM_HILOS);

    printf("Hilo %ld, LIM inf %d LIM sup %d \n",t,lim_inf,lim_sup);
    for (i=lim_inf;i<lim_sup;i++)
    {

        for (j=0;j<columnas;j++)
        {
            int temporal=0;
            for(k = 0;k<filas;k++){
                temporal+=matriz1[i][k]*matriz2[k][j];
                matriz3[i][j]= temporal;


            }
        }
    }

    pthread_exit(NULL);
}

int main()
{

    struct timeval start;
    struct timeval finish;
    long compTime;
    double Time;



    int i,j,k;
    int m,n,rc;
    long t;
    double var1,var2;
    void *status;

    pthread_t hilos[NUM_HILOS]; //declaro mi arreglo de hilos

    pthread_attr_t at;
    pthread_attr_init(&at);
    pthread_attr_setdetachstate(&at,PTHREAD_CREATE_JOINABLE);



    printf("Multiplicacion de Matrices\n");
    printf("Cuantas filas tiene la matriz 1 ?\n");
    scanf("%d",&m);
    printf("Cuantas columnas tiene la matriz 1 ?\n");
    scanf("%d",&n);
    printf("La Matriz 1 es %d x %d \n",m,n);
    printf("La Matriz 2 es %d x %d \n",m,n);
    printf("La Mtraiz 3 es %d x %d \n",m,n);

    allocar_memoria(m,n);
    llenar_numeros(m,n);
//imprimir_matrix(n,n);
    gettimeofday(&start,0);
    double ti = start.tv_sec;

    printf("tiempo inicial %f \n",ti);

    for (t=0;t<NUM_HILOS;t++)
    {

        datos_pasar[t].thread_id=t;
        datos_pasar[t].num_filas=m;
        datos_pasar[t].num_columnas=n;

        printf("Creando el hilo %ld \n",t);
        rc=pthread_create(&hilos[t],&at,calcular,(void*)&datos_pasar[t]);
        if (rc)
        {printf("ERROR codigo %d \n",rc);
            exit(-1);}
    }

    pthread_attr_destroy(&at);

    for(t=0;t<NUM_HILOS;t++)
    {
        rc=pthread_join(hilos[t],&status);
        if(rc){
            printf("ERROR,codigo %d \n",rc);
            exit(-1);
        }
        printf("acabo el hilo %ld \n",t);
    }

    gettimeofday(&finish,0);

    compTime=(finish.tv_sec - start.tv_sec)*1000000;
    compTime=compTime+(finish.tv_usec - start.tv_usec);
    Time=(double)compTime;

    ti = finish.tv_sec;

    printf("tiempo final %f\n", ti);

    printf("tiempo de ejecucion  %lf  \n",(double)Time/1000000.0);


    imprimir_matrix(m,n);
    printf("Despues de multiplicar \n");
    printf("stiempo de ejecucion %lf\n",(double)Time/1000000.0);
    printf("Fin del programa \n");
    pthread_exit(NULL);
    return 0;
}//fin funcion principal

void allocar_memoria(int filas,int columnas)
{
    int i,j,k;
    matriz1=(double**)malloc(filas*sizeof(double*));
    for (i=0;i<filas;i++)
    {
        matriz1[i]=(double*)malloc(columnas*sizeof(double));
    }

    matriz2=(double**)malloc(filas*sizeof(double*));
    for (i=0;i<filas;i++)
    {
        matriz2[i]=(double*)malloc(columnas*sizeof(double));
    }

    matriz3=(double**)malloc(filas*sizeof(double*));
    for (i=0;i<filas;i++)
    {
        matriz3[i]=(double*)malloc(columnas*sizeof(double));
    }

} //Fin de Allocar Memoria

void Multiplicar_Matrices(){




}





void imprimir_matrix(int filas,int columnas)
{
    int i,j,k;
    printf("MATRIX 1\n");
    for (i=0;i<filas;i++)
    {
        for (j=0;j<columnas;j++)
        {
            printf("%lf \t",matriz1[i][j]);
        }
        printf("\n");
    }
    printf("MATRIX 2\n");
    for (i=0;i<filas;i++)
    {
        for (j=0;j<columnas;j++)
        {
            printf("%lf \t",matriz2[i][j]);
        }
        printf("\n");
    }
    printf("MATRIX 3\n");
    for (i=0;i<filas;i++)
    {
        for (j=0;j<columnas;j++)
        {
            printf("%lf \t",matriz3[i][j]);
        }
        printf("\n");
    }
}//Fin de Imprimir Arreglos



void llenar_numeros(int filas, int columnas)
{
    int i,j,k;
    for (i=0;i<filas;i++)
    {
        for (j=0;j<columnas;j++)
        {
            matriz1[i][j]=j*3+2*(rand()%15) ;
            matriz2[i][j]=j*3+2*(rand()%15) ;

        }

    }

}