#pragma once

#include <boost/asio.hpp>
#include "asio_obj.h"

namespace aio = boost::asio;

class processor: asio_obj
{
  aio::steady_timer polling_timer_;
  aio::steady_timer broadcast_timer_;

public:

  processor(aio::io_context& c): asio_obj(c)
                                  , polling_timer_(c)
                                  , broadcast_timer_(c){
  }

  void open();
  void close();

  void process_polling();
  void process_broadcast();


};

