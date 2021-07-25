/*
 * GccApplication1.c
 *
 * Created: 14-Jul-19 8:41:46 AM
 *  Author: mishr
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB=0xff;
	adc_init();
	lcd_init();
	lcd_string("TEMP. SENSOR");
    while(1)
    {
		adc_conversion();
    }
}

void adc_init()
{
	ADCSRA=0XA0;
	ADMUX=0X40;
}

void adc_conversion()
{
	ADCSRA|=(1<<6);
	while((ADCSRA&(1<<4))==0);
	float x=ADC;
	float t=((x*5)/1023)*100;
	ascii(t);
}

void lcd_cmd(char x)
{
	PORTB=x&0xf0;
	PORTB&=~(1<<0);
	PORTB&=~(1<<1);
	PORTB|=(1<<2);
	_delay_ms(1);
	PORTB&=~(1<<2);
	_delay_ms(1);
	
	PORTB=((x<<4)&0xf0);
	PORTB&=~(1<<0);
	PORTB&=~(1<<1);
	PORTB|=(1<<2);
	_delay_ms(1);
	PORTB&=~(1<<2);
	_delay_ms(1);
}

void lcd_init()
{
	lcd_cmd(0x02);
	lcd_cmd(0x28);
	lcd_cmd(0x0e);
	lcd_cmd(0x06);
	lcd_cmd(0x80);
}

void lcd_data(char x)
{
	PORTB=(x&0xf0);
	PORTB|=(1<<0);
	PORTB&=~(1<<1);
	PORTB|=(1<<2);
	_delay_ms(1);
	PORTB&=~(1<<2);
	_delay_ms(1);
	
	PORTB=((x<<4)&0xf0);
	PORTB|=(1<<0);
	PORTB&=~(1<<1);
	PORTB|=(1<<2);
	_delay_ms(1);
	PORTB&=~(1<<2);
	_delay_ms(1);
}

void lcd_string(char *x)
{
	while(*x)
	{
		lcd_data(*x);
		x++;
	}
}

void ascii(int t)
{
	int b;
	char pos=0xc3;
	lcd_cmd(0xc5);
	lcd_data(0xdf);
	lcd_cmd(0xc6);
	lcd_data('C');
	for (int i=0;i<=3;i++)
	{
		b=t%10;
		t=t/10;	
		lcd_cmd(pos);
		lcd_data(b+48);
		pos--;
	}
}