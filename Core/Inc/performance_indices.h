#ifndef PERFORMANCE_INDICES_H_
#define PERFORMANCE_INDICES_H_


float ISE_calc(int muestras,float e[muestras], float dt);
float ITSE_calc(int muestras,float e[muestras],float t[muestras], float dt);
float IAE_calc(int muestras,float e[muestras], float dt);
float ITAE_calc(int muestras,float e[muestras],float t[muestras], float dt);

float SettlingTime(int muestras,float *t, float *y,float yinit, float yfinal,float porcentaje);
float Overshoot(int muestras, float *y,float yinit, float yfinal);
float RiseTime(int muestras,float *t, float *y,float yinit,float yfinal);


#endif /* PERFORMANCE_INDICES_H_ */
