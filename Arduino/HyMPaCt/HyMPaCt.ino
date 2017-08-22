#include <SPI.h>
#include <MAX31865.h>
#include <Adafruit_MAX31865.h>

/**** Variables definition ****/
// Define Serial Protocol
#define SYNC_0    0b10110100
#define SYNC_1    0b11110000

// Define message type
#define HYMPACT   0b00001001

#define PKTL      41
#define CRC16     0x8005

// Define digital pins for RTD-to-digital sensors
#define RTD_CS_PIN1   41
#define RTD_CS_PIN2   43
#define RTD_CS_PIN3   45

// Reference resistance in Ohm
#define RREF          430.0

// Define analog pins for Accelerometer
#define ACC_X       0
#define ACC_Y       1
#define ACC_Z       2

// Declare temperature sensor in MAX31865 class
MAX31865_RTD temperature_sensor[3] = {MAX31865_RTD(MAX31865_RTD::RTD_PT100, RTD_CS_PIN1, RREF),
                                      MAX31865_RTD(MAX31865_RTD::RTD_PT100, RTD_CS_PIN2, RREF),
                                      MAX31865_RTD(MAX31865_RTD::RTD_PT100, RTD_CS_PIN3, RREF)};

// The sensor can meausre up to \pm 200 g
int         scale_acc = 200;

bool        init_acc = false;

// If a device is found not to be online, try and reconnect
bool        auto_reconnect = false;

unsigned int    seq_number[10];
unsigned char   packet[PKTL];
int             dummyArray[16] = { 24, 20, 200, -300, -5, 31, 32, -56 },
                tempArray[16];

/**** Checksum calculation ****/
uint16_t calculateCheckSum(unsigned char *buf, unsigned int n) {
  uint16_t checksum = 0;
  while (n>0) {
    checksum += *buf++;
    n--;
  }
  checksum = ((~checksum) + 1); 
  //cout << "Checksum: " << std::bitset<16>(checksum) << ", " << (int)checksum << "\n";
  return checksum;
}

/**** CRC calculation ****/
uint16_t calculateCRC(const uint8_t *data, uint16_t size){
    uint16_t out = 0;
    int bits_read = 0, bit_flag;

    /* Sanity check: */
    if (data == NULL)
        return 0;

    while (size > 0) {
        bit_flag = out >> 15;

        /* Get next bit: */
        out <<= 1;
        out |= (*data >> (7 - bits_read)) & 1;

        /* Increment bit counter: */
        bits_read++;
        if (bits_read > 7)
        {
            bits_read = 0;
            data++;
            size--;
        }
            
        /* Cycle check: */
        if (bit_flag) {
            out ^= CRC16;
        }
    }
    return out;
}

/**** Reads temperature from RTD ****/
int readTemprature(MAX31865_RTD* rtd, int index) {
    float ret = 0;
    Serial.println("OK");
    
    rtd[index].read_all();
    
    Serial.print("Status: "); Serial.println(rtd[index].status());
        
    if (rtd[index].status() == 0){
        ret = rtd[index].temperature()*100;
    }
    else {
        // Error
        ret = -2;
    }

    Serial.print("Value: "); Serial.println(ret/100);
    return ret;
}

/**** Read analog value "as is" ****/
int readAcc(int analogPin) {
    int raw = analogRead(analogPin);
    return raw;
}

/**** Conncts to an RTD-to-digital interface with PT100 ****/
bool connectRTD(MAX31865_RTD* rtd, int index){
    /* Configure:
        V_BIAS enabled
        Auto-conversion
        1-shot disabled
        3-wire enabled
        Fault detection:  automatic delay
        Fault status:  auto-clear
        50 Hz filter
        Low threshold:  0x0000
        High threshold:  0x7fff
    */
    rtd[index].configure(true, true, false, false, MAX31865_FAULT_DETECTION_NONE,
        true, true, 0x0000, 0x7fff);
}

/**** Generates a randomly populated array ****/
void rndArray(int* dummyArray, int lenght, int min_v, int max_v) {
    // Random preventive population
    for (int i = 0; i < lenght; i++) {
        dummyArray[i] = random(min_v, max_v)*100;
    }
    
    // Populate with data I actually have
    dummyArray[0] = readTemprature(temperature_sensor, 0);

    dummyArray[1] = readTemprature(temperature_sensor, 1);

    dummyArray[2] = readTemprature(temperature_sensor, 2);

    // Acceleration, X-Axis
    dummyArray[5] = readAcc(ACC_X);
    // Acceleration, Y-Axis
    dummyArray[6] = readAcc(ACC_Y);
    // Acceleration, Z-Axis
    dummyArray[7] = readAcc(ACC_Z);
}

/**** Assembles a full packet of given type and value(s) ****/
void pktAssemble(unsigned char* packet, int type, int value[]) {
    uint16_t  checkSum = 0;
    uint16_t  crc = 0;
    
    // synchronization word   [0-1]
    packet[0] = SYNC_0;
    packet[1] = SYNC_1;

    // message type       [2]
    packet[2] = type;

    // sequence         [3-4]
    packet[3] = (char)((seq_number[type]) >> 8);  
    packet[4] = (char)(seq_number[type]++);

    // data (16 sets of 2 byte) [5-36]
    if (type == HYMPACT) {
        for (int set = 0; set < 16; set++)
            for (int byte = 0; byte < 2; byte++)
            packet[5 + set * 2 + byte] = (char)((value[set] >> 8 * (1 - byte)));
    }

    // data (16 sets of 2 byte) [5-36]
    else {
        for (int set = 0; set < 8; set++)
            for (int byte = 0; byte < 4; byte++)
            packet[5 + set * 4 + byte] = (char)((value[set] >> 8 * (3 - byte)));
    }

    // checksum         [37-38]
    checkSum = calculateCheckSum(packet, PKTL - 4);
    packet[37] = (char)((checkSum) >> 8);
    packet[38] = (char)(checkSum);

    // crc            [39-40]
    crc = calculateCRC(packet, PKTL - 2);
    packet[39] = (char)((crc) >> 8);
    packet[40] = (char)(crc);
    //cout << "CRC = " << crc << "; [39] = " << (int)packet[39] << ", [40] = " << (int)packet[40] << "\n";
}

void setup() {
    Serial.begin(115200);

    // Set ADC to 12 bit
    analogReadResolution(12);

    /* Initialize SPI communication. */
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    SPI.setDataMode(SPI_MODE3);
    
    /* Allow the MAX31865 to warm up. */
    delay(100);
    
    // Connect RTD to read temperature data from PT-100
    connectRTD(temperature_sensor, 0);
    connectRTD(temperature_sensor, 1);
    connectRTD(temperature_sensor, 2);
    
    // Connect to Accelerometer
 }

void loop() {
    rndArray(tempArray, 16, 20, 30);
    pktAssemble(packet, HYMPACT, tempArray);

    for( int n = 0; n < PKTL; n++ ) {
        Serial.write(packet[n]);
    }

    delay(100);
}