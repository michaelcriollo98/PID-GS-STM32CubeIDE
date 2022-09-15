
#include "pid.h"
#include "fgs_control.h"
#include "fuzzy.h"
#include "stdio.h"

void FGS_Init(fgs_s *fgs, int select){
	pid_s pid;
	PID_Init(&pid, select);
	(*fgs).Kpmin =  0.533 * pid.Kp;
	(*fgs).Kpmax =     1  * pid.Kp;
	(*fgs).Kdmin  = 1.067 * pid.Kd;
	(*fgs).Kdmax =      2 * pid.Kd;
	if (select == 2)
	{
		(*fgs).Kimin=     1.2 * pid.Ki;
		(*fgs).Kimax =    2.5 * pid.Ki;
	}
	else
	{
		(*fgs).Kimin=     1.2 * pid.Ki;
		(*fgs).Kimax =    2.5 * pid.Ki;
	}

	Fuzzy_Init();
}
void FGS_control(fgs_s *FGS,pid_s *pid){

	float delt_e = pid->e[0] - pid->e[1];
	float in[2], out[3];
	in[0] = pid->e[0] ;	in[1] = delt_e;

	if (pid->r != 0)
	{
		in[0] = in[0]/pid->r;
		in[1] = in[1]/pid->r;
	}

	eval_fuzzy(in,out);
	float kpp = out[0], kdp = out[1], kip = out[2];

	out[0] = (FGS->Kpmax - FGS->Kpmin) * kpp + FGS->Kpmin;
	out[2] = (FGS->Kdmax - FGS->Kdmin) * kdp + FGS->Kdmin;
	out[1] = (FGS->Kimax - FGS->Kimin) * kip + FGS->Kimin;
	for (int i =0;i<3;++i)
		pid->Gains[i]=out[i];
}
