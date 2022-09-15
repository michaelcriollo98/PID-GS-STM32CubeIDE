#ifndef SIMULATED_ANNEALING_H_
#define SIMULATED_ANNEALING_H_

#include "pid.h"

typedef struct {
	float Gain[3];
	float Cost;
} solSA_s;

typedef struct {
	int iniciar;
	int enable;
	int iter[2];
	int subiter[2];
	int muestra_i;
	int muestras;
	int selectCostF;
	float rangeGains[6];
	float Temp;
	float alpha;
	float *y;
	float r;
	float *BestCost;
	solSA_s sol;
	solSA_s BestSol;
} sa_s;


void SA_Init(sa_s *SA, int select);
void Simulated_Annealing(sa_s *SA,pid_s *pid,float dt);
void CreateNeighbor(float *solGains,float range_gains[6]);


#endif /* SIMULATED_ANNEALING_H_ */
