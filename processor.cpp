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



void send_kzp(const TDatagram2& kzp)
{
  gtlan_send( kzp );
  std::cout <<"-> "<< kzp.Name << std::endl;
}


void processor::open()
{
  gtlan_buf(1, Opt.tsname_address.c_str(), [this](TDatagramPacket2* pck2){ this->kzp_addr.store(pck2->Dtgrm.Data[0]); } );
  gtlan_buf(1, Opt.tsname_value.c_str(), [this](TDatagramPacket2* pck2){ this->kzp_value.store(pck2->Dtgrm.Data[0]); } );
}

void processor::close()
{
  gtlan_buf(1, Opt.tsname_address.c_str(), nullptr);
  gtlan_buf(1, Opt.tsname_value.c_str(), nullptr);
}

void processor::process_polling()
{
  static unsigned int tick=0;


  uint8_t addr = ++tick % 4;

  boost::asio::io_service::strand strand(this->io_context());


  //-------------------------------------------------------
  std::chrono::milliseconds polling_timeout;
  polling_timeout = std::chrono::milliseconds(Opt.wr_timeout);
  polling_timer_.expires_after(polling_timeout);
  polling_timer_.async_wait( [this](const std::error_code& ec)
                            { if(!ec) this->process_polling(); }
                            );
}



