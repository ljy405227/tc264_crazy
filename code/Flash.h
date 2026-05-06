#ifndef  _FLASH_H_
#define  _FLASH_H_

#define Flash_Buffer_NUM 85                  //宏定义写入Flash参数的个数，根据需要修改    你需要写几个参数就把它改成多少个，一个数占一个
                                             //这个数可以大于你要存的参数个数，但是不能小于
extern float   Flash_Read_Buffer_float[Flash_Buffer_NUM];

extern void Startingup_Flash_Value_Updata(void);
extern void Flash_Value_Updata(void);
extern void Flash_Value_Read(void);
extern void Flash_Value_Write(void);
extern void flash(void);

#endif
