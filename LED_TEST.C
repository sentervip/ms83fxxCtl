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
#include 	"MS83Fxx02.h";
//===========================================================
//Variable definition
//===========================================================

#define  DIN PA2
#define  led_r  0xa5
#define  led_g  0x5a
#define  led_b  0xa5
//===========================================================
//Funtion name��sendbyte
//parameters: dat
//returned value����
//===========================================================
void sendbyte(unsigned char dat)
{
    unsigned char j=8;
    while(j--)
    {
    if(dat&0x80)
    {
        DIN=1;
        NOP();
        NOP();
       // NOP();
        DIN=0;
        NOP();
    }
    else
    {
       DIN=1;
        //NOP();
        DIN=0;
        NOP();
       // NOP();
        //NOP();    
    }
    dat<<=1;
    }
    
}

//===========================================================
//Funtion name��delay100us
//parameters: ��
//returned value����
//===========================================================
void delay100us(void)
{
    unsigned char i=100;
    while(i--)
    {
       NOP();
    } 
}


//===========================================================
//Funtion name��ws2812_rst
//parameters: ��
//returned value����
//===========================================================
void ws2812_rst(void)
{
    DIN=1;
    delay100us();
    DIN=0;
    delay100us();
}

//===========================================================
//Funtion name��io_init
//parameters����
//returned value����
//===========================================================
void  io_init(void)
{
   ANSEL = 0b00000000;//�ر�ģ��˿�
   PORTA=0XFB;//PA2����Ϊ���
   TRISA=0XFB;
   OSCCON=0B01110111;//16M/2T
   MSCKCON = 0B00000000;//����LDO
   CMCON0 = 0B00000111; //�رձȽ�����CxINΪ����IO��
}

//===========================================================
//Funtion name��interrupt ISR
//parameters����
//returned value����
//===========================================================
void interrupt ISR(void)
{
}
//===========================================================
//Funtion name��main
//parameters����
//returned value����  
//===========================================================
main()
{ 
    unsigned char k;
    io_init();
    while(1)
    {
    ws2812_rst();
    for (k=8;k>0;k--)
    {
    sendbyte(led_r);
    sendbyte(led_g);
    sendbyte(led_b);
    }
    }
}
//===========================================================