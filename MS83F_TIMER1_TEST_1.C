/**********************************************************
*文件名:MS83F_TIMER1_TEST_1.C
*功能:MS83Fxx02的TIMER1定时应用演示
*器件型号:MS83F1602
*振荡器:内部RC 4MHz 2T
*引脚定义:
*                 ----------------
*  VDD-----------|1(VDD)   (GND)16|------------GND
*  NC------------|2(RA7)   (PA0)15|-------------NC
*  NC------------|3(PA6)   (PA1)14|-------------NC
*  NC------------|4(PA5)   (PA2)13|---------OUTPUT
*  NC------------|5(PC3)   (PA3)12|-------------NC
*  NC------------|6(PC2)   (PC0)11|-------------NC
*  NC------------|7(PA4)   (PC1)10|-------------NC
*  NC------------|8(PC5)   (PC4)09|-------------NC
*                 ----------------
*                 MS83F1602 SOP16
*说明:设置TIMER1的分频比为1:4，内部振荡器为4MHz(2T)，TIMER1初值为15535，
      故TIMER1溢出时间为100ms。因此RA2每100ms翻转一次。
      如果把指令周期设置为4T，那么RA2每200ms翻转一次。
**********************************************************/
#include	"syscfg.h";
#include 	"MS83Fxx02.h";

#define DATA_LED		PORTC
#define  RED_LED     0x02
#define  LED_NUM    (5+1)
#define PWM_OK      30 
unsigned char loop = 0;

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
   unsigned char j = 150;
    while(i--)
    {
        while(j--){
            delay100us();
       }
       CLRWDT();
    } 
}
/*====================================================
*函数名:interrupt ISR
*功能:中断服务函数
*输入参数:无
*返回参数:无
====================================================*/
void interrupt ISR(void)
{
    static  unsigned char cnt1 = 0,cnt2=0;
	if(TMR1IE&&TMR1IF)
	{
		TMR1IF = 0;
		TMR1H = 0xfe;//1ms//0xec;//10ms ;
  		TMR1L = 0x0b;//0x77;;
        
        cnt1++;
        if(cnt1 >10)  {
            cnt1 =0;
        }

             if( cnt1 %3 == 0){
                 DATA_LED  = 1 << loop;
                
            }else{
                DATA_LED = 0;
           
              
	  }
		
	}
}
//[TMR1H:TMR1L]=0x3CAF=15535,2T指令周期,内部RC频率4MHz,预分频值1:8
//定时溢出时长=(1/4MHz)*2*4*(65535-15535)=100ms
//也就是RA0每100毫秒翻转一次
/*====================================================
*函数名:DEVICE_INIT
*功能:上电器件初始化
*输入参数:无
*返回参数:无
====================================================*/
void DEVICE_INIT(void)
{
	OSCCON = 0B01010001;	//Bit7,LFMOD=0,WDT振荡器频率=32KHz
							//Bit6:4,IRCF[2:0]=101,内部RC频率=4MHz
							//Bit0,SCS=1,系统时钟选择为内部振荡器

	INTCON = 0B00000000;	//暂禁止所有中断

	CMCON0 = 0B00000111;	//关闭比较器，CxIN为数字IO口
	ANSEL = 0B00000000;		//关闭所有模拟口

	//PORTA = 0B00000000;
	//TRISA = 0B11111011;		//PA输入输出 0-输出 1-输入
	//WPUA =  0B00000000;		//PA端口上拉控制 1-开上拉 0-关上拉

	PORTC = 0B00000000;
	TRISC = 0B11100000;		//PC输入输出 0-输出 1-输入 
	WPUC = 0B11100000;		//PC端口上拉控制 1-开上拉 0-关上拉

	OPTION = 0B00001000;	//Bit4=1 WDT MODE,PS=000=1:1 WDT RATE
}

/*====================================================
*函数名:TIMER1_INIT
*功能:Tiemr1初始化
*输入参数:无
*返回参数:无
====================================================*/
void TIMER1_INIT(void)
{
	TMR1H = 0xfe;
    TMR1L = 0x0b;			//Timer1初值为0x3CAF=15535
	T1CON = 0B00100001;		//预分频值=1:4
}

/*====================================================
*函数名:ENABLE_INTERRUPT
*功能:开启所需中断
*输入参数:无
*返回参数:无
====================================================*/
void ENABLE_INTERRUPT(void)
{
	TMR1IF = 0;				//清TIMER1中断标志位
	TMR1IE = 1;				//使能TIMER1中断
    PEIE = 1;
	GIE = 1;				//开启总中断
} 
//注:Timer1属于外设,所以要开启Timer1溢出中断，必须把外设总闸PEIE开启
/*====================================================
*函数名:main
*功能:主函数
*输入参数:无
*返回参数:无
====================================================*/
void main(void)
{
	DEVICE_INIT();			//器件初始化
	TIMER1_INIT();			//TIMER1初始化
	ENABLE_INTERRUPT();
	while(1)
	{
        CLRWDT();
        DEVICE_INIT();			//器件初始化
	    TIMER1_INIT();			//TIMER1初始化
	   ENABLE_INTERRUPT();
        for( loop=0; loop <= LED_NUM; loop++){
            CLRWDT();
			 if(loop == LED_NUM){ 
				loop = 0;          
			}		 
            
			 PEIE = 0;
             DATA_LED = 0;
             TIMER1_INIT();
			 delay1s(2); 
             
			 PEIE = 1;   
			 delay1s(2); 
        }
        
	}
}