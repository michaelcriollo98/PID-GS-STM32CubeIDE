
#include <math.h>
#include <stdio.h>

#include "cost_function.h"
#include "math_vector.h"
#include "performance_indices.h"



float calcule_cost(int muestras, float y[muestras], float ref, float dt,	int select){

	float J,t[muestras] , e[muestras],OS,tr,tss,ISE ,ITSE , IAE, ITAE, y_aux[muestras];
	float yinit = y[0],yfinal = ref;

	linspace(t, 0,(muestras-1)*dt+0,muestras);

	prod_vector_escalar(muestras, y, -1, y_aux);
	sum_vector_escalar(muestras, y_aux, ref, e);

		switch (select) {
			case 1:
				IAE = IAE_calc(muestras, e, dt);
				OS = Overshoot(muestras, y, yinit, yfinal);
				tss = SettlingTime(muestras, t, y, yinit, yfinal, 0.05);
				tr = RiseTime(muestras, t, y, yinit, yfinal);

				J = 0.10*ITAE + 0.60*OS + 0.20*tss + 0.10*tr;
				break;
			case 2:
				IAE = IAE_calc(muestras, e, t, dt);
				tss = SettlingTime(muestras, t, y, yinit, yfinal, 0.05);
				OS = Overshoot(muestras, y, yinit, yfinal);

				J = 0.05*IAE + 0.05*tss + 0.90*OS;
				break;
			case 3:
				ISE = ISE_calc(muestras, e, dt);
				tss = SettlingTime(muestras, t, y, yinit, yfinal, 0.05);
				tr = RiseTime(muestras, t, y, yinit, yfinal);

				J = 0.10*ISE + 0.20*tss + 0.70*tr;
				break;
			case 4:
				ISE = ISE_calc(muestras, e, dt);
				tss = SettlingTime(muestras, t, y, yinit, yfinal, 0.05);
				tr = RiseTime(muestras, t, y, yinit, yfinal);

				J = 0.10*ISE + 0.70*tss + 0.20*tr;
				break;
			case 5:
				ISE = ISE_calc(muestras, e, dt);
				tss = SettlingTime(muestras, t, y, yinit, yfinal, 0.05);
				OS = Overshoot(muestras, y, yinit, yfinal);

				J = 0.10*ISE + 0.10*tss + 0.80*OS;
				break;
			default:
				ISE = ISE_calc(muestras, e, dt);
				ITSE = ITSE_calc(muestras, e, t, dt);
				IAE = IAE_calc(muestras, e, dt);
				ITAE = ITAE_calc(muestras, e,t, dt);
				OS = Overshoot(muestras, y, yinit, yfinal);
				tss = SettlingTime(muestras, t, y, yinit, yfinal, 0.05);
				tr = RiseTime(muestras, t, y, yinit, yfinal);
				printf("\ntss = %f\nOS = %f\ntr = %f\nISE = %f\nIAE = %f\nITAE = %f\nITSE = %f\n",tss,OS,tr,ISE,IAE,ITAE,ITSE);
				printf("Select 1 : J = 0.10*IAE + 0.60*OS + 0.20*tss + 0.10*tr = %f\n",J = 0.10*IAE + 0.60*OS + 0.20*tss + 0.10*tr );
				printf("Select 2 : J = 0.05*ITAE + 0.05*tss + 0.90*OS = %f\n",J = 0.05*ITAE + 0.05*tss + 0.90*OS);
				printf("Select 3 : J = 0.10*ISE + 0.20*tss + 0.70*tr = %f\n",J = 0.10*ISE + 0.20*tss + 0.70*tr );
				printf("Select 4 : J = 0.10*ISE + 0.70*tss + 0.20*tr = %f\n",J = 0.10*ISE + 0.70*tss + 0.20*tr );
				printf("Select 5 : J = 0.10*ISE + 0.10*tss + 0.80*OS = %f\n",J = 0.10*ISE + 0.10*tss + 0.80*OS );
				J = 0.05*IAE + 0.05*tss + 0.90*OS;
				break;
		}
		if (J != J){
			J = INFINITY;
		}
		//printf("\nJ es %f\n",J);
	return J;
}


