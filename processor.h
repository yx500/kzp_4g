#pragma once

#include <boost/asio.hpp>
#include <atomic>
#include "asio_obj.h"

namespace aio = boost::asio;

class processor: asio_obj
{
  aio::steady_timer polling_timer_;


public:
  std::atomic_int kzp_addr;
  std::atomic_int kzp_value;


  processor(aio::io_context& c): asio_obj(c)
                                  , polling_timer_(c)
  {
  }

  void open();
  void close();
  void process_polling();

};

