#define byte unsigned char

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
// #define SETUP_CONNECTION Serial.begin(BAUD_RATE); 

/*Sourced from-- http://www.pololu.com/product/1217  */
const int linear_rez = 3;//mm == 1/8 inch
const int count_per_rev = 48;

// Basic structure for organizing measurement data
typedef struct
{
    P_type position;
    V_type velocity;
    A_type xlr8tion;
}data;

// Global buffer for sending data
byte buf [MSG_SIZE]; 

/* based on -- http://www.ni.com/white-paper/7109/en/  */
int position();

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
void send_data( data& _L/*left wheel*/, data& _R/*right wheel*/)
{
    byte * _tmp = buf;
    
    *_tmp++ = 'L';
    *_tmp++ = 'P';
    _tmp = fill_byte(_tmp,&(_L.position),sizeof(P_type)/sizeof(byte));  
    *_tmp++ = 'V';
    _tmp = fill_byte(_tmp,&(_L.velocity),sizeof(V_type)/sizeof(byte));    
    *_tmp++ = 'A';
    _tmp = fill_byte(_tmp,&(_L.xlr8tion),sizeof(A_type)/sizeof(byte));
        
    *_tmp++ = 'R';
    *_tmp++ = 'P';
    _tmp = fill_byte(_tmp,&(_R.position),sizeof(P_type)/sizeof(byte));  
    *_tmp++ = 'V';
    _tmp = fill_byte(_tmp,&(_R.velocity),sizeof(V_type)/sizeof(byte));    
    *_tmp++ = 'A';
    _tmp = fill_byte(_tmp,&(_R.xlr8tion),sizeof(A_type)/sizeof(byte));
    
    *_tmp++ = '\0';
    
    // Serial.write(buf, MSG_SIZE);
    return;
} 


int main()
{
    data _d1;
    _d1.position = 0x1337;
    _d1.velocity = 0x3030;
    _d1.xlr8tion = 0xF00F;
    
    data _d2;
    _d2.position = 0xABCD;
    _d2.velocity = 0x1234;
    _d2.xlr8tion = 0x0FF0;
    
    send_data(_d1,_d2);
    
    for(size_t i =0; i< MSG_SIZE; ++i){ printf("\n%02X : %c",buf[i] ,buf[i]); }

    
    return 0;
}

