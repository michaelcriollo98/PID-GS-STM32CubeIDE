
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "fuzzy.h"
#include "math_vector.h"

fuzzy_s fuzzy;
void Fuzzy_Init(){
	// Definicion de rangos de las funciones de error,delta error, Kp', Kd' y  Ki'
	fuzzy.range_e[0]  = -1;		fuzzy.range_e[1] = 1;		// rango del error
	fuzzy.range_de[0] = -1;		fuzzy.range_de[1] = 1;		// rango de la diferencia del error (delta error)
	fuzzy.range_kpp[0] = 0;		fuzzy.range_kpp[1] = 1;		// rango de la ganancia proporcional Kp'
	fuzzy.range_kdp[0] = 0;		fuzzy.range_kdp[1] = 1;		// rango de la ganancia derivativa Kd'
	fuzzy.range_kip[0] = 0;	    fuzzy.range_kip[1] = 1;		// rango de la ganancia integral Ki'

	// definicion de logitud maxima de los vectores para las funciones de membresia
	int len_e =  n_max_e;
	int len_de =  n_max_e;
	int len_kpp =  n_max_kpp;
	int len_kdp = n_max_kpp;
	int len_kip =  n_max_kip;
	fuzzy.len_MFout[0] = len_kpp;
	fuzzy.len_MFout[1] = len_kdp;
	fuzzy.len_MFout[2] = len_kip;

	/* generacion de los vectores de error, delta error, Kp',Kd',Ki'
	   con valores dentro del rango y  espacio entres valores (paso)
	   presetablecido */
	float paso = PASO;
	generate_vector(fuzzy.error,fuzzy.range_e[0],fuzzy.range_e[1], paso);
	generate_vector(fuzzy.delt_error,fuzzy.range_de[0],fuzzy.range_de[1], paso);
	generate_vector(fuzzy.kpp,fuzzy.range_kpp[0],fuzzy.range_kpp[1], paso);
	generate_vector(fuzzy.kdp ,fuzzy.range_kdp[0],fuzzy.range_kdp[1], paso);
	generate_vector(fuzzy.kip,fuzzy.range_kip[0],fuzzy.range_kip[1], paso);
	// definicion de las variables de entrada de entrada y salida
	fuzzy.fun_in_out[0] = fuzzy.error;
	fuzzy.fun_in_out[1] = fuzzy.delt_error;
	fuzzy.fun_in_out[2] = fuzzy.kpp;
	fuzzy.fun_in_out[3] = fuzzy.kdp;
	fuzzy.fun_in_out[4] = fuzzy.kip;
	// funciones de membresia para la variable de entrada del error
	for (int i = 0 ; i < len_e; ++i){
		fuzzy.eNB[i] = trapmf(fuzzy.error[i], -1E100, -1E100, -0.9667,  -0.7);
		fuzzy.eNM[i] = trimf(fuzzy.error[i],  -1, -0.6667, -0.3333);
		fuzzy.eNS[i] = trimf(fuzzy.error[i], -0.6667, -0.3333, -5.551E-17);
		fuzzy.eZO[i] = trimf(fuzzy.error[i], -0.3333, 0, 0.3333);
		fuzzy.ePS[i] = trimf(fuzzy.error[i], -5.551e-17, 0.3333, 0.6667);
		fuzzy.ePM[i] = trimf(fuzzy.error[i], 0.3333, 0.6667, 1);
		fuzzy.ePB[i] = trapmf(fuzzy.error[i],  0.7, 0.9667,   1E10, 1E10);
	}
	fuzzy.eMF[0] = fuzzy.eNB;
	fuzzy.eMF[1] = fuzzy.eNM;
	fuzzy.eMF[2] = fuzzy.eNS;
	fuzzy.eMF[3] = fuzzy.eZO;
	fuzzy.eMF[4] = fuzzy.ePS;
	fuzzy.eMF[5] = fuzzy.ePM;
	fuzzy.eMF[6] = fuzzy.ePB;
	// funciones de membresia para la variable de entrada delta error
	for (int i = 0 ; i < len_de; ++i){
		fuzzy.deNB[i] = trapmf(fuzzy.delt_error[i], -1E100, -1E100, -0.9667,  -0.7);
		fuzzy.deNM[i] = trimf(fuzzy.delt_error[i],  -1, -0.6667, -0.3334);
		fuzzy.deNS[i] = trimf(fuzzy.delt_error[i], -0.6666, -0.3333, 0);
		fuzzy.deZO[i] = trimf(fuzzy.delt_error[i], -0.3334, 0, 0.3334);
		fuzzy.dePS[i] = trimf(fuzzy.delt_error[i], 0, 0.3334, 0.6667);
		fuzzy.dePM[i] = trimf(fuzzy.delt_error[i], 0.3334, 0.6667, 1);
		fuzzy.dePB[i] = trapmf(fuzzy.delt_error[i],  0.7, 0.9667,   1E10, 1E10);
	}
	fuzzy.deMF[0] = fuzzy.deNB;
	fuzzy.deMF[1] = fuzzy.deNM;
	fuzzy.deMF[2] = fuzzy.deNS;
	fuzzy.deMF[3] = fuzzy.deZO;
	fuzzy.deMF[4] = fuzzy.dePS;
	fuzzy.deMF[5] = fuzzy.dePM;
	fuzzy.deMF[6] = fuzzy.dePB;
	// funciones de membresia para  las varaibles de salida Kp'
	for (int i = 0 ; i < len_kpp; ++i){
		fuzzy.small1[i] = gaussmf(fuzzy.kpp[i], 0.4247, 0);
		fuzzy.big1[i] = gaussmf(fuzzy.kpp[i], 0.4247, 1);
	}
	fuzzy.kppMF[0] = fuzzy.small1; fuzzy.kppMF[1] = fuzzy.big1;
	// funciones de membresia para  las varaibles de salida Kd'
	for (int i = 0 ; i < len_kdp; ++i){
		fuzzy.small2[i] = gaussmf(fuzzy.kdp[i], 0.4247, 0);
		fuzzy.big2[i] = gaussmf(fuzzy.kdp[i], 0.4247, 1);
	}
	fuzzy.kdpMF[0] = fuzzy.small2; fuzzy.kdpMF[1] = fuzzy.big2;
	// funciones de membresia para  las varaibles de salida Ki'
	for (int i = 0 ; i < len_kip; ++i){
		fuzzy.S[i]  = gaussmf(fuzzy.kip[i], 0.1,0);
		fuzzy.MS[i] = gaussmf(fuzzy.kip[i], 0.1, 0.295);
		fuzzy.MB[i] = gaussmf(fuzzy.kip[i], 0.1, 0.705);
		fuzzy.B[i]  = gaussmf(fuzzy.kip[i], 0.1, 1);
	}
	fuzzy.kipMF[0] = fuzzy.S; fuzzy.kipMF[1] = fuzzy.MS;
	fuzzy.kipMF[2] = fuzzy.MB; fuzzy.kipMF[3] = fuzzy.B;
	// funciones de membresia de entrada
	fuzzy.MFin[0]=fuzzy.eMF;fuzzy.MFin[1]=fuzzy.deMF;
	// funciones de membresia de salida
	fuzzy.MFout[0]=fuzzy.kppMF;fuzzy.MFout[1]=fuzzy.kdpMF;fuzzy.MFout[2]=fuzzy.kipMF;
	// definicion de las reglas difusas
	char *valores[N_RULES]= {
			"1 1, 2 1 4",
			"1 2, 2 1 4",
			"1 3, 2 1 4",
			"1 4, 2 1 4",
			"1 5, 2 1 4",
			"1 6, 2 1 4",
			"1 7, 2 1 4",
			"2 1, 1 2 3",
			"2 2, 2 2 3",
			"2 3, 2 1 4",
			"2 4, 2 1 4",
			"2 5, 2 1 4",
			"2 6, 2 2 3",
			"2 7, 1 2 3",
			"3 1, 1 2 2",
			"3 2, 1 2 3",
			"3 3, 2 2 3",
			"3 4, 2 1 4",
			"3 5, 2 2 3",
			"3 6, 1 2 3",
			"3 7, 1 2 2",
			"4 1, 1 2 1",
			"4 2, 1 2 2",
			"4 3, 1 2 3",
			"4 4, 2 2 3",
			"4 5, 1 2 3",
			"4 6, 1 2 2",
			"4 7, 1 2 1",
			"5 1, 1 2 2",
			"5 2, 1 2 3",
			"5 3, 2 2 3",
			"5 4, 2 1 4",
			"5 5, 2 2 3",
			"5 6, 1 2 3",
			"5 7, 1 2 2",
			"6 1, 1 2 3",
			"6 2, 2 2 3",
			"6 3, 2 1 4",
			"6 4, 2 1 4",
			"6 5, 2 1 4",
			"6 6, 2 2 3",
			"6 7, 1 1 3",
			"7 1, 2 1 4",
			"7 2, 2 1 4",
			"7 3, 2 1 4",
			"7 4, 2 1 4",
			"7 5, 2 1 4",
			"7 6, 2 1 4",
			"7 7, 2 1 4"};
		for (int i = 0 ; i < N_RULES; ++i) {
			fuzzy.reglas[i] = valores[i];
		}
}

void eval_fuzzy(float *entradas, float *salidas){
	float paso = PASO;
	int n_entradas = num_MFin;
	int n_salidas = num_MFout;
	int n_rules = N_RULES;
	int index_in[n_entradas];
	float aux_var;
	float **out;
	float fuzzy_value;
	int selec_MFin[n_entradas], selec_MFout[n_salidas];

	// reservamos espacio de memoria para out
	out = (float **) calloc(n_salidas, sizeof(float *));
	for(int k = 0; k < n_salidas; ++k)
		out[k] = (float * ) calloc(fuzzy.len_MFout[k],sizeof(float));

	//buscamos el indice correspondiente para cada variable de entrada;
	for (int i = 0; i < n_entradas;++i)
		index_in[i] = find_index(*(entradas + i), fuzzy.error[0], paso);

	for(int i = 0 ; i < n_rules; ++i ){
		//convertimos los reglas de string a valores numericos
		// salidas de acuerdo a las entradas seleccionadas en base la reglas difusas
		// Ejem: IF (e is NB) and (de is NB) THEN (kpp is big ) and (kdp is small) and (alpha is 2)
		Reglas_str2num(selec_MFin,selec_MFout,i);
		//fusificacion
		fuzzy_value = fuzz(n_entradas,selec_MFin,index_in);
		// inferencia difusa de mandani  (and => min)
		for (int k = 0; k< n_salidas;++k){
			for (int j = 0; j < fuzzy.len_MFout[k]; ++j) {
				aux_var = min(*(*(fuzzy.MFout[k]+selec_MFout[k])+ j),fuzzy_value);
				if (i == 0)
					(*(*(out +k)+j)) = aux_var;
				else
					*(*(out +k)+j) = max(*(*(out +k)+j),aux_var);
			}
		}
	}
	//Defuzzificacion
	for (int k = 0 ; k < n_salidas;++k)
	*(salidas+k) = defuzz(fuzzy.fun_in_out[n_entradas + k], out[k], fuzzy.len_MFout[k]);

    //liberamos espacio de memoria de out
	for (int k = 0;k < n_salidas;++k)
		free(out[k]);
	free(out);
}

// funcion maximo
float max(float a, float b){
	float res_max;
	if(a>b || a == b)
		res_max = a;
	if ( b > a)
		res_max = b;
	return res_max;
}

// funcion minimo
float min(float a, float b){
	float res_min;
	if(a>b || a == b)
		res_min = b;
	if ( b > a)
		res_min = a;
	return res_min;
}

float gaussmf(float x, float sigma, float media){
// standard deviation: sigma, and mean: m,
	float y , value;
	value = -pow(x-media,2)/(2*pow(sigma,2));
	y = exp(value);
	return y ;
}

float trimf(float x, float a, float b,float c){
	float y ;
	 y = max(min((x-a)/(b-a),(c-x)/(c-b)),0);
	return y ;
}

float trapmf(float x, float a, float b,float c,float d){
	float y ;
	 y = max(min((x-a)/(b-a),min(1,(d-x)/(d-c))),0);
	return y ;
}

//encuentra la posicion de un elemento en un vector ordenado;
int find_index(float x, float inicio, float paso){
	int index;
	index = (x-inicio)/paso + 0.5;
	if (index > n_max_e-1)
		index = n_max_e-1;
	else if (index < 0)
		index = 0;
	return index;
}

void Reglas_str2num(int *v1,int *v2, int i){
	int n = 0, cont1 = 0, cont2 = 0;
	while (fuzzy.reglas[i][n] != ','){
		if (fuzzy.reglas[i][n] != ' '){
			*(v1+cont1) = fuzzy.reglas[i][n] - '0'-1;
			cont1++;
		}
		n++;
	}
	while (fuzzy.reglas[i][n] != '\0'){
		if (fuzzy.reglas[i][n] != ' ' && fuzzy.reglas[i][n] != ','){
			*(v2+cont2) = fuzzy.reglas[i][n] - '0'-1;
			cont2++;
		}
	n++;
	}
}


float fuzz(int n_entradas, int selec_MFin[n_entradas], int index_in[n_entradas]){
	float fuzzy_value;
    for ( int k = 0 ;k <n_entradas; ++k){
    	if (k == 0){
    		fuzzy_value = *(*(fuzzy.MFin[k]+selec_MFin[k])+ index_in[k]);
    	}
    	else{
    		fuzzy_value = min(fuzzy_value,*(*(fuzzy.MFin[k]+(selec_MFin[k]))+ index_in[k]));
    	}
    }
	return fuzzy_value;
}

float defuzz(float *x, float *mf,int len){
	float out;
	float suma = 0;
	float Area = 0;
	for (int i = 0;i<len;++i){
		suma = suma + (*(x+i))*(*(mf+i));
		Area = Area + (*(mf+i));
	}
	out = suma/Area;
	return out;
}
