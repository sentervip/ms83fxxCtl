//Project: LED_TEST.prj
// Device: MS83Fxx02
// Memory: Flash 2KX14b, EEPROM 256X8b, SRAM 128X8b
// Author: 
//Company: 
//Version:
//   Date: 
//===========================================================
//===========================================================
#include	"SYSCFG.h";
#include 	"MS83Fxx02.h"
#include "stdint.h"
//===========================================================
//Variable definition
//===========================================================

#define  DIN PC4
#define  LED_IN  PA7
#define  DATA_LED   PORTC
#define  RED_LED     0x02
#define  LED_NUM    (5+1)
//unsigned char color[]={0x00,0x23,0x45,0x65,0x78,0x90,0x45,0x65,09};
#define  led_r  0xff
#define  led_g  0xff
#define  led_b  0xff 
//===========================================================
//Funtion name：sendbyte
//parameters: dat
//returned value：无
//===========================================================
void sendbyte(unsigned char dat)
{
    unsigned char j=8;
    while(j--)
    {
    if(dat&0x80)
    {
        DIN=1;
        DIN=1;
        DIN=1;
        DIN=1;
        DIN=1;
        DIN=0;

    }
    else
    {
        DIN=1;
        DIN=1;
        DIN=1;
        DIN=0;
        DIN=0;
        DIN=0;
    }
    dat<<=1;
    }
    //DIN=1;
}

//===========================================================
//Funtion name：delay100us
//parameters: 无
//returned value：无
//===========================================================
void delay100us(void)
{
    unsigned char i=250;
    while(i--)
    {
       NOP();
    } 
}

void delay1s(unsigned char i)
{
   unsigned char j = 250;
    while(i--)
    {
        while(j--){
            delay100us();
       }
       CLRWDT();
    } 
}

//===========================================================
//Funtion name：ws2812_rst
//parameters: 无
//returned value：无
//===========================================================
void ws2812_rst(void)
{
    DIN=1;
    delay100us();
    DIN=0;
    delay100us();
}
void delay2s(unsigned int t)
{
    unsigned int i,j;
    for(i=0;i<t;i++)
       delay100us();  
}
//===========================================================
//Funtion name：io_init
//parameters：无
//returned value：无
//===========================================================
void  io_init(void)
{
   OSCCON = 0B01110001;//16M/2T
   ANSEL = 0b00000000;//关闭模拟端口
   //PA
	//PORTA = 0B11111111;
	//TRISA = 0B11111111;		//PA7输入输出 0-输出 1-输入
	//WPUA =  0B10000000;		//PA7端口上拉控制 1-开上拉 0-关上拉
    //PC
   PORTC=0b11100000;//PC4设置为输出
   TRISC=0b11100000;
   WPUC =  0B00011111;		//PA7端口上拉控制 1-开上拉 0-关上拉
   MSCKCON = 0B00000000;//禁用LDO
   CMCON0 = 0B00000111; //关闭比较器，CxIN为数字IO口
}
void DEVICE_INIT(void)
{
	OSCCON = 0B01010001;	//Bit7,LFMOD=0,WDT振荡器频率=32KHz
							//Bit6:4,IRCF[2:0]=101,内部RC频率=4MHz
							//Bit0,SCS=1,系统时钟选择为内部振荡器

	INTCON = 0B00000000;	//暂禁止所有中断

	CMCON0 = 0B00000111;	//关闭比较器，CxIN为数字IO口
	ANSEL = 0B00000000;		//关闭所有模拟口

	PORTA = 0B00000000;
	TRISA = 0B11111111;		//PA输入输出 0-输出 1-输入
	WPUA =  0B00000000;		//PA端口上拉控制 1-开上拉 0-关上拉

	PORTC = 0B00000000;
	TRISC = 0B00100100;		//PC输入输出 0-输出 1-输入 
	WPUC =  0B00000100;		//PC端口上拉控制 1-开上拉 0-关上拉

	OPTION = 0B00001000;	//Bit4=1 WDT MODE,PS=000=1:1 WDT RATE
}
void PWM_INIT(void)
{
	PWM3CR0 = 0b00000000;
	PWM3CR1 = 0b00000000;
	PWM4CR0 = 0b00000000;
	PWM4CR1 = 0b00000000;
	PWM5CR0 = 0b00000000;
	PWM5CR1 = 0b00000000;
}

/*====================================================
*函数名:SET_PWM3_ON
*功能:开启PWM3
*输入参数:无
*返回参数:无
====================================================*/
void SET_PWM5_ON(unsigned char data)
{
    
	TRISC2 = 1;
	PWM5CR0 = 0b00110010;
	PWM5CR1 = 0b10000000;
	T5CKDIV = 7;
	TMR5H = 0;
	TMR5L = 0;
	PR5L = 50;//128;
	TMR5IF = 0;
	TMR5ON = 1;
	while(TMR5IF==0) CLRWDT();
	TRISC2 = 0;
}
void PWM5_ONOFF(unsigned char sw)
{
    if(sw){
        TMR5IF = 1;    
        TMR5ON = 1;
    }else{
        TMR5IF = 0;    
        TMR5ON = 0;
        PR5L = 0;
        TRISC2 = 0;
    }
}
//===========================================================
//Funtion name：interrupt ISR
//parameters：无
//returned value：无
//===========================================================
void interrupt ISR(void)
{
}
void main2()
{
    char  count =0x01;
   
   while(1){      
    io_init();
    PWM_INIT();  
   for( count = 0; count <= LED_NUM; count++){
       if(count == LED_NUM){ 
           count = 0;          
       }
        
       
       if(count == RED_LED){
             PWM5_ONOFF(1);   
       }else{
             DATA_LED  = 1 << count;
             PWM5_ONOFF(0);    
     }
       delay1s(2);    
   }
}

}
void main(void)
{
    unsigned char count;
    
	DEVICE_INIT();     //器件初始化
	PWM_INIT();        //PWM初始化
	SET_PWM5_ON(0);     //开启PWM3
	while(1)
	{
		CLRWDT();
         for( count = 0; count <= LED_NUM; count++){
			 if(count == LED_NUM){ 
				count = 0;          
			}
			
            
			 if(count == RED_LED){
                    DATA_LED = 0;
                    //PWM_INIT();
                     SET_PWM5_ON(1);  
                     
			   }else{				
					 PWM5_ONOFF(0);    
                     DATA_LED  = 1 << count;
			 }
			  
			   delay1s(8);    
        }
	}
}