	 //I2C EEPROM Read/Write single byte Using LPC21xx with LCD Display
#include<lpc21xx.h>

void i2c_init(void);
void start(void);	
void stop(void);
void write(char);
char read(unsigned char);
char ack(void);

void i2c_init()
{
	PINSEL0 |=0x50; //Configures P0.2(SCL) and P0.3(SDA) pins for I2C function
	I2SCLL=75;      //Sets low time of SCL clock
	I2SCLH=75;      //Sets high time of SCL clock
	I2CONSET=1<<6;  //Enables I2C interface (I2EN bit)
}
void start()
{
	 I2CONSET=1<<5;               //Sets STA bit-> generates START condition
	 I2CONCLR=1<<3;               //Clears SI flag (allows I2C to proceed)
	 while(((I2CONSET>>3)&1)==0); //Waits until SI flag is set
	 I2CONCLR=1<<5;               //Clears START bit
}
void stop()
{
	I2CONSET=1<<4;  //Sets STO bit-> generates STOP condition
	I2CONCLR=1<<3;  //Clears SI flag
}
void write(char x)
{
	I2DAT=x;                    //Loads data byte into I2C data register
	I2CONCLR=1<<3;              //Clears SI flag to start transmission
	while(((I2CONSET>>3)&1)==0);//Waits until SI flag is set
}
char read(unsigned char addr)
{
	char ch;     //Variable to store received data
	start();     //Sends START condition
	write(0xa0); //Sends EEPROM address in write mode
	write(addr); //Sends memory location address
	start();     //Sends repeated START
	write(0xa1); //Sends EEPROM address in read mode
	ch=ack();    //Reads data and sends ACK
	return ch;   //Returns received byte
}
char ack()
{
	char ch;
	I2CONSET=1<<2;               //Enable Asset Acknowledge bit
	I2CONCLR=1<<3;               //Clears SI flag
	while(((I2CONSET>>3)&1)==0); //Waits for data reception
	ch=I2DAT;                    //Reads received byte from I2C data register
	return ch;                   //Returns data
}
