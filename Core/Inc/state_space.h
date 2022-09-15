
#ifndef INC_STATE_SPACE_H_
#define INC_STATE_SPACE_H_

// Representacion discreta de espacio de estados
#define n   3    // dimension de la matriz de estado
#define m	1	 // numero de entradas
#define p	1 	 // numero de salidas

typedef struct{
	double x[n];		// Vector de estado
	double u[m];		// Vector de entrada
	double y[p];		// Vector de salida
	double G[n][n];		// Matriz de estado
	double H[n][m];		// Matriz de entrada
	double C[p][n];		// Matriz de Salida
	double D[p][m];		// Matriz de transferencia directa

} State_Space_s;

typedef struct {
	double **Mout[5];  //  numero maximo de matrices dinamicas 5.
} matriz_s;


void xActual_Gx_Hu(double G[n][n],double *x, double H[n][m], double u[m]);
void y_Cx_Du(double C[p][n],double  x[n], double D[n][m], double u[m],double *y);
void MemoriaDinamica_matriz_out(int filas, int columnas,int num_matrices);
void liberarMemoria_matriz_out(int filas,int num_matrices);
void SS_Init(State_Space_s *ss);


#endif /* INC_STATE_SPACE_H_ */
