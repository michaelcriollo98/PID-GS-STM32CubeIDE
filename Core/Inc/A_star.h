#ifndef A_STAR_H_
#define A_STAR_H_

#include "pid.h"


typedef struct {
	int enable;
	int fin;
	int cont;
	int pausar;
	int iter[2];
	int dim_x, dim_y, dim_z;
	int X1,Y1,Z1;
	int X2,Y2,Z2;
	int ***space;
	int ***ID;
	int ***padre;
	float ***F;
	float ***H;
	float ***G;

	float Kpmin,  Kpmax;
	float Kdmin,  Kdmax;
	float Kimin,  Kimax;

	float *Kp_values,  *Kd_values, *Ki_values;
	int   *ListaA, *ListaC;
	float *ListaAF;
	int lenC,lenA;

	int cifras[3];

	float *y;
	float r;
	int muestra_i;
	int muestras;

} Astar_s;


void Astar_Init(Astar_s *Astar,int select);
void reserve_memory_Astar(Astar_s *Astar);
void generate_ID(int filas,int columnas,int capas, int ***ID, int *cifras);
void contador(int iter,int *out);
int delimitar_coordenadas(int dim[3], int posActual[3],int iter);
void Aestrella(Astar_s *Astar,pid_s *pid,float dt);


#endif /* A_STAR_H_ */
