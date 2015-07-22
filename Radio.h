#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>
#include <RF24.h>

//#define NET_PREFIX 0xDEADBEEF00ULL
#define NET_PREFIX 0xE8E8F0F000ULL

// Specifies a station ID
// Used for pipes and internally for message structures
typedef uint8_t net_id_t;

struct radio_pkt
{
  uint8_t size;
  net_id_t id;
  uint8_t data[31];
};

class Radio
{
  public:
    Radio(RF24 * rf24, net_id_t me);
    void begin();

    bool sendTo(net_id_t who, radio_pkt * pkt);
    void recv(radio_pkt * pkt);
  private:
    RF24 * m_radio;
    net_id_t m_id;
};

#endif // RADIO_H
