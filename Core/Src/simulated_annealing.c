
#include <math.h>
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

#include "simulated_annealing.h"
#include "random_numbers.h"
#include "cost_function.h"

void SA_Init(sa_s *SA, int select){
	SA-> iniciar = 0;
	SA-> enable = 0;	    // habilita el algoritmo SA

	SA->iter[0] = 0;		// Inicio
	SA->iter[1] = 3;		// Fin

	SA->subiter[0] =0;		// Inicio
	SA->subiter[1] = 2;		// Fin

	SA->muestra_i = 0;	    // Inicio
	SA->muestras = 70;		// Fin
	SA->selectCostF = 2;      // Selecion de la funcion objetivo
	SA->Temp = 0.025;  	    // Temperatura Inicial
 	SA->alpha = 0.99;
	SA->y = (float *)malloc((SA->muestras+1)*sizeof(float));
	SA->BestCost = (float *)malloc((SA->iter[1])*sizeof(float));

	pid_s pid;
	PID_Init(&pid, select);
	SA->rangeGains[0] = 0.533 * pid.Kp;  // Kp minimo
	SA->rangeGains[1] =    1  * pid.Kp;	 // Kp maximo
	SA->rangeGains[2] = 1.067 * pid.Kd;	 // Kd minimo
	SA->rangeGains[3] =     2 * pid.Kd;  // Kd maximo
	SA->rangeGains[4] =  1.2 * pid.Ki;  // Ki minimo
	SA->rangeGains[5] =  2.5 * pid.Ki;  // Kd maximo

	//srand(time(NULL)+18);
}

void Simulated_Annealing(sa_s *SA,pid_s *pid,float dt)
{
	if (SA->iniciar == 1) // se inicializa en el primer cambio de referencia
	{
		if (SA->enable == 1) // se habilita en cada cambio de referencia
		{
			solSA_s newsol;
			// se determina la solucion inicial
			if (SA->iter[0] == 0 && SA->subiter[0] == 0)
			{
				for (int i =0;i<3;++i){
				SA->sol.Gain[i] = (*pid).Gains[i];}
				SA->sol.Cost = calcule_cost((SA->muestras)-1, SA->y, SA->r, dt, SA->selectCostF);
				SA->BestSol = SA->sol;
				// crea nuevos valores de Kp , Kd o Ki
				CreateNeighbor(pid->Gains,SA->rangeGains);
				(SA->iter[0])++;			// primera iteracion
				(SA->subiter[0])++;			// primera subiteracion
			}
			else
			{
				if (SA->iter[0] <= SA->iter[1]) // realiza si aun no se cumple con numero de iteraciones
				{
					if (SA->subiter[0]<=SA->subiter[1]) // realiza si aun no se cumple con numero de sub-iteraciones
					{
						for (int i =0;i<3;++i) {
						newsol.Gain[i] = (*pid).Gains[i];} //nuevos valores de ganancias
						newsol.Cost = calcule_cost((SA->muestras)-1, SA->y, SA->r, dt, SA->selectCostF); //evaluacion de la funcion objetivo
						// la nueva solucion pasa ha ser la solucion actual si tiene un costo menor
						if (newsol.Cost <=SA->sol.Cost){
							SA->sol = newsol;
						}
						// caso contrario la nnueva solucion puede ser aceptada o no con probabilidad P
						else {
							float DELTA = (newsol.Cost-SA->sol.Cost)/SA->sol.Cost;
							float  P = exp(-DELTA/SA->Temp);
							if (randf() <= P){
								SA->sol = newsol;}
						}
						// actualiza la mejor solucion encontrada
						if (SA->sol.Cost <= SA->BestSol.Cost)
						{
							SA->BestSol = SA->sol;
						}
						CreateNeighbor(pid->Gains,SA->rangeGains); // crea soluciones vecinas
					}
					// actualiza el valor de las sub-iteracones
					int cont = 0;
					if (SA->subiter[0] == SA->subiter[1]){(SA->subiter[0])=1;cont = 1;}
					else {(SA->subiter[0])++;}
					if (cont == 1){
						SA->Temp = SA->alpha*SA->Temp; // actualiza el valor de temperatura
						SA->BestCost[(SA->iter[0])-1] = SA->BestSol.Cost; // guarda la mejor solucion
						(SA->iter[0])++; 		      // actualiza el valor de las iteraciones
						cont = 0;
					}
				}
				else // al finalizar el numero de iteraciones
				{
					// se fija las ganancias del controlador PID con la mejor solucion
					for(int i = 0;i<3;i++)
					pid->Gains[i] = SA->BestSol.Gain[i];
				}
			}
			SA->muestra_i = 0; SA->enable = 0;
		}
		else // se toma determinadas muestras
		{
			if (SA->muestra_i < SA->muestras-1)
				SA->muestra_i ++;
		}
	}
}

void CreateNeighbor(float *Gains,float range_gains[6]){

	float Kpmin = range_gains[0], 			Kpmax = range_gains[1];
	float Kdmin = range_gains[2], 		    Kdmax = range_gains[3];
	float Kimin = range_gains[4], 		    Kimax = range_gains[5];

	float pKp = 0.2,  pKi = 0.5, pKd = 1-pKp-pKi;

	float r = randf();
	int variacion = 0;
	if (r <=pKp)
		variacion = 1;
	if (r>pKp && r<=pKp+pKi)
		variacion = 2;
	if (r>pKp+pKi && r<=pKp+pKi+pKd)
		variacion = 3;

	switch (variacion) {
		case 1:
            Gains[0] = Kpmin + (Kpmax-Kpmin)*randf();
			break;
		case 2:
			Gains[1] = Kimin + (Kimax-Kimin)*randf();
			break;
		case 3:
			Gains[2] = Kdmin + (Kdmax-Kdmin)*randf();
			break;
		default:
			break;
	}

}

