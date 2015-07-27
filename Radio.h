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

#define MAX_PACKET_SIZE 10
#define PACKET_HEADER_SIZE 1

#if MAX_PACKET_SIZE - PACKET_HEADER_SIZE < 0
  #error "Exceeded the max packet size
#endif

struct radio_pkt
{
  uint8_t size;
  net_id_t id;
  uint8_t data[MAX_PACKET_SIZE-PACKET_HEADER_SIZE];
};

class Radio
{
  public:
    Radio(RF24 * rf24, net_id_t me);
    void begin();
    // check the PktRingBuffer
    bool available();
    bool queuePacket(radio_pkt * pkt);

    bool sendTo(net_id_t who, radio_pkt * pkt);
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
