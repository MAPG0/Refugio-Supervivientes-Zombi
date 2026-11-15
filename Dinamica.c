#include <stdio.h>
#include <stdlib.h>

int main(){
int *a;
int *b;
int *Resultado;



a = malloc(sizeof(int));
b = malloc(sizeof(int));
Resultado = malloc(sizeof(int));

if(a==NULL){
printf("no hay memoria disponible");
return 1;
}


if(b==NULL){
printf("no hay memoria disponible");
return 1;
} 
 

if(Resultado==NULL){
printf("no hay memoria disponible");
return 1;
}  

printf("resultado: %d,a:%d b:%d\n", *Resultado,*a,*b);
*a=2;
*b=3;
*Resultado = *a+*b;
printf("resultado: %d,a:%d b:%d\n", *Resultado,*a,*b);
printf("resultado %d\n", *Resultado);

free(a);
free(b);

printf("resultado: %d,a:%d b:%d\n", *Resultado,*a,*b);
free(Resultado);

}
