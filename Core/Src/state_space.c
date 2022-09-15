#include <stdlib.h>
#include <stdio.h>
#include "state_space.h"
#include "math_matriz.h"

matriz_s temp;

void SS_Init(State_Space_s *ss){

	(*ss).G[0][0] = 2.72252913541173;	(*ss).G[0][1] = -2.53365442650837;	(*ss).G[0][2] = 0.807348385022683;
	(*ss).G[1][0] = 1;				(*ss).G[1][1] = 0;					(*ss).G[1][2] = 0;
	(*ss).G[2][0] = 0;				(*ss).G[2][1] = 1;					(*ss).G[2][2] = 0;

	(*ss).H[0][0] = 1;
	(*ss).H[1][0] = 0;
	(*ss).H[2][0] = 0;

	(*ss).C[0][0] = 0.000665462644362899; (*ss).C[0][1] = 0.00251355277458455; (*ss).C[0][2] = 0.000597890655014763;

	(*ss).D[0][0] = 0;

	(*ss).x[0] = 0;
	(*ss).x[1] = 0;
	(*ss).x[2] = 0;

	(*ss).y[0] = 0;
	(*ss).u[0] = 0;

}

void xActual_Gx_Hu(double G[n][n],double *x, double H[n][m], double u[m])
{
	double  xm[n][1], um[m][1];
	for(int i=0; i<n; ++i)
		xm[i][0] = x[i];
	for(int i=0; i<m; ++i)
		um[i][0] = (double) u[i];

	MemoriaDinamica_matriz_out(n, 1, 2);
	multiplicar_matriz(n, n, G, n, 1, xm ,temp.Mout[0]);
	multiplicar_matriz(n, m, H, m, 1, um, temp.Mout[1]);

	for(int i=0; i<n; ++i)
	  for(int j=0; j<1; ++j)
		  x[i] = *(*(temp.Mout[0]+i)+j) + *(*(temp.Mout[1]+i)+j);
	liberarMemoria_matriz_out(n, 2);
}


void  y_Cx_Du(double C[p][n],double  x[n], double D[n][m], double u[m],double *y) {
	double  xm[n][1];
	for(int i=0; i<n; ++i)
		xm[i][0] = x[i];

	MemoriaDinamica_matriz_out(p, 1, 1);
	multiplicar_matriz(p, n, C, n, 1, xm, temp.Mout[0]);

	for(int i=0; i<p; ++i)
	  for(int j=0; j<1; ++j)
		  y[i] = *(*(temp.Mout[0]+i)+j);
	liberarMemoria_matriz_out(p, 1);

}

void MemoriaDinamica_matriz_out(int filas, int columnas,int num_matrices){
	for (int k = 0; k < num_matrices;++k){
		temp.Mout[k] = (double **) calloc(filas, sizeof(double *));
		for (int i=0; i<filas; ++i)
			temp.Mout[k][i] = (double *) calloc(columnas, sizeof(double));
	}
}

void liberarMemoria_matriz_out(int filas,int num_matrices){
	for (int k = 0; k < num_matrices;++k){
		for (int i = 0; i < filas; ++i){
			free(temp.Mout[k][i]);}
		free(temp.Mout[k]);
	}
}
