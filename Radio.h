#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>
#include <RF24.h>
#include "PktBuffer.h"

//#define NET_PREFIX 0xDEADBEEF00ULL
#define NET_PREFIX 0xE8E8F0F000ULL

// Specifies a station ID
// Used for pipes and internally for message structures
typedef uint8_t net_id_t;
typedef uint8_t opcode_t;

#define MAX_PACKET_SIZE 10
#define PACKET_HEADER_SIZE 3
#define PAYLOAD_SIZE MAX_PACKET_SIZE - PACKET_HEADER_SIZE 
#define MIN_PACKET_SIZE PACKET_HEADER_SIZE

#if PAYLOAD_SIZE < 0
  #error "Exceeded the max packet size
#endif

// Radio opcodes
#define RADIO_OP_SET_STATUS 0x1
#define RADIO_OP_GET_STATUS 0x2
#define RADIO_OP_RESTART 0x3
#define RADIO_OP_PING 0x4
#define RADIO_OP_PONG 0x5

#define RADIO_OP_SUCCESS 0x80
#define RADIO_OP_FAILURE 0x81
#define RADIO_OP_SUCCESS_RESULT 0x82
#define RADIO_OP_FAILURE_RESULT 0x83

struct radio_pkt
{
  net_id_t from;
  opcode_t opcode;
  uint8_t size; // optional: size of the below data
  uint8_t data[PAYLOAD_SIZE];
};

class Radio
{
  public:
    Radio(RF24 * rf24, net_id_t me);
    void begin();
    void enableInterrupt();
    void disableInterrupt();

    // check the PktRingBuffer
    bool available();
    bool queuePacket(radio_pkt * pkt);

    bool sendTo(net_id_t who, opcode_t opcode, uint8_t * buf, uint8_t size);
    void recv(radio_pkt * pkt);
    static void dump(radio_pkt * pkt);
  private:
    void stopListening();
    void startListening();
    bool isListening();

    PktBuffer<3> m_pktBuffer;
    RF24 * m_radio;
    net_id_t m_id;
    bool m_listening;
};

#endif // RADIO_H
