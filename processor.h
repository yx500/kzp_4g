#pragma once

#include <boost/asio.hpp>
#include <atomic>
#include "asio_obj.h"

namespace aio = boost::asio;

class processor: asio_obj
{
  aio::steady_timer   timer_;

  void take_addr(uint8_t adr);
  void read_and_send();
  void process_step();
  void open();
  void close();

public:
  int  addr;
  std::atomic_uint8_t  addr_by_ts;
  std::atomic_uint8_t  value_by_ts;

  processor(aio::io_context& c): asio_obj(c), timer_(c) { open(); }
  ~processor() { close(); }

  void begin_step();

};

