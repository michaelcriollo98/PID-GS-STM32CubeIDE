#ifndef FGS_CONTROL_H_
#define FGS_CONTROL_H_

#include "pid.h"
typedef struct
{
	double Kpmin;
	double Kpmax;
	double Kdmin;
	double Kdmax;
	double Kimin;
	double Kimax;
} fgs_s;

void FGS_Init(fgs_s *fgs, int select);
void FGS_control(fgs_s *FGS,pid_s *pid);

#endif /* FGS_CONTROL_H_ */
