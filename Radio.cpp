#include "Radio.h"

Radio::Radio(RF24 * rf24, net_id_t me)
  :m_id(me), m_radio(rf24)
{

}

void Radio::begin()
{
  // call the RF24's begin function
  m_radio->begin();

  // start powered off
  m_radio->powerDown();
}

bool Radio::sendTo(net_id_t who, radio_pkt * pkt)
{
  pkt->id = who;

  m_radio->stopListening();
  m_radio->openWritingPipe(NET_PREFIX | who);

  // write the packet with a plus 1 for size (the id)
  return m_radio->write(pkt->data, pkt->size+1);
}

void Radio::recv(radio_pkt * pkt)
{

}
