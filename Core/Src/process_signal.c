

#include "process_signal.h"
#include <math.h>

float saturation(float value,float interval[2]){

	value = value > interval[1] ? interval[1]:value;
	value = value < interval[0] ? interval[0]:value;

	return value;
}

float normalizacion (float value, float interval[2],float scaling[2])
{
	value = scaling[0] + (value - interval[0])*(scaling[1]-scaling[0])/(interval[1]- interval[0]);
	return value;
}

float desnormalizacion (float value, float interval[2],float scaling[2])
{
	value  = (value-scaling[0]) * (interval[1]- interval[0])/(scaling[1]-scaling[0]) + interval[0];
	return value;
}

float fround(float X, int N)
{
	float num, aux;
	num =(float) (((int) (X*pow(10, N)))/pow(10, N));

	aux = X - num;
	if ((fabs(aux) + 5/pow(10, N+1)) + 0.000001 >= 1/pow(10, N))
	{
		num = num + fabs(aux)/aux*1/pow(10, N);
	}
	return num;
}

float limitar_altura(float h , int *M)
{
    if (h > 25 || h < 0)
    {
    	h = 0;
    	(*M)--;
    }
    return h;
}


