#include "Arduino.h"

#include <Encoder.h>

// The types of the position, velocity, and acceleration measurements (respectively)
#define P_type int
#define V_type int
#define A_type int

#define DATA_SIZE ( (sizeof(P_type)+sizeof(V_type)+sizeof(A_type))/sizeof(byte) )
// The size of the message in bytes
#define MSG_SIZE ( (DATA_SIZE + 3/*P,V,A*/ + 1/*L|R*/)*2/*BOTH*/ +1/*NULL*/)
// The rate of communication
#define BAUD_RATE 115200

// Goes in setup() somewhere
#define SETUP_CONNECTION Serial.begin(BAUD_RATE) 

// Basic structure for organizing measurement data
class data
{
public:
    P_type _position;
    V_type _velocity;
    A_type _xlr8tion;
};
/*Sourced from-- http://www.pololu.com/product/1217  */
const int linear_rez = 3;//mm == 1/8 inch
const int count_per_rev = 48;



const int SEC = 60;
const int x = 2;
const int N = 100;

/*number of pulses generated by the encoder per shaft revolution*/
// not sure what value to try 
long positionLeft  = -999;
long positionRight = -999;
const int FTI = 60; //in seconds 
const int encoder_pulse = 48;
int PowerMotorR = 12; 
int PowerEncR = 13;
int EncA = 5;
int EncB = 6;

Encoder MotorLeft(2,3);
Encoder MotorRight(5,6);

void setup() {
  // put your setup code here, to run once:
  pinMode(PowerMotorR,OUTPUT);
  pinMode(PowerEncR, OUTPUT);
  pinMode(EncA,INPUT);
  pinMode(EncB, INPUT);
  SETUP_CONNECTION;
  int velocity();
  int acceleration();
  int positionm();
  void UART();
}



void loop() {
  // put your main code here, to run repeatedly:
  long newLeft, newRight;
  newLeft = digitalRead(MotorLeft.read());
  newRight = digitalRead(MotorRight.read());
  Serial.print(newLeft);
  Serial.print("/n");
  if (newLeft != positionLeft || newRight != positionRight) {
    Serial.print("Left = ");
    Serial.print(newLeft);
    Serial.print(", Right = ");
    Serial.print(newRight);
    Serial.println();
    positionLeft = newLeft;
    positionRight = newRight;
  }
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both Motors to zero");
    MotorLeft.write(0);
    MotorRight.write(0);
  }
  UART(); //may need a delay 
}

int velocity()
{
  /* velocity in terms of RPM */
  /* velocity is taken over the span of a minute (60 sec) this can be altered if needed */
  // number can be found by figuring out how many pulses occur during a fixed time interval
  int PPV;
  int velocity;
  velocity = SEC*((encoder_pulse)/PPV)/FTI;

  return velocity;
}

int acceleration()
{
  int accel;


  return accel;
}


int positionm()
{
  /*rotational position*/
  int Edge_Count = MotorLeft.read(); /*this is starting position */
  int rotations = (360*(Edge_Count))/(N*x);


  return rotations;
}

void UART()
{
  int p = positionm();
  int a = acceleration();  
  int v = velocity();
  //Serial.write(p,a,v,MotorLeft_.read(),MotorRight.read());  
}


/* Parses the value pointed to by _data into bytes and fills the _array
    with them starting with the index of the pointer. The number of 
    bytes parsed whould be equal to sizeof(_data)/sizeof(byte), which
    should be equal to _num_bytes. The pointer returns is to the index
    of _array which immediately follows the last byte of _data written.
 */
byte* fill_byte(byte * _array, void * _data, size_t _num_bytes)
{
    byte* _tmp = (byte*)_data;
    for(size_t i =0; i< _num_bytes; i++ )
    {
        (*_array++) = (*_tmp++);
    }
    return _array;
}

/* Fills out the global buffer with the provided data and sends it via
    Serial.write() to the receiver. The buffer is also filled with index
    markers to delineate the measurements and each wheel.
 */
int send_data( data  _L/*left wheel*/, data  _R/*right wheel*/)
{
    byte * _tmp = buf;
    
    *_tmp++ = 'L';
    *_tmp++ = 'P';
    _tmp = fill_byte(_tmp,&(_L._position),sizeof(P_type)/sizeof(byte));  
    *_tmp++ = 'V';
    _tmp = fill_byte(_tmp,&(_L._velocity),sizeof(V_type)/sizeof(byte));    
    *_tmp++ = 'A';
    _tmp = fill_byte(_tmp,&(_L._xlr8tion),sizeof(A_type)/sizeof(byte));
        
    *_tmp++ = 'R';
    *_tmp++ = 'P';
    _tmp = fill_byte(_tmp,&(_R._position),sizeof(P_type)/sizeof(byte));  
    *_tmp++ = 'V';
    _tmp = fill_byte(_tmp,&(_R._velocity),sizeof(V_type)/sizeof(byte));    
    *_tmp++ = 'A';
    _tmp = fill_byte(_tmp,&(_R._xlr8tion),sizeof(A_type)/sizeof(byte));
    
    *_tmp++ = '\0';
    
    // Serial.write(buf, MSG_SIZE);
    return 0;
} 


