#ifndef PID_H_
#define PID_H_

typedef struct
{
    float r;
    float e[3];
    float Kp,Ki,Kd;
    float y;
    float u;
    float Gains[3];
} pid_s;


void PID_Init(pid_s *pid, int select);
float PID_control(float *e, float Gains[3],float Ts);


#endif /* PID_H_ */
