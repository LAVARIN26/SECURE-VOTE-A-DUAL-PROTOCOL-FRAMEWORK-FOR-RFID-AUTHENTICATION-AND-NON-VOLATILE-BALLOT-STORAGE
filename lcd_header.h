// 4 pins LCD DRIVER PROGRAM
#include<LPC21XX.H>
#define RS 1<<14
#define LCD 0xF<<8
#define EN 1<<15

void delay(int ms)
{
	T0PR=15000-1;
	T0TCR=0X01;
	while(T0TC<ms);
	T0TCR=0X03;
	T0TCR=0X00;
}

void LCD_INIT(void);
void LCD_CMD(unsigned char);
void LCD_DATA(unsigned char);
void STRING(unsigned char *);
void INTEGER(int);
void LCD_FLOAT(float);
void LCD_ROLLING_STRING(unsigned char *);

void FLOAT(float f)
{
	int x=f;
	INTEGER(x);
	LCD_DATA('.');
	f=(f-x)*10;
	INTEGER(f);

}
void INTEGER(int n)
{
	unsigned char a[5];
	signed char i=0;
	if(n==0){
		LCD_DATA('0');
	}
	else if(n<0){
		LCD_DATA('-');
		n=-n;
	}
	while(n>0)
	{
		a[i++]=n%10;
		n=n/10;
	}
	for(--i;i>=0;i--)
	{
	 	LCD_DATA(a[i]+48);
	}
}
void LCD_INIT(void)
{
	IODIR0 = LCD | RS | EN;
	LCD_CMD(0x01);
	LCD_CMD(0x02);
	LCD_CMD(0x0c);
	LCD_CMD(0x28);
}
void LCD_CMD(unsigned char cmd)
{
	IOCLR0 = LCD;
	IOSET0 = ((cmd>>4)&0x0f)<<8;
	IOCLR0 = RS;
	IOSET0 = EN;
	delay(2);
	IOCLR0 = EN;

	IOCLR0 = LCD;
	IOSET0 = (cmd&0x0f)<<8;
	IOCLR0 = RS;
	IOSET0 = EN;
	delay(2);
	IOCLR0 = EN;
}

void LCD_DATA(unsigned char d)
{
	IOCLR0=LCD;
	IOSET0=((d>>4)&0x0f)<<8;
	IOSET0 = RS;
	IOSET0 = EN;
	delay(2);
	IOCLR0 = EN;

	IOCLR0=LCD;
	IOSET0=(d&0x0f)<<8;
	IOSET0 = RS;
	IOSET0 = EN;
	delay(2);
	IOCLR0 = EN;
}

void STRING(unsigned char *s)
{
	while(*s)
	{
		LCD_DATA(*s++);
	}
}
void LCD_ROLLING_STRING(unsigned char* s)
{
	int i,j,n;
	
		for(i=0;s[i];i++);
		n=i;
		
		for(i=0;i<n;i++){
			LCD_CMD(0X80);
			for(j=0;j<16;j++){
				LCD_DATA(s[(i+j)%n]);
			}
		delay(200);
		}
}
