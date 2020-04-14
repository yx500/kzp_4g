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
#include "device.h"
#include "do_message.hpp"
#include <boost/tokenizer.hpp>


struct Tasks{

  struct Job{
    std::string dev;
    bool  is_do;
    int   port_start;
    int   port_count;
    std::string dtg_name;
    int   buf_offset;
    std::string profile;
  };

      std::deque<Job> tasks;
  std::chrono::milliseconds polling_timeout;
  std::chrono::milliseconds broadcast_timeout;


      void load_configuration(){
    polling_timeout = std::chrono::milliseconds(Opt.polling_timeout);
    broadcast_timeout = std::chrono::milliseconds(Opt.broadcast_timeout);

        std::ifstream in(Opt.config_file);
    if (in.is_open()){
      std::string line;
      while (getline(ws(in),line)) {
        if(line.size() && line[0]=='#')
          continue;
        using Tokenizer = boost::tokenizer< boost::char_separator<char> >;
        boost::char_separator<char> sep("|");
        Tokenizer tok(line, sep);
        std::vector< std::string > strv( tok.begin(), tok.end() );
        if (strv.size() < 7) continue;
        Job j{
            strv[0],
            std::stoi(strv[1]) ? true : false, // in/out
            std::stoi(strv[2]),                // start port
            std::stoi(strv[3]),                // port count
            strv[4],
            std::stoi(strv[5]),                // offset
            strv[6],
            };
        tasks.emplace_back(j);
        copy(strv.begin(), strv.end(), std::ostream_iterator<std::string>(std::cout, "|"));
        std::cout << std::endl;
      }
    }
  }
};
#define TSK Vx::Singleton<Tasks>::instance()


void work_with_tu(TDatagramPacket2* pck2);
void send_ts(const TDatagram2& dtg);



void processor::open()
{
  TSK.load_configuration();

  for (auto &j : TSK.tasks ){
    TDatagram2& dtg = TS.pack(j.dtg_name);
    dtg.Size = std::max( int(dtg.Size), j.buf_offset+j.port_start+j.port_count+1 );
    dtg.Type=1;
    if(! device::create( j.dev, j.is_do, j.profile ) )
      std::cerr<<"failed "<<j.dev<<std::endl;
  }

  gtlan_buf(0, "*"); //все ту
  gtlan_setcallback(&work_with_tu);
}

void processor::close()
{
  gtlan_setcallback(nullptr);
}

void processor::process_polling()
{
  std::set<TDatagram2*> changed;
  for (auto &j : TSK.tasks ){
    TDatagram2& dtg = TS.pack(j.dtg_name);
    bool ch = device::in( j.dev,
                         j.port_start,
                         j.port_count,
                         &dtg.Data[ 1 + j.buf_offset ] );
    if(ch)
      changed.insert(&dtg);
  }

  for(auto dtg : changed){
    send_ts( *dtg );
    std::cout/*<<"\r"*/<<dtg->Name<<" ";
  }


  //-------------------------------------------------------
  polling_timer_.expires_after(TSK.polling_timeout);
  polling_timer_.async_wait( [this](const std::error_code& ec)
                            { if(!ec) this->process_polling(); }
                            );
}

void processor::process_broadcast()
{
  std::cout<<"+"<<std::endl;

  for (auto &j : TSK.tasks ){
    TDatagram2& dtg = TS.pack(j.dtg_name);
    send_ts( dtg );
  }
  //-------------------------------------------------------
  broadcast_timer_.expires_after(TSK.broadcast_timeout);
  broadcast_timer_.async_wait( [this](const std::error_code& ec)
                              { if(!ec) this->process_broadcast(); }
                              );
}


void work_with_tu(TDatagramPacket2* pck2)
{
  do_message m(pck2->Dtgrm.Data);

  std::string slot(pck2->Dtgrm.Name);
  std::cout <<"DO: "<< slot
            << " n" << m.number
            << " s" << m.on_off
            << " i" << m.id
      ;

  std::string dev;
  auto f = std::find_if( TSK.tasks.begin()
                            , TSK.tasks.end()
                            , [&slot](auto j){ return slot==j.dtg_name; }
                        );

  if(f != TSK.tasks.end())
    dev=f->dev;
  else
    std::cout <<"  bad slot. ";

  if( !dev.empty() && device::out( dev, m.number/8, m.number%8, m.on_off) ){
    std::cout <<"  ok."<<std::endl;
  } else
    std::cout <<"  failed."<<std::endl;
}

void send_ts(const TDatagram2& ts)
{
  gtlan_send( ts );
  // todo perepack to 101 type ?
}

