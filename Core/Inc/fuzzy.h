#ifndef INC_FUZZY_H_
#define INC_FUZZY_H_

//definimos paso entre muestras
#define PASO  0.01;
// definimos el numero maximo de valores
#define n_max_e  201   // n_max_e = n_max_de
#define n_max_kpp 101  //n_max_kpp = n_max_kdp
#define n_max_kip 101
//Numero de MF de entrada y salida
#define num_MFin 2
#define num_MFout 3
//definimos el numero de reglas
#define N_RULES 49

typedef struct {
	//Declaracion del rango de valores de las funciones: error, delta error, kpp, kdp y alpha.
	int range_e[2];
	int range_de[2];
	int range_kpp[2];
	int range_kdp[2];
	int range_kip[2];

	float  **MFin[num_MFin];
	float  **MFout[num_MFout];
	int len_MFout[num_MFout];

	float *eMF[7];
	float *deMF[7];
	float *kppMF[2];
	float *kdpMF[2];
	float *kipMF[4];

	float *fun_in_out[num_MFin+num_MFout];
	// declaracion de las funciones entrada y salida: error, delta error, kpp, kdp y alpha
	float error[n_max_e], delt_error[n_max_e],kpp[n_max_kpp], kdp[n_max_kpp], kip[n_max_kip];
	//declaracion  MF(funciones de membrecia) de entrada
	float eNB[n_max_e],eNM[n_max_e],eNS[n_max_e],eZO[n_max_e],ePS[n_max_e],ePM[n_max_e],ePB[n_max_e];
	float deNB[n_max_e],deNM[n_max_e],deNS[n_max_e],deZO[n_max_e],dePS[n_max_e],dePM[n_max_e],dePB[n_max_e];
	//declaracion MF(funciones de membrecia) de salida
	float small1[n_max_kpp],big1[n_max_kpp];
	float small2[n_max_kpp],big2[n_max_kpp];
	float S[n_max_kip], MS[n_max_kip],MB[n_max_kip], B[n_max_kip];

	char *reglas[N_RULES];
} fuzzy_s;

// prototipos de las funciones
float min(float a, float b);
float max(float a, float b);
float gaussmf(float x, float sigma, float media);
float trimf(float x, float a, float b,float c);
float trapmf(float x, float a, float b,float c,float d);
int find_index(float x, float inicio, float paso);
void eval_fuzzy(float *entradas, float *salidas);
float fuzz(int n_entradas, int selec_MFin[n_entradas], int index_in[n_entradas]);
float defuzz(float *x, float *mf,int len);
void Reglas_str2num(int *v1,int *v2, int i);
void Fuzzy_Init();

#endif /* INC_FUZZY_H_ */
