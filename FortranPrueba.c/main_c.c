#include <stdio.h>
#include <stdlib.h>

void print_float_array(float* __Array__, int dim);

int main (void){
  float *Array=(float*) malloc(5*sizeof(float));
  int a=0;
  while (a<5) {*(Array+a)=a+1;++a;}
  print_float_array(Array,5);
  free(Array);
  return 0;
}
