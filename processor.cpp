#include "processor.h"

#include <string>
#include <chrono>
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <algorithm>

#include <boost/tokenizer.hpp>
#include <boost/bind/bind.hpp>
#include "opt.h"
#include "vsingleton.h"
#include "gtlan.h"
#include "messageDO.h"
#include "tKzp2.h"
#include "helpers.h"


void processor::open()
{
    gtlan_buf(1, Opt.tsname_address.c_str(), [this](TDatagramPacket2* pck2) {
        this->addr_by_ts.store(pck2->Dtgrm.Data[1]);
    }
              );
    gtlan_buf(1, Opt.tsname_value.c_str(), [this](TDatagramPacket2* pck2) {
        this->value_by_ts.store(pck2->Dtgrm.Data[1]);
    }
              );
}

void processor::close()
{
    gtlan_buf(1, Opt.tsname_address.c_str(), nullptr);
    gtlan_buf(1, Opt.tsname_value.c_str(), nullptr);
}


void processor::begin_step()
{
    std::chrono::milliseconds timeout(1); //todo maybe
    timer_.expires_after(timeout);
    timer_.async_wait( [this](const std::error_code& ec)
                      { if(!ec) this->process_step(); }
                      );

    std::cout<<" "<<__func__;
}


void processor::process_step()
{
    static unsigned int tick=0;
    std::cout<<std::endl<<"---"<<tick;
    take_addr( (++tick % 8)+1 );
}


void processor::take_addr(uint8_t adr)
{
    this->addr = adr;
    message_DO m{};
    m.flag=1;
    m.number=0;
    m.data[0]=adr;
    m.commit();

    TDatagram2 qry;
    qry.setName(Opt.tsname_address);
    qry.Type=0;
    qry.setData(m.begin(), m.size());
    gtlan_send( qry );

    std::chrono::milliseconds timeout(Opt.wr_timeout);
    timer_.expires_after(timeout);
    timer_.async_wait( [this](const std::error_code& ec)
                      { if(!ec) this->read_and_send(); begin_step(); }
                      );

    std::cout<<" "<<__func__<<addr;
}

/*
int suka_kode(uint8_t val){
  int res;
  switch(val){
  default: res=0;
  case 0b001: res=50;
  case 0b010: res=100;
  case 0b011: res=150;
  case 0b100: res=200;
  case 0b101: res=250;
  case 0b110: res=300;
  case 0b111: res=350;
  }
  return res;
}
*/

int marshrut(uint8_t addr)
{
    int res=addr & 0xf;
    return res;
}


void processor::read_and_send()
{
    int a = this->addr_by_ts.exchange(0);
    int v = this->value_by_ts.exchange(0);

    //>>todo плата out != in
    a=this->addr;

    t_KvKzp kzp{};
    kzp.mar = marshrut(a);
    kzp.val = v*50;//suka_kode(v);
    kzp.tim = Vx::timestamp();
    kzp.addr = this->addr;
    kzp.val_ks = v;

    TDatagram2 res;
    res.setName( IpxNameKZP+std::to_string(kzp.mar) );
    res.Type=IpxTypeKZP;
    res.setData( &kzp, sizeof(kzp) );
    gtlan_send( res );

    std::cout<<" "<<__func__<<this->addr<<">"<<a<<":"<<v;
}





