#ifndef MATH_VECTOR_H_
#define MATH_VECTOR_H_



void sum_vector_escalar(int len_vector,float v[len_vector],float valor, float *out);
void prod_vector_escalar(int len_vector,float v[len_vector],float valor, float *out);
float sum_vector(int muestras,float v[muestras]);
void linspace(float *vector,float inicio,float fin, int muestras);
void generate_vector(float *vector ,float inicio, float fin, float paso);
void mostrar_vector(float *vector, int len, char *name);
void mostrar_vector_p(double *vector, int len,char *name);
void mostrar_vector_int(int *vector, int len,char *name);
int find(float value, int len, float *v, float error);
int find_int(int value, int len, int *v, float error);
float min_v(int len, float *v);
int setdiff(int *len_dir,float *v,float value);
int setdiff_int(int *len_dir,int *v,int value);
void remove_value_position(int *len_dir,float *v, int pos);
#endif /* MATH_VECTOR_H_ */
