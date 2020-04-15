#include "processor.h"

#include <string>
#include <chrono>
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <algorithm>

#include "opt.h"
#include "vsingleton.h"
#include "gtlan.h"
#include <boost/tokenizer.hpp>
// dsvsfgdfg


//хрень

void processor::open()
{

  gtlan_callback_buffun f = [this](TDatagramPacket2* )-> void {};

  gtlan_buf(1, Opt.tsname_address.c_str(),  &f);
  gtlan_buf(1, Opt.tsname_value.c_str(), &f);
}

void processor::close()
{
  gtlan_setcallback(nullptr);
}

void processor::process_polling()
{


  //-------------------------------------------------------
  std::chrono::milliseconds polling_timeout;
  polling_timeout = std::chrono::milliseconds(Opt.wr_timeout);
  polling_timer_.expires_after(polling_timeout);
  polling_timer_.async_wait( [this](const std::error_code& ec)
                            { if(!ec) this->process_polling(); }
                            );
}


void send_kzp(const TDatagram2& kzp)
{
  gtlan_send( kzp );
  std::cout <<"-> "<< kzp.Name << std::endl;
}

