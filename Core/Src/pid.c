#include "pid.h"
#include "stdio.h"



void PID_Init(pid_s *pid, int select)
{

   	(*pid).r = 1.0;		// referencia
   	(*pid).e[0]= 0.0; 	// error
   	(*pid).e[1]= 0.0;		// error anterior e(k-1)
   	(*pid).e[2]= 0.0;		// error acumulado
   	(*pid).y = 0;			// Salida de la Planta
   	(*pid).u = 0;			// Señal de control

	switch (select)
	{
	case 1:
		/*
	   	(*pid).Kp = 0.2;
		(*pid).Kd = 0.01;
		(*pid).Ki = 0.0005 ;


//  -SA
	   	(*pid).Kp = 0.2;
		(*pid).Kd = 0.0124109779;
		(*pid).Ki = 0.000143810001 ;

// AS----
	   	(*pid).Kp = 0.2;
		(*pid).Kd = 0.010669;
		(*pid).Ki = 0.000215 ;

// SA ----- FALLAS

		(*pid).Kp = 0.2;
		(*pid).Kd = 0.019397;
		(*pid).Ki = 0.0004243 ;

	//	AS ----- FALLAS

		(*pid).Kp = 0.2;
		(*pid).Kd = 0.015334;
		(*pid).Ki = 0.000215 ;

// SA ----- FALLAS T2

		(*pid).Kp = 0.2;
		(*pid).Kd = 0.016;
		(*pid).Ki = 0.001203;*/

	// SA ----- FALLAS T2

			(*pid).Kp = 0.2;
			(*pid).Kd = 0.010696;
			(*pid).Ki = 0.000925;

		break;
	case 2:
		/*

	   	(*pid).Kp = 0.2;
		(*pid).Kd = 0.1;
		(*pid).Ki = 0.0001;

	   	(*pid).Kp = 0.113614343;
		(*pid).Kd = 0.1;
		(*pid).Ki = 0.0000167836 ;


	   	(*pid).Kp = 0.2;
		(*pid).Kd = 0.1533;
		(*pid).Ki = 0.000043 ;


// ****SA FALL T2
	   	(*pid).Kp = 0.18;
		(*pid).Kd = 0.14166;
		(*pid).Ki = 0.0001476 ;
		*/
		// ****AS FALL T2

	   	(*pid).Kp = 0.2;
		(*pid).Kd = 0.1067;
		(*pid).Ki = 0.00018499;

		break;
	case 3:

		/*
	   	(*pid).Kp = 0.7;
		(*pid).Kd = 0.1;
		(*pid).Ki = 0.001;


	   	(*pid).Kp = 0.7;
		(*pid).Kd = 0.1697 ;
		(*pid).Ki = 0.000592;

	   	(*pid).Kp = 0.7;
		(*pid).Kd = 0.1067;
		(*pid).Ki = 0.00043;

		// ****SA FALL T2
		(*pid).Kp = 0.6;
		(*pid).Kd = 0.1632;
		(*pid).Ki = 0.001;*/

		// ****AS FALL T2
	   	(*pid).Kp = 0.7;
		(*pid).Kd = 0.1067;
		(*pid).Ki = 0.00185;

		break;
	default:
	   	(*pid).Kp = 1;
		(*pid).Kd = 0;
		(*pid).Ki = 0;
	}
	(*pid).Gains[0] = (*pid).Kp; (*pid).Gains[1] =(*pid).Ki; (*pid).Gains[2] =(*pid).Kd;
}



float PID_control( float *e, float Gains[3],float Ts)
{
	float u;
	float Kp = Gains[0],  Ki = Gains[1],  Kd = Gains[2];

	float error = e[0];
	float error_ant = e[1];
	float error_sum = e[2];
	float delt_error = error - error_ant;
	float error_acum = error_sum+ error;

	u = Kp * error  + Ki * (Ts * error_acum) + Kd * (delt_error / Ts);

	*(e+1) = error;
	*(e+2) = error_acum;

    return u;
}
