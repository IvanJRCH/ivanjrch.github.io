#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "genann.h"
#include "stb_image.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char **argv)
{
  // Se declara una red neuronal con 3 entradas, 16 neuronas ocultas y 1 salida.
  genann *ann = genann_init(3, 16, 1, 0.1);


// Se cargan las imágenes.
  int width = 0;
  int height = 0;
  unsigned char *imageData = _AQUI_
  int inputsize = width * height;


// Se declara un vector de 3 entradas.
  double *inputs = malloc(input_size * sizeof(double));


// Se alimenta la red neuronal con los datos de las imágenes.
  for (int i = 0; i < input_size; i++) {
    inputs[i] = (double)imageData[i];
  }


// Se realiza el entrenamiento de la red neuronal.
  int iterations = 500;
  genann_train(ann, inputs, NULL, iterations);


// Se realiza una predicción.
  genann_predict(ann, inputs);


// Se imprimen los resultados del entrenamiento.
  printf("Resultados del entrenamiento:\n");
  for (int i = 0; i < input_size; i++) {
    printf("Valor de entrada %d: %f\n", i, inputs[i]);
  }
  printf("Valor de salida: %f\n", ann->out[0]);


// Se libera la memoria de la red neuronal.
  genann_free(ann);


// Se libera la memoria del vector de entradas.
  free(inputs);


return 0;
}

