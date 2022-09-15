#include <stdio.h>
#include "math_matriz.h"
#include <math.h>

void multiplicar_matriz(int fil_A,int col_A, double A[][col_A],int fil_B, int col_B, double B[][col_B], double **C)
{
	if (col_A == fil_B){
        for(int i=0; i<fil_A; ++i){
			for(int j=0; j<col_B; ++j){
				double Cij = 0;
				for(int z=0; z<col_A; ++z)
					Cij += A[i][z] * B[z][j];
				C[i][j] = Cij;
			}
        }
	}
	else{
		printf("Error: dimension de las matrices de entrada incorrecta");
	}
}

void sumar_matriz(int fil_A,int col_A, double A[][col_A],int fil_B, int col_B, double B[][col_B], double **C){
    printf("\n*** SUMA *** \n");
    if (fil_A == fil_B && col_A == col_B){
        for(int i=0; i<fil_A; ++i){
			for(int j=0; j<col_B; ++j){
				C[i][j] = A[i][j] +  B[i][j];;
			}
        }

    }
    else{
		printf("Error: dimension de las matrices de entrada incorrecta");
    }

}

void mostrarMatriz(int filas, int columnas,double mA[][columnas],char *name){
	printf("%s = \n[",name);
    for(int i=0; i<filas; ++i)
    {
        for(int j=0; j<columnas; ++j)
        {
            printf("%f\t", mA[i][j]) ;
        }
        if (i != filas -1)
        	printf(";\n");
    }
    printf("]\n");
}

void mostrarMatrizp(int filas, int columnas, double **matriz,char *name){
	printf("%s = \n[",name);
    for(int i=0; i<filas; ++i)
    {
        for(int j=0; j<columnas; ++j)
        {
            printf("%f \t", matriz[i][j]) ;
        }
        if (i != filas -1)
        	printf(";\n");
    }
    printf("]\n");
}

void mostrarMatrizR3_int(int filas,int columnas,int capas, int ***matriz, char *name){

	for (int k = 0;k<capas;++k){
		printf("\n %s(:,:,%d)\n",name,k);
		for (int i = 0;i<filas;++i){
			for (int j = 0;j<columnas;++j){
				printf("%d \t",matriz[i][j][k]);
			}
			printf("\n");
		}
	}
}

void mostrarMatrizR3(int filas,int columnas,int capas, float ***matriz, char *name){

	for (int k = 0;k<capas;++k){
		printf("\n %s(:,:,%d)\n",name,k);
		for (int i = 0;i<filas;++i){
			for (int j = 0;j<columnas;++j){
				printf("%f \t",matriz[i][j][k]);
			}
			printf("\n");
		}
	}
}


void onesR3(int filas, int columnas, int capas,int ***matriz){
	for (int k = 0;k<capas;++k){
		for (int i = 0;i<filas;++i){
			for (int j = 0;j<columnas;++j){
				matriz[i][j][k] = 1;
			}
		}
	}
}

void onesR3f(int filas, int columnas, int capas,float ***matriz){
	for (int k = 0;k<capas;++k){
		for (int i = 0;i<filas;++i){
			for (int j = 0;j<columnas;++j){
				matriz[i][j][k] = 1;
			}
		}
	}
}


void zerosR3(int filas, int columnas, int capas,int ***matriz){
	for (int k = 0;k<capas;++k){
		for (int i = 0;i<filas;++i){
			for (int j = 0;j<columnas;++j){
				matriz[i][j][k] = 0;
			}
		}
	}
}

void zerosR3f(int filas, int columnas, int capas,float ***matriz){
	for (int k = 0;k<capas;++k){
		for (int i = 0;i<filas;++i){
			for (int j = 0;j<columnas;++j){
				matriz[i][j][k] = 0;
			}
		}
	}
}

void findminMatrizR3(int filas,int columnas ,int capas, float ***matriz,int *out){
	int x= -1, y=-1,z = -1;
	float min_value = INFINITY;
	for (int i = 0;i<filas;++i)
	{
		for (int j = 0;j<columnas;++j)
		{
			for (int k = 0;k<capas;++k)
			{
				if (min_value > matriz[i][j][k]){
					min_value = matriz[i][j][k];
					x = i; y = j; z = k;
				}
			}
		}
	}

	out[0] = x;
	out[1] = y;
	out[2] = z;
}

void prod_escalarMatrizR3(int filas,int columnas ,int capas, float ***matriz,float escalar){
	for (int k = 0;k<capas;++k){
		for (int i = 0;i<filas;++i){
			for (int j = 0;j<columnas;++j){
				matriz[i][j][k] = escalar*matriz[i][j][k];
			}
		}
	}
}
