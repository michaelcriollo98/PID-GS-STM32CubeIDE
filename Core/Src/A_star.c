
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "A_star.h"
#include "pid.h"
#include "math_matriz.h"
#include "math_vector.h"
#include "random_numbers.h"
#include "cost_function.h"

void Astar_Init(Astar_s *Astar,int select){

	Astar->muestra_i = 0;			Astar->muestras = 70;
	Astar->iter[0] =   1;			Astar->iter[1] =  6;
	Astar-> enable = 0;
	Astar-> fin = 0;
	Astar->pausar = 0;

	Astar-> lenC = 0;
	Astar-> lenA = 0;

	Astar-> cont = 0;

	Astar->dim_x = 3;
	Astar->dim_y = 3;
	Astar->dim_z = 3;

	pid_s pid; PID_Init(&pid, select);
	Astar->Kpmin =  0.533 * pid.Kp;
	Astar->Kpmax =     1  * pid.Kp;
	Astar->Kdmin  = 1.067 * pid.Kd;
	Astar->Kdmax =      2 * pid.Kd;
	//Astar->Kimin=    0.01 * pid.Ki;
	//Astar->Kimax =   0.85 * pid.Ki;
	Astar->Kimin=    1.2 * pid.Ki;
	Astar->Kimax =   2.5 * pid.Ki;

	reserve_memory_Astar(Astar);
	generate_ID(Astar->dim_x, Astar->dim_y, Astar->dim_z,  Astar->ID,Astar->cifras);
	zerosR3(Astar->dim_x, Astar->dim_y, Astar->dim_z,  Astar->space);
	zerosR3(Astar->dim_x, Astar->dim_y, Astar->dim_z,  Astar->padre);
	zerosR3f(Astar->dim_x, Astar->dim_y, Astar->dim_z,  Astar->G);
	onesR3f(Astar->dim_x, Astar->dim_y, Astar->dim_z,  Astar->F);
	prod_escalarMatrizR3(Astar->dim_x, Astar->dim_y, Astar->dim_z,  Astar->F, NAN);
	prod_escalarMatrizR3(Astar->dim_x, Astar->dim_y, Astar->dim_z,  Astar->H, NAN);
	linspace(Astar->Kp_values, Astar->Kpmin, Astar->Kpmax,Astar->dim_x);
	linspace(Astar->Kd_values, Astar->Kdmin, Astar->Kdmax,Astar->dim_y);
	linspace(Astar->Ki_values, Astar->Kimin, Astar->Kimax,Astar->dim_z);

	Astar->X1 = find(pid.Gains[0],Astar->dim_x,Astar->Kp_values,-1);
	Astar->Y1 = find(pid.Gains[2],Astar->dim_y,Astar->Kd_values,-1);
	Astar->Z1 = find(pid.Gains[1],Astar->dim_z,Astar->Ki_values,-1);
	// error -1: encuentra en el vector el indice mas cercano al valor dado

	Astar->X2 = randi(0, Astar->dim_x -1);
	Astar->Y2 = randi(0, Astar->dim_y -1);
	Astar->Z2 = randi(0, Astar->dim_z -1);

	Astar->X2 =-1;
	Astar->Y2 =-1;
	Astar->Z2 = -1;

	/*
	printf("index Kp = %d\n",Astar->X1);
	printf("index Kd = %d\n",Astar->Y1);
	printf("index Ki = %d\n",Astar->Z1);

	printf("index2 Kp = %d\n",Astar->X2);
	printf("index2 Kd = %d\n",Astar->Y2);
	printf("index2 Ki = %d\n",Astar->Z2);*/

}

void Aestrella(Astar_s *Astar,pid_s *pid,float dt)
{
	if (Astar->fin == 0){
		if (Astar->enable == 1)
		{
			int x,y,z;
			int *coord[] = {&x,&y,&z};
			int dim[] ={Astar->dim_x,Astar->dim_y,Astar->dim_z};
			int posActual[3];
			int subiter[3];
			int salir = 0;

			int lenAF,pos,cont_next, find_indexC, find_indexA,ID_XYZ,ZYX, YX ;
			float G;

			do{
				posActual[0] = Astar->X1;posActual[1] = Astar->Y1;posActual[2] = Astar->Z1;
				if (!((Astar->X1 == Astar->X2 && Astar->Y1 == Astar->Y2 && Astar->Z1 == Astar->Z2)|| Astar->iter[0] > Astar->iter[1]))
				{
					if (Astar->cont == 0){
						for (int i = 0;i<3;++i){(*coord[i]) = (posActual[i]);}
						cont_next  = 1;
						Astar->pausar = 1;
					}
					else{
						/*--------------------------------------------------------------------------------
						Delimitamos las coordenas que se encuntren fuera del rango  0<(x,y,z)<dim(x,y,z)*/
						Astar->cont  = delimitar_coordenadas(dim, posActual, Astar->cont  );
						cont_next    = delimitar_coordenadas(dim, posActual, Astar->cont+1);
						/*-------------------------------------------------------------------------------
						//Determina la coordenada (x,y,z) actual 							             */
						contador(Astar->cont,  subiter);
						for (int i = 0;i<3;++i){(*coord[i]) = (posActual[i]) + subiter[i]-2;}
						//---------------------------------------------------------------------------------
						}
					find_indexC = find_int(Astar->ID[x][y][z], Astar->lenC, Astar->ListaC, 0);
					if (Astar->space[x][y][z] == 0 && find_indexC ==-1)
					{
						find_indexA = find_int(Astar->ID[x][y][z], Astar->lenA, Astar->ListaA, 0);
						if (find_indexA == -1)
						{
							if (Astar->pausar == 1)
							{
								Astar->H[x][y][z] = calcule_cost((Astar->muestras)-1, Astar->y, Astar->r, dt,2);
								Astar->G[x][y][z] = Astar->G[Astar->X1][Astar->Y1][Astar->Z1] + sqrtf(powf(x-Astar->X1,2)+powf(y-Astar->Y1,2)+powf(z-Astar->Z1,2));
								Astar->F[x][y][z] = Astar->G[x][y][z] + Astar->H[x][y][z];
								Astar->padre[x][y][z] = Astar->ID[Astar->X1][Astar->Y1][Astar->Z1];

								Astar->lenA++;
								Astar->ListaA[Astar->lenA-1] = Astar->ID[x][y][z];
								Astar->ListaAF[Astar->lenA-1] = Astar->F[x][y][z];

								Astar->pausar = 0;
								Astar-> iter[0] ++;
							}
							else
							{
								Astar->pausar = 1;
								salir = 1 ;
							}
						}
						else
						{
							G = Astar->G[Astar->X1][Astar->Y1][Astar->Z1] + sqrtf(powf(x-Astar->X1,2)+powf(y-Astar->Y1,2)+powf(z-Astar->Z1,2));
							if (G < Astar->G[x][y][z] )
							{
								Astar->G[x][y][z] = G;
								Astar->padre[x][y][z] = Astar->ID[Astar->X1][Astar->Y1][Astar->Z1];
								Astar->F[x][y][z] =Astar->G[x][y][z] + Astar->H[x][y][z];
								Astar->ListaAF[find_indexA] = Astar->F[x][y][z];
							}

						}
					}

					if ((((Astar->cont-1)%(27))>((cont_next-1)%(27)) && Astar->pausar == 0)||Astar->cont == 0 )
					{
						ID_XYZ = find(min_v(Astar->lenA, Astar->ListaAF), Astar->lenA, Astar->ListaAF, 0);
						if  (ID_XYZ !=-1)
						{
							ZYX = Astar->ListaA[ID_XYZ];
							Astar->Z1 = (int) (ZYX / Astar->cifras[2]);
							YX = ZYX -  Astar->Z1 * Astar->cifras[2];
							Astar->Y1 = (int) YX / Astar->cifras[1];
							Astar->X1 =  YX - Astar->Y1 * Astar->cifras[1];

							Astar->lenC++;
							Astar->ListaC[Astar->lenC-1] = Astar->ID[Astar->X1][Astar->Y1][Astar->Z1];
							lenAF = Astar->lenA;
							pos = setdiff_int(&Astar->lenA, Astar->ListaA, Astar->ID[Astar->X1][Astar->Y1][Astar->Z1]);
							remove_value_position(&lenAF, Astar->ListaAF, pos);
							if (Astar->lenC ==(Astar->dim_x*Astar->dim_y*Astar->dim_z)){Astar->fin = 1;salir= 1;}
						}
					}

					if (Astar->pausar == 0){Astar-> cont ++;}
				}
				else{Astar->fin = 1;salir= 1;}

			}while (salir == 0);

			/*-------------------------------------------------------------------------------*/
			/*------------------------------mostrar Listas---------------------------------
			printf("\nlen de A es :%d\n",Astar->lenA);
			mostrar_vector_int(Astar->ListaA, Astar->lenA, "ListaA =");
			mostrar_vector(Astar->ListaAF, Astar->lenA, "ListaAF =");
			printf("len de C es :%d",Astar->lenC);
			mostrar_vector_int(Astar->ListaC, Astar->lenC, "ListaC =");
			printf("*******************************\n");*/
			//--------------------------------------------------------------------------------
			//Selecciona los nuevos valores de ganacia para la siguiente iteracion------------
			// ***************** Escogemos la funcion menor Costo **************************
			if (Astar->fin == 1)
			{
				//mostrarMatrizR3(Astar->dim_x,Astar->dim_y,Astar->dim_z,Astar->F,"F");
				//mostrarMatrizR3(Astar->dim_x,Astar->dim_y,Astar->dim_z,Astar->H,"H");
				int out[3];
				findminMatrizR3(Astar->dim_x, Astar->dim_y, Astar->dim_z, Astar->H, out);
				if (out[0]!=-1)
				{
					pid->Gains[0] = Astar->Kp_values[out[0]];
					pid->Gains[1] = Astar->Ki_values[out[2]];
					pid->Gains[2] = Astar->Kd_values[out[1]];
				}
				//mostrar_vector(pid->Gains, 3, "BestGains Astar =");
			}
			else
			{
				pid->Gains[0] = Astar->Kp_values[x];
				pid->Gains[1] = Astar->Ki_values[z];
				pid->Gains[2] = Astar->Kd_values[y];
			}

			Astar->muestra_i = 0; Astar->enable = 0;
		}
		else
		{
			if (Astar->muestra_i < Astar->muestras-1)
				Astar->muestra_i ++;
		}
	}
}

void reserve_memory_Astar(Astar_s *Astar){

	Astar->F =(float***) malloc(Astar->dim_x*sizeof(float**));
	for (int i = 0 ; i<Astar->dim_x;++i){
		Astar->F[i] = (float**) malloc(Astar->dim_y*sizeof(float*));
		for(int j = 0 ; j<Astar->dim_y;++j){
			Astar->F[i][j] = (float*) malloc(Astar->dim_z*sizeof(float));
		}
	}

	Astar->H =(float***) malloc(Astar->dim_x*sizeof(float**));
	for (int i = 0 ; i<Astar->dim_x;++i){
		Astar->H[i] = (float**) malloc(Astar->dim_y*sizeof(float*));
		for(int j = 0 ; j<Astar->dim_y;++j){
			Astar->H[i][j] = (float*) malloc(Astar->dim_z*sizeof(float));
		}
	}

	Astar->G =(float***) malloc(Astar->dim_x*sizeof(float**));
	for (int i = 0 ; i<Astar->dim_x;++i){
		Astar->G[i] = (float**) malloc(Astar->dim_y*sizeof(float*));
		for(int j = 0 ; j<Astar->dim_y;++j){
			Astar->G[i][j] = (float*) malloc(Astar->dim_z*sizeof(float));
		}
	}

	Astar->space=(int***) malloc(Astar->dim_x*sizeof(int**));
		for (int i = 0 ; i<Astar->dim_x;++i){
			Astar->space[i] = (int**) malloc(Astar->dim_y*sizeof(int*));
			for(int j = 0 ; j<Astar->dim_y;++j){
				Astar->space[i][j] = (int*) malloc(Astar->dim_z*sizeof(int));
			}
		}

	Astar->padre=(int***) malloc(Astar->dim_x*sizeof(int**));
		for (int i = 0 ; i<Astar->dim_x;++i){
			Astar->padre[i] = (int**) malloc(Astar->dim_y*sizeof(int*));
			for(int j = 0 ; j<Astar->dim_y;++j){
				Astar->padre[i][j] = (int*) malloc(Astar->dim_z*sizeof(int));
			}
		}

	Astar->ID=(int***) malloc(Astar->dim_x*sizeof(int**));
		for (int i = 0 ; i<Astar->dim_x;++i){
			Astar->ID[i] = (int**) malloc(Astar->dim_y*sizeof(int*));
			for(int j = 0 ; j<Astar->dim_y;++j){
				Astar->ID[i][j] = (int*) malloc(Astar->dim_z*sizeof(int));
			}
		}

	Astar->Kp_values = (float *) malloc(Astar->dim_x*sizeof(float));
	Astar->Kd_values = (float *) malloc(Astar->dim_y*sizeof(float));
	Astar->Ki_values = (float *) malloc(Astar->dim_z*sizeof(float));

	int max_valores =	(Astar->iter[1]<(Astar->dim_x*Astar->dim_y*Astar->dim_z)?Astar->iter[1]:(Astar->dim_x*Astar->dim_y*Astar->dim_z));
	Astar->ListaA = (int *) malloc(max_valores*sizeof(int));
	Astar->ListaC = (int *) malloc(max_valores*sizeof(int));
	Astar->ListaAF = (float *) malloc(max_valores*sizeof(float));

	Astar->y= (float *)malloc((Astar->muestras+1)*sizeof(float));
}

void generate_ID(int filas,int columnas,int capas, int ***ID,int *cifras){

	int x_cifras = 1,y_cifras, z_cifras;
	char text1[10],text2[10];

	sprintf(text1,"%d",filas-1);
	y_cifras = (pow( 10,strlen( text1)) + 0.000001);

	sprintf(text2,"%d",columnas-1);
	z_cifras = (pow(10,strlen(text1)+strlen(text2))+  0.000001);

	cifras[0] = x_cifras;
	cifras[1] = y_cifras;
	cifras[2] = z_cifras;
	for (int k = 0;k<capas;++k){
		for (int i = 0;i<filas;++i){
			for (int j = 0;j<columnas;++j){
				ID[i][j][k] = i*x_cifras+j*y_cifras+k*z_cifras;
			}
		}
	}
}

// -----------contador de 3 cifras----------------------------
void contador(int iter,int *out){
	int base = 3;int cifras = 3;
	int cont[cifras];
		for(int i = 0;i<cifras;++i){
			cont[i] = ((iter-1)%((int)powf(base,i+1)))+1;
			out[i] =  ((cont[i]-1)/powf(base,i)) + 1;}

}
//------------------------------------------------------------

int delimitar_coordenadas(int dim[3], int posActual[3],int iter){

	int subiter[3];
	int coord[3];
	do {
		contador(iter, subiter);
		for (int i = 0;i<3;++i){
			coord[i] = (posActual[i]) +  subiter[i]-2;
			if (coord[i] <0 || coord[i] >= dim[i]){iter += (int) powf(3,i);}}

	} while (coord[0]<0 || coord[0]>=dim[0] || coord[1]<0 || coord[1]>=dim[1] || coord[2]<0 || coord[2]>=dim[2]);

	return iter;
}
