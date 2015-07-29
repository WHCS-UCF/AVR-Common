#include "Radio.h"

#include <avr/pgmspace.h>
#include <stdio.h>

#include "pinout.h"

Radio::Radio(RF24 * rf24, net_id_t me)
  :m_id(me), m_radio(rf24), m_listening(false)
{

}

void Radio::begin()
{
  // call the RF24's begin function
  m_radio->begin();

  // start powered off
  m_radio->powerUp();
  
  m_radio->openReadingPipe(0, 0xabc127ffdeLL);
  m_radio->openReadingPipe(1, NET_PREFIX | m_id);
  //m_radio->enableDynamicPayloads();
  //m_radio->setPayloadSize(MAX_PACKET_SIZE);
  startListening();
}

void Radio::enableInterrupt()
{
#if defined(__AVR_ATmega328P__)
  EICRA |= _BV(ISC11);
  EICRA &= ~_BV(ISC10);

  PIN_MODE_INPUT(NRF_IRQ);
  PIN_HIGH(NRF_IRQ); //Enable the pull-up resistor

  EIMSK |= _BV(INT1);
#elif defined(__AVR_ATmega32__)
  MCUCR |= _BV(ISC11);
  MCUCR &= ~_BV(ISC10);

  PIN_MODE_INPUT(NRF_IRQ);
  PIN_HIGH(NRF_IRQ); //Enable the pull-up resistor

  GICR |= _BV(INT0);
#endif
}

void Radio::disableInterrupt()
{
#if defined(__AVR_ATmega328P__)
  EICRA &= ~_BV(ISC11);
  EICRA &= ~_BV(ISC10);

  PIN_MODE_INPUT(NRF_IRQ);
  PIN_HIGH(NRF_IRQ); //Enable the pull-up resistor

  EIMSK &= _BV(INT1);
#elif defined(__AVR_ATmega32__)
  MCUCR &= ~_BV(ISC11);
  MCUCR &= ~_BV(ISC10);

  PIN_MODE_INPUT(NRF_IRQ);
  PIN_HIGH(NRF_IRQ); //Enable the pull-up resistor

  GICR &= _BV(INT0);
#endif
}

bool Radio::available()
{
  return m_pktBuffer.available();
}

bool Radio::queuePacket(radio_pkt * pkt)
{
  return m_pktBuffer.put(pkt);
}

bool Radio::sendTo(net_id_t who, opcode_t opcode, uint8_t * buf, uint8_t size)
{
  radio_pkt pkt;
  pkt.from = m_id; // from us. duh
  pkt.opcode = opcode;

  if(size > PAYLOAD_SIZE)
  {
    printf_P(PSTR("Radio::sendTo pkt exceeded max payload\n"));
    return false;
  }

  pkt.size = size;

  if(pkt.size)
    memcpy(pkt.data, buf, size);

  stopListening();
    m_radio->openWritingPipe(NET_PREFIX | who);

    printf("Radio::sendTo ");
    Radio::dump(&pkt);

    uint8_t outSize = 0;
    if(pkt.size > 0)
      outSize = pkt.size+PACKET_HEADER_SIZE;
    else
      outSize = PACKET_HEADER_SIZE;

    //printf("Outsize %hu\n", outSize);

    // write the packet with a plus 1 for size (the id)
    bool result = m_radio->write(&pkt, outSize);
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
    Radio::dump(pkt);
  }
  else
  {
    printf("Not availble\n");
  }
}

void Radio::stopListening()
{
  /*if(!m_listening)
    return;*/

  m_radio->stopListening();
  m_listening = false;
}

void Radio::startListening()
{
  /*if(m_listening)
    return;*/

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

  printf_P(PSTR("Pkt(from %hu, op %02x)"), pkt->from, pkt->opcode);

  if(!pkt->size)
  {
    printf_P(PSTR("\n"));
    return;
  }
  else
    printf_P(PSTR(" payload sz %hu ["), pkt->size);

  for(int i = 0; i < pkt->size; i++)
    if(i+1 == pkt->size)
      printf_P(PSTR("%02x]\n"), pkt->data[i]);
    else
      printf_P(PSTR("%02x "), pkt->data[i]);
}
