#include <math.h>
#include <stdio.h>
#include "performance_indices.h"


float ISE_calc(int muestras,float e[muestras], float dt)
{
	float ISE = 0;
	for (int i=1;i < muestras;++i){
			ISE += ((e[i]*e[i]+e[i-1]*e[i-1])/2)*dt;
	}

	return ISE;
}

float ITSE_calc(int muestras,float e[muestras],float t[muestras], float dt)
{
	float ITSE = 0;
	for (int i=1;i<muestras;++i){
			ITSE += ((t[i]*e[i]*e[i]+t[i-1]*e[i-1]*e[i-1])/2)*dt;
	}

	return ITSE;
}


float IAE_calc(int muestras,float e[muestras], float dt)
{
	float IAE = 0;

	for (int i=1;i<muestras;++i){
			IAE += ((fabsf(e[i])+fabsf(e[i-1]))/2)*dt;
	}

	return IAE;
}


float ITAE_calc(int muestras,float e[muestras],float t[muestras], float dt)
{
	float ITAE = 0;

	for (int i=1;i<muestras;++i){
			ITAE += ((t[i]*fabsf(e[i])+t[i-1]*fabsf(e[i-1]))/2)*dt;
	}

	return ITAE;
}


float SettlingTime(int muestras,float *t, float *y,float yinit, float yfinal,float porcentaje){

	float tss = NAN, yaux = yfinal;
	float  ajuste=0;
	int i = muestras-1;

	while (fabsf((y[i]-yfinal)/(yfinal-yinit))<=porcentaje && i>=0){
		i--;
	}
	//ajuste lineal para un valor mas aproximado del tiempo;

	if (i != muestras-1){
	if ((y[i]-yfinal)/(yfinal-yinit)>0)
		yaux = yfinal*(1+porcentaje);
	else if ((y[i]-yfinal)/(yfinal-yinit)<0)
		yaux = yfinal*(1-porcentaje);

	ajuste = (t[i+1]-t[i])/(y[i+1]-y[i])*(yaux-y[i]);

	tss = t[i]+ajuste;
	}

	return tss;
}


float Overshoot(int muestras, float *y,float yinit, float yfinal){

	float OS = INFINITY;
	float max_num = 0;
	for (int i=0 ; i<muestras;++i)
	{
		if ((y[i]-yinit)/(yfinal-yinit)-1 > max_num)
			max_num = ((y[i]-yinit)/(yfinal-yinit)-1);
	}
	if (max_num>0)
		OS = max_num*100;
	return OS;
}

float RiseTime(int muestras,float *t, float *y,float yinit,float yfinal){
	float tr = 0;
	int i1 = 0,i2 = 0;
	int i = 0;

	while((y[i]-yinit)/(yfinal-yinit) <= 0.1 && i<muestras){
		i++;
	}
	i1 = i;
	while((y[i]-yinit)/(yfinal-yinit) <= 0.9 && i<muestras){
		i++;
	}
	i2 = i;

	if ((i1 !=0 || i1 != muestras )&& (i2!=0 ||i2!=muestras) )
		tr = t[i2]-t[i1];

	return tr;
}



