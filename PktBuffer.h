#ifndef PKTBUFFER_H
#define PKTBUFFER_H

#include <stdint.h>
#include <util/atomic.h>
#include <string.h>

struct radio_pkt;

template <int T>
class PktBuffer
{
public:
  PktBuffer()
    :m_head(0), m_tail(0), m_available(0)
  {
    // for good measure
    memset(m_pktBuf, 0, sizeof(m_pktBuf));
  }
  bool available() { return m_available > 0; }
  bool full() { return m_available == T; }

  void clear() {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      m_available = 0;
      m_tail = m_head = 0;
    }
  }

  bool empty() { return m_available == 0; }
  bool get(radio_pkt * pkt) {
    if(!pkt)
      return false;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      if(empty())
        return false;

      memcpy(pkt, &m_pktBuf[m_head], sizeof(*pkt));
      m_head = (m_head + 1) % T;
      m_available--;
    }

    return true;
  }

  bool put(radio_pkt * pkt) {
    if(!pkt)
      return false;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      if(full())
        return false;

      memcpy(&m_pktBuf[m_tail], pkt, sizeof(*m_pktBuf));
      m_tail = (m_tail + 1) % T;
      m_available++;
    }

    return true;
  }
private:
  radio_pkt m_pktBuf[T];
  uint8_t m_head, m_tail;
  uint8_t m_available;
};

#endif // !PKTBUFFER_H
