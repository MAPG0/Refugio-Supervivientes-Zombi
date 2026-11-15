#include <stdio.h>
#include <stdlib.h>
typedef struct{
  int x;
  int y;
}Vector;
Vector* crearVector(int x, int y);
Vector* suma(Vector *uno,Vector *dos);

int main()
{
  Vector *a , *b, *resultado;
  char respuesta='s';
  a = crearVector(2,3);
  b = crearVector(8,1);
  
  if(a==NULL || b==NULL)return 1;
  
  resultado= suma(a,b);
  
  if(resultado==NULL)return 1;
  
  printf("El resultado de la suma de vectores es: (%d, %d) \n ",resultado->x,resultado->y);
  free(a);
  free(b);
  free(resultado);
  
  printf("Quieres Seguir agregando datos al vector???? (S/N)");
  scanf("%c", &respuesta);
  while(respuesta == 's'){
  
  printf("Dame el valor del primer valor del vector");
  scanf("%d", &x);
  
  printf("Dame el valor del primer valor del vector");
  scanf("%d", &y);
  
  a=crearVector(x,y);
  if(a== NULL)return 1;
  
  }
  return 0;
}
Vector* crearVector(int x, int y){
  Vector *ap;
  ap=malloc(sizeof(Vector));
  
  if(ap==NULL){
    return NULL;
  }
  ap ->x=x;
  ap ->y=y;
  return ap;
  
  
}
Vector* suma(Vector *uno,Vector *dos){

  Vector *resultante;
  resultante = crearVector(uno->x + dos->x, uno->y + dos->y);
  if(resultante==NULL)return NULL;
  
  return resultante;
  
}
