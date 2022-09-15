/*
 * process_signal.h
 *
 *  Created on: 14 jul. 2022
 *      Author: Usuario
 */

#ifndef INC_PROCESS_SIGNAL_H_
#define INC_PROCESS_SIGNAL_H_

float saturation(float value,float interval[2]);
float normalizacion (float value, float interval[2],float scaling[2]);
float desnormalizacion (float value, float interval[2],float scaling[2]);
float fround(float X, int N);
float limitar_altura(float h , int *M);

#endif /* INC_PROCESS_SIGNAL_H_ */
