/*
 * GccApplication34.cpp
 *
 * Created: 10/20/2015 1:36:34 PM
 *  Author: Mosam
 */ 

// Stopwatch Timer with 7-segment display on AT89C51

#include<reg51.h>
#define msec 1
unsigned int second1,second2;
int second1_1,second1_2,second2_1,second2_2;

unsigned int digi_val[10]={0x40,0xF9,0x24,0x30,0x19,0x12,0x02,0xF8,0x00,0x10};
sbit dig_ctrl_1=P1^0;  // Declare the control pins of seven segments
sbit dig_ctrl_2=P1^1;
//sbit dig_ctrl_3=P1^2;
//sbit dig_ctrl_4=P1^3;
sbit start_pin = P1^4;  // Start pin to start the watch.
sbit stop_pin = P1^5;  // Stop pin to stop the watch.
sbit reset_pin = P1^6;  // Reset pin to reset the watch.
int s,t;

void mplex_delay(unsigned int time)  // Function to provide a time delay of approximatelty one second using Timer 1
{
int i,j;
for (i=0;i<=time;i++)
  for(j=0;j<=50;j++);
}

void digi_out(unsigned int current_num)
{
    P2=digi_val[current_num];
     mplex_delay(msec);
}

void display(unsigned int dig1)  // Function to display the digits on seven segmnet. For more details refer seven segment multiplexing.
{
    second1_2=dig1%10;
second1_1=dig1/10;
//second2_2=dig2%10;
//second2_1=dig2/10;
TMOD=0x01;  //Enable Timer 0
TL0=0xFF;
TH0=0xDB;
TR0=1;  // Triger Timer 0
while(TF0==0)
{
  dig_ctrl_1 = 1;
  dig_ctrl_2 = 0;
  digi_out(second1_1);
  dig_ctrl_2 = 1;
  dig_ctrl_1 = 0;
  digi_out(second1_2);
  //dig_ctrl_3 = 1;
  //dig_ctrl_2 = dig_ctrl_1 = dig_ctrl_4 = 0;
  //digi_out(second2_1);
  //dig_ctrl_4 = 1;
  //dig_ctrl_2 = dig_ctrl_3 = dig_ctrl_1 = 0;
  //digi_out(second2_2);
}

TR0=0;
TF0=0;
}

void main()
{
while(1)
{
start:  // Segment to start the stop watch
  start_pin = 1;
  stop_pin = 1;
  reset_pin = 1; 
  dig_ctrl_1 = 0;
  dig_ctrl_2 = 0;
  //dig_ctrl_3 = 0;
  //dig_ctrl_4 = 0;
  P2 = 0xFF;
  s = t = 0;
  while(start_pin == 1)// Check if start pin is pressed
  {
  display(0,0);
  }

stopwatch:  // Segment to stop the watch
  for (second1=s;second1<=99;second1++)
  {
   if (stop_pin == 0 )  //Check if stop pin is pressed
   break;
    //for (second2=t;second2<=99; second2++)
    //{
    //if (stop_pin == 0 )  //Check if stop pin is pressed
    //break;
    t=0;
    display(second1);
    //}
  }
  stop_pin = 1;
  s = second1;
  //t = second2;
 
  while ( start_pin != 0 && reset_pin != 0 )  //Check if start pin or reset pins are not pressed
  {
  display(second1);
  }
 
  if (start_pin == 0)  //Check if start pin is pressed
  {
  goto stopwatch;
  }
  else
  {
   if (reset_pin == 0 )  //Check if reset pin is pressed
   {
   s = t = 0;
   goto start;
   }
  }
}
}
