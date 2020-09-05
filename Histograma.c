//
//  Histograma.c
//  Actividad 3 E2
//
//  Created by Christian Alberto Dalma Schults, Octavio Iván Garduza Labastida, Luis Armando Revilla and Antonio Junco de Haas on 02/09/20.
//  Copyright © 2020 Christian Alberto Dalma Schultz. All rights reserved.
//

#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>


/*-----------
 -Metodos
-------------*/
 
long long int* listFilesRecursively(char *path, long long int * vector);

unsigned char statFileType(char *fname);

int stat(const char*path, struct stat *buf);

long long int *revisarUrnas( long long int * vector, int valor);

char *getFullName(char *ruta, struct dirent *ent);

void printVector(long long int *vector);

long long int getUrna(long long int *vector);

long long int getNumero(long long int *vector);

long long int *setNumero(long long int *vector, int nuevonum);

long long int *clearNew(long long int *vector, int lastvalue);

long long int *accumulate(long long int *vector, int position);

int maximoArchivos (long long int *vector);

void imprimirHistograma(long long int *vector);



int main(int argc, char *argv[]){

  int urna=1024;
  int num_urnas=1;
  int limite=urna*num_urnas;

  long long int * vector = (long long int *) malloc(3 * sizeof(long long int));
    long long int * final = vector + 3;

  int coun=0;
  for (long long int * aux = vector; aux < final; ++aux) {
        if(coun==0){
      *aux=num_urnas;
        }
            else if (coun==1){
            *aux=urna;
        }
    coun++;
    }
    
    /*-----------------
     - Metodos
     ------------------*/

  getUrna(vector);
  getNumero(vector);


  struct dirent *pDirent;
  DIR *pDir;

  if (argc != 2) {
      printf ("Usage: testprog <dirname>\n");
      return 1;
  }

  pDir = opendir (argv[1]);
  if (pDir == NULL) {
      printf ("Cannot open directory '%s'\n", argv[1]);
      return 1;
  }

  vector=listFilesRecursively(argv[1],vector);

  imprimirHistograma(vector);
  free(vector);
  closedir(pDir);
}

long long int * listFilesRecursively(char*basePath, long long int *vector){
  char path[1000];
  struct dirent *dp;
  DIR *dir=opendir(basePath);
  if (!dir){
    return vector;
  }
  while ((dp=readdir(dir))!=NULL){
    if (strcmp(dp->d_name, ".")!=0&&strcmp(dp->d_name,"..")){
        
      struct stat st;
      stat(dp->d_name,&st);
      long long int size=st.st_size;

      char *nombrecompleto=getFullName(basePath, dp);
      unsigned char a=statFileType(nombrecompleto);
      if(a==DT_DIR){
          return 0;
      }
      else{
        struct stat data;
        stat(nombrecompleto,&data);
        vector=revisarUrnas(vector, (long long)data.st_size);
      }

      strcpy(path, basePath);
      strcat(path,"/");
      strcat(path,dp->d_name);
      vector=listFilesRecursively(path,vector);
    }
  }
  return vector;
  closedir(dir);
}


/*
 -Prueba de stat(), si no devuelve
 -num desocnocido
 */
unsigned char statFileType(char *fname)
{
  struct stat sdata;
  if (stat(fname, &sdata)==-1)
    {
      return DT_UNKNOWN;
    }

  switch (sdata.st_mode & S_IFMT)
    {
    case S_IFBLK:  return DT_BLK;
    case S_IFCHR:  return DT_CHR;
    case S_IFDIR:  return DT_DIR;
    case S_IFIFO:  return DT_FIFO;
    case S_IFLNK:  return DT_LNK;
    case S_IFREG:  return DT_REG;
    case S_IFSOCK: return DT_SOCK;
    default:       return DT_UNKNOWN;
    }
}

char *getFullName(char *ruta, struct dirent *ent)
{
  char *nombrecompleto;
  int tmp;

  tmp=strlen(ruta);
  nombrecompleto=malloc(tmp+strlen(ent->d_name)+2);
  if (ruta[tmp-1]=='/')
    sprintf(nombrecompleto,"%s%s", ruta, ent->d_name);
  else
    sprintf(nombrecompleto,"%s/%s", ruta, ent->d_name);
  return nombrecompleto;
}

/*
-Revision de urnas para saber las inidcadas
 */
long long int * revisarUrnas(long long int * vector, int valor){
  
  int urna= getUrna(vector);
  int cantidad=getNumero(vector);
  int limite=(urna*cantidad);
  int lastvalue=getNumero(vector);

  while (valor>=limite){
    cantidad++;
    limite=urna*cantidad;
      
    vector = (long long int *) realloc(vector, sizeof(long long int) * (cantidad+2));
    vector=setNumero(vector, cantidad);
    vector =clearNew(vector, cantidad);
  }

  /*
   -Insercion de la urna
   */
  int pos=valor/urna;
  long long int * final = vector + cantidad+2;
  int counter=0;
  long long int * naux;
  int flag=0;
  for (naux = vector; naux < final; ++naux) {

    if ((counter>(pos+1))&&(flag==0)){
      *naux=(*naux)+1;
      vector=accumulate(vector,counter);
      flag=1;
    }
    counter++;
  }
  return(vector);
}

/*
 -Print del vector
 */

void printVector(long long int *vector){
  printf("\n\nprinting vector: \n");
  long long int * aux = vector;
  long long int *final=vector + *aux+2;
  int counter=0;

  for (aux; aux < final; ++aux) {
        printf("counter: %d value: %lld\n",counter,*aux);
    counter++;
    }
}

/*
 -Metodo que regresa el valor de las urnas
 */
long long int getUrna(long long int *vector){
  long long int * aux = vector;
  long long int *final=++aux;
  //printf("Urna: %lld\n",*final);
  return *final;

}

/*
-Metodo que regresa el numero en el vector
-metodos set y get
*/
long long int getNumero(long long int *vector){
  long long int * aux = vector;
  return *aux;
}

long long int *setNumero(long long int *vector, int nuevonum){
  *vector=nuevonum;
  return vector;
}

long long int *clearNew(long long int *vector, int lastvalue){
  int count=0;
  long long int *aux=vector;
  long long int *final=vector + *aux+2;
  for (aux=vector; aux < final; ++aux) {
    if (count>lastvalue){
          *aux=0;
    }
    count++;
    }
  return vector;
}


long long int *accumulate(long long int *vector, int position){
  int count=0;
  long long int *aux=vector;
  long long int *final=vector + *aux+2;
  for (aux=vector; aux < final; ++aux) {
    if (count==(position+2)) {
          *aux++;;
    }
    count++;
    }
  return vector;

}

int maximoArchivos(long long int *vector){
  int max=0;
  int count=0;
  long long int *aux=vector;
  long long int *final=vector + *aux+2;
  for (aux=vector; aux < final; ++aux) {
    if (count>=2) {
      if(max<*aux)
        max=*aux;
    }
    count++;
    }
  return max;
}

/*
 -Metodo para imprimir el histograma
 */
void imprimirHistograma(long long int *vector){
  int max=maximoArchivos(vector);
  double asteriscos=40.0;
  int total=getNumero(vector);
  int urna=getUrna(vector);
  double array [total];
  int array2[total];

  int count=0;
  long long int *aux=vector;
  long long int *final=vector + *aux+2;
  for (aux=vector; aux < final; ++aux) {
    if (count>=2) {
      array[count-2]=(*aux*asteriscos)/max;
      array2[count-2]=*aux;
    }
    count++;
    }
    
    
  printf("%19s %11s %10s\n","Urnas"," Num de Archivos","Histograma");
  for(int i=0;i<total;i++){
    printf("%8d - %8d",i*urna,(i+1)*urna-1);
    printf("%15d",array2[i]);
    while(array[i]>0){
      printf("*");
      array[i]--;
    }
    printf("\n");
  }
}
