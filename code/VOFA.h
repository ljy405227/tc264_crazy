#ifndef CODE_VOFA_H_
#define CODE_VOFA_H_

typedef union
{
    float fdata;
    unsigned long ldata;
}FloatLongType;

void Float_to_Byte(float f,unsigned char byte[]);
void Byte_to_Float(float *f,unsigned char byte[]);
void VOFA_Send(float t);
void VOFA_draw(void);
void My_Printf(float Data);
void My_Vofa_Send(void);




#endif /* CODE_VOFA_H_ */
