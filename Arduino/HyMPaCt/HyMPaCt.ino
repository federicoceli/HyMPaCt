/**** Variable definition ****/
// Define Serial Protocol
#define SYNC_0    0b10110100
#define SYNC_1    0b11110000

// Define message type
#define HYMPACT   0b00001001

#define PKTL    41
#define CRC16   0x8005

unsigned int    seq_number[9];
unsigned char   packet[PKTL];
int             dummyArray[8] = { 24, 20, 200, -300, -5, 31, 32, -56 },
                tempArray[8];

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
uint16_t calculateCRC(const uint8_t *data, uint16_t size)
{
  uint16_t out = 0;
  int bits_read = 0, bit_flag;

  /* Sanity check: */
  if (data == NULL)
    return 0;

  while (size > 0)
  {
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

/**** Generates a randomly populated array ****/
void rndArray(int* dummyArray, int lenght, int min_v, int max_v) {
  for (int i = 0; i < lenght; i++) {
    dummyArray[i] = random(min_v, max_v);
  }
}

/**** Assembles a full packet of given type and value(s) ****/
void pktAssemble(unsigned char* packet, int type, int value[]) {
  uint16_t checkSum = 0;
  uint16_t crc = 0;
  
  // synchronization word   [0-1]
  packet[0] = SYNC_0;
  packet[1] = SYNC_1;

  // message type       [2]
  packet[2] = type;

  // sequence         [3-4]
  packet[3] = (char)((seq_number[type]) >> 8);  
  packet[4] = (char)(seq_number[type]++);

  // data (8 sets of 4 byte)  [5-36]
  if (type == HYMPACT) {
    for (int set = 0; set < 8; set++)
      for (int byte = 0; byte < 4; byte++)
        packet[5 + set * 4 + byte] = (char)((value[set] >> 8 * (3 - byte)));
  }

  // data (16 sets of 2 byte) [5-36]
  else {
    for (int set = 0; set < 16; set++)
      for (int byte = 0; byte < 2; byte++)
        packet[5 + set * 2 + byte] = (char)((value[set] >> 8 * (1 - byte)));
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
  Serial.begin(9600); 
}

void loop() {
  rndArray(tempArray, 8, 20, 30);
  pktAssemble(packet, HYMPACT, tempArray);
  //Serial.write(packet, PKTL);
  for( int n = 0; n < PKTL; n++ )
  {
    Serial.write(packet[n]);
  }
  
  delay(50);
}