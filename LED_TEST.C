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

#define  DIN PC4
//unsigned char color[]={0x00,0x23,0x45,0x65,0x78,0x90,0x45,0x65,09};
#define  led_r  0x77
#define  led_g  0x77
#define  led_b  0x77
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
    unsigned char i=100;
    while(i--)
    {
       NOP();
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

//===========================================================
//Funtion name：io_init
//parameters：无
//returned value：无
//===========================================================
void  io_init(void)
{
   OSCCON = 0B01110001;//16M/2T
   ANSEL = 0b00000000;//关闭模拟端口
   PORTA=0XFF;//PA0设置为输出
   TRISA=0XFF;
   PORTC=0XEF;//PA0设置为输出
   TRISC=0XEF;
   MSCKCON = 0B00000000;//禁用LDO
   CMCON0 = 0B00000111; //关闭比较器，CxIN为数字IO口
}

//===========================================================
//Funtion name：interrupt ISR
//parameters：无
//returned value：无
//===========================================================
void interrupt ISR(void)
{
}
//===========================================================
//Funtion name：main
//parameters：无
//returned value：无
//===========================================================
main()
{ 
    unsigned char k,j,k,m;
    unsigned char am[3]={0x80,0x00,0x00};
    io_init();
    j=0;
    while(1)
    {
    ws2812_rst();
    for (k=8;k>0;k--)
    {
        if(k==j)
        {
          sendbyte(am[0]);
          sendbyte(am[1]);
          sendbyte(am[2]);
        }
        else
        {
           sendbyte(0x00);
           sendbyte(0x00);
           sendbyte(0x00);
        }
    }

for(k=250;k>0;k--)
{
    delay100us();
    delay100us();
    delay100us();
     delay100us();
    delay100us();
}
  j++;
    if(j==9)
    {
        j=1; 
        m=am[0];
        am[0]=am[1];
        am[1]=am[2];
        am[2]=m;     
    }
    }
}
//===========================================================
