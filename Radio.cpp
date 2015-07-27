#include "Radio.h"

#include <avr/pgmspace.h>
#include <stdio.h>

Radio::Radio(RF24 * rf24, net_id_t me)
  :m_id(me), m_radio(rf24), m_listening(false)
{

}

void Radio::begin()
{
  // call the RF24's begin function
  m_radio->begin();

  // start powered off
  //m_radio->powerDown();
  
  m_radio->openReadingPipe(1, NET_PREFIX | m_id);
  startListening();
}

bool Radio::available()
{
  return m_pktBuffer.available();
}

bool Radio::queuePacket(radio_pkt * pkt)
{
  return m_pktBuffer.put(pkt);
}

bool Radio::sendTo(net_id_t who, radio_pkt * pkt)
{
  pkt->id = who;

  stopListening();
    m_radio->openWritingPipe(NET_PREFIX | who);

    printf("Radio::sendTo ");
    dump(pkt);

    // write the packet with a plus 1 for size (the id)
    bool result = m_radio->write(&pkt->id, pkt->size+1);
  startListening();

  return result;
}

void Radio::recv(radio_pkt * pkt)
{
  if(!pkt)
    return;

  if(m_pktBuffer.available())
  {
    m_pktBuffer.get(pkt);

    printf("Radio::recv ");
    dump(pkt);
  }
}

void Radio::stopListening()
{
  if(!m_listening)
    return;

  m_radio->stopListening();
  m_listening = false;
}

void Radio::startListening()
{
  if(m_listening)
    return;

  m_radio->startListening();
  m_listening = true;
}

bool Radio::isListening()
{
  return m_listening;
}

void Radio::dump(radio_pkt * pkt)
{
  if(!pkt)
    return;

  printf_P(PSTR("Pkt(len %hu, id %hu) ["), pkt->size, pkt->id);

  for(int i = 0; i < pkt->size; i++)
    if(i+1 == pkt->size)
      printf_P(PSTR("%02x]\n"), pkt->data[i]);
    else
      printf_P(PSTR("%02x "), pkt->data[i]);
}
