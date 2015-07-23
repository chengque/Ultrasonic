#include<iostream>
#include<wiringPi.h>
#include<errno.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>       //for uint32_t
using namespace std;
uint32_t time1=0,time2=0;
uint32_t time11=0,time12=0;
uint32_t time_diff=0;
uint32_t time_diff1=0;
float Range_cm=0;
float Range_cm1=0;
volatile int flag=0;
volatile int flag1=0;
void show_distance(void);
 
void myInterrupt(void) // Interrupt routine
 {                     // Called whenever the input pin toggles
    uint32_t timeTemp=micros();
    if(flag==0) // first toggle? note the time
      {
            time1=timeTemp;
            flag=1;
 
      }
    else // second toggle? compute the distance
      {
            time2=timeTemp;
            flag=0;
            time_diff=time2-time1;
            Range_cm=time_diff/58.;
       }
  }
void myInterrupt1(void) // Interrupt routine
 {                     // Called whenever the input pin toggles
    uint32_t timeTemp=micros();
    if(flag1==0) // first toggle? note the time
      {
            time11=timeTemp;
            flag1=1;
 
      }
    else // second toggle? compute the distance
      {
            time12=timeTemp;
            flag1=0;
            time_diff1=time12-time11;
            Range_cm1=time_diff1/58.;
       }
  }

void show_distance()// writes the distance as a 
  {                 // binary float to stdout.
//    fwrite(&Range_cm,sizeof(float),1,stdout); 
//    cout.flush();
    printf("%f-%f\r\n",Range_cm,Range_cm1);
  }
 
int main(void)
  {
    if(wiringPiSetup()<0)
     {
       cout<<"wiringPiSetup failed !!\n";
     }
    pinMode(4,OUTPUT);
    pinMode(5,INPUT);
    pinMode(6,INPUT);
    pullUpDnControl(6,PUD_DOWN);
    pullUpDnControl(5,PUD_DOWN);
    if(wiringPiISR(5,INT_EDGE_BOTH,&myInterrupt) < 0)
            {
            cerr<<"interrupt error ["<<strerror (errno)<< "]:"<<errno<<endl;
            return 1;
            }
 
    if(wiringPiISR(6,INT_EDGE_BOTH,&myInterrupt1) < 0)
            {
            cerr<<"interrupt error ["<<strerror (errno)<< "]:"<<errno<<endl;
            return 1;
            }
    while(1) // this loop starts a new measurement
    {        // every 2500 miliseconds
        time1=0;
        time2=0;
        flag=0;
        time11=0;
        time12=0;
        flag1=0;
    digitalWrite(4,0);
    delayMicroseconds(1);
    digitalWrite(4,1);
    delayMicroseconds(10);
    digitalWrite(4,0);
    delayMicroseconds(25000);
    show_distance();
    }
    return 0;
 }
