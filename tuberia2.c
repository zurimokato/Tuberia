#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#ifndef BUFFER
#define BUFFER  1024
#endif

void manejador(int sig);
void leerDeArchivo(int tub[],char cadena[],char nombreArchivo[]);
void imprimirMensaje(int tub[],char cadena[]);

int main(){
	pid_t hijos[2], padre;
    int tub1[2],tub2[2];
    char cadena[BUFFER];
    pipe(tub1);
    pipe(tub2);
    padre=getpid();

    for(int i=0;i<2;i++){
        if(!(hijos[i]=fork()))
            break;
    }

    if(padre==getpid()) {
    	for (int i = 0; i < 2; i++){
    		wait(NULL);
    	}
    	imprimirMensaje(tub1,cadena);
    	imprimirMensaje(tub2,cadena);
    }else{
    	//pause();
      	if(hijos[0]==0){
	   			leerDeArchivo(tub1,cadena,"archivo.txt");
	            
	    }else{
	    	if(hijos[1]==0){
	    		leerDeArchivo(tub2,cadena,"archivo2.txt");
	    	}
	    }
    }
  	
	return 0;
}


void leerDeArchivo(int tub[],char cadena[],char nombreArchivo[]){
	int i=0;
   	close(tub[0]);
    FILE *txt=fopen(nombreArchivo,"r");
    if(txt==NULL){
        printf("Error al abrir el archivo\n");
    }else{
        while(!feof(txt)){
           	cadena[i] = fgetc(txt);
            i++;
       	}
        cadena[i]='\0';
        write(tub[1], cadena, BUFFER);
       	printf("archivo enviado a ,%d\n", getppid());
        close(tub[1]);
        fclose(txt);
    }
}
void imprimirMensaje(int tub[],char cadena[]){
	close(tub[1]);
    int n=read(tub[0],cadena,BUFFER);
    cadena[n]='\0';
    printf("El proceso [%d] ha recibido del hijo: %s\n", getpid(), cadena);
    close(tub[0]);
}