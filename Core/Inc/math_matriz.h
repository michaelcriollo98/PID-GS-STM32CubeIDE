
#ifndef MATH_MATRIZ_H_
#define MATH_MATRIZ_H_

void multiplicar_matriz(int fil_A,int col_A, double A[][col_A],int fil_B, int col_B, double B[][col_B], double **C);
void sumar_matriz(int fil_A,int col_A, double A[][col_A],int fil_B, int col_B, double B[][col_B], double **C);
void mostrarMatrizp(int filas, int columnas, double **matriz,char *name);
void mostrarMatriz(int filas, int columnas,double mA[][columnas],char *name);
void mostrarMatrizR3_int(int filas,int columnas,int capas, int ***matriz, char *name);
void mostrarMatrizR3(int filas,int columnas,int capas, float ***matriz, char *name);
void onesR3(int filas, int columnas, int capas,int ***matriz);
void onesR3f(int filas, int columnas, int capas,float ***matriz);
void zerosR3(int filas, int columnas, int capas,int ***matriz);
void zerosR3f(int filas, int columnas, int capas,float ***matriz);
void findminMatrizR3(int filas,int columnas ,int capas, float ***matriz, int *out);
void prod_escalarMatrizR3(int filas,int columnas ,int capas, float ***matriz,float escalar);

void Math_Matriz_Init();

#endif /* MATH_MATRIZ_H_ */
