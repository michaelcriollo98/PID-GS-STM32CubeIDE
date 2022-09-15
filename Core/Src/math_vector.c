
#include "math_vector.h"
#include <stdio.h>
#include <math.h>


void sum_vector_escalar(int len_vector,float v[len_vector],float valor, float *out){
	for ( int i = 0;i<len_vector;++i){
		out[i] = valor + v[i];
	}
}
void prod_vector_escalar(int len_vector,float v[len_vector],float valor, float *out){
	for ( int i = 0;i<len_vector;++i){
		out[i] = valor*v[i];
	}
}

float sum_vector(int muestras,float v[muestras])
{
	float sum = 0;
	for (int i=0;i<muestras;++i){
		sum+=v[i];
	}
	return sum;
}


void linspace(float *vector,float inicio,float fin, int muestras){
	float paso = (float)(fin-inicio)/(muestras-1);
	for (int i = 0;  i < muestras; ++i ){
			*(vector + i) = (float)inicio + paso*i;
		}
}

void generate_vector(float *vector ,float inicio, float fin, float paso){
	int n = ((fin - inicio)/paso)+1.000009;
	for (int i = 0;  i < n; ++i ){
			*(vector + i) = inicio + paso*i;
		}
}

void mostrar_vector(float *vector, int len,char *name){
	printf("%s = [",name);
	for (int i = 0;  i < len; ++i ){
			printf("%f ",vector[i]);
		}
	printf("]';\n");
}

void mostrar_vector_d(double *vector, int len,char *name){
	printf("%s = [",name);
	for (int i = 0;  i < len; ++i ){
			printf("%f ",vector[i]);
		}
	printf("]';\n");
}


void mostrar_vector_int(int *vector, int len,char *name){
	printf("%s = [",name);
	for (int i = 0;  i < len; ++i ){
			printf("%d ",*(vector+i));
		}
	printf("]';\n");
}



int find(float value, int len, float *v, float error){
	int index = -1;
	if (len>=2){
		if (fabsf(v[1]-v[0]) - (v[len-1] - v[len-2]) < 0.0000009 && error == -1)
		{
			index = (value - v[0])/(v[1]-v[0]) + 0.5;
			if (index > len-1)
				index = len-1;
			else if (index < 0)
				index = 0;
		}
		else
		{
			int i = 0;
			while( i< len && fabsf(value) != INFINITY)
			{
				if (fabsf(value-v[i])< error + 0.0000009){
					index = i;
					break;
				}
				i++;
			}
			if (fabsf(value) == INFINITY)
			{
				int i = 0;
				while( i< len )
				{
					if (value == v[i] ){
						index = i;
						break;
					}
					i++;
				}
			}

		}
	}
	else if (len == 1)
	{
		if (value == v[0] )
			index = 0;
	}
	return index;
}



int find_int(int value, int len, int *v, float error){
	int index = -1;
			int i = 0;
			while( i< len){
				if (fabsf(value-v[i])< error + 0.0000009){
					index = i;
					break;
				}
				i++;
			}
	return index;
}


float min_v(int len, float *v){
	float a = v[0];
	for (int i = 1;i<len;++i){
		a = a > v[i] ? v[i] : a;
	}
	return a;
}

int setdiff(int *len_dir,float *v,float value){
	int len =  *len_dir;
	float aux[len-1];
	int cont = 0 ,j = 0, pos_removed = -1;
	for(int i = 0 ;i<len;++i)
	{
		if (fabsf(value-v[i])< 0.0000009 && cont == 0){
			cont = 1;
			pos_removed = i;
		}
		else{
			aux[j] = v[i];
			j++;
		}
	}
	if (cont == 1)
	{
		for(int i =0 ;i<len-1;++i){
			v[i] = aux[i];
		}
		(*len_dir)--;
	}
	return pos_removed;
}

void remove_value_position(int *len_dir,float *v, int pos){
	int len = *len_dir;
	if (pos>=0 && pos<len)
	{
		for (int i = pos;i<len-1;++i)
		{
			v[i] = v[i+1];}
		(*len_dir)--;
	}
}

int setdiff_int(int *len_dir,int *v,int value){
	int len =  *len_dir;
	float aux[len-1];
	int cont = 0 ,j = 0, pos_removed = -1;
	for(int i = 0 ;i<len;++i)
	{
		if (fabsf(value-v[i])< 0.0000009 && cont == 0){
			cont = 1;
			pos_removed = i;
		}
		else{
			aux[j] = v[i];
			j++;
		}
	}
	if (cont == 1)
	{
		for(int i =0 ;i<len-1;++i)
		{
			v[i] = aux[i];
		}
		(*len_dir)--;
	}
	return pos_removed;
}
