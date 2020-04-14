#include <iostream>
#include <iomanip>
#include <string>

#include <boost/asio.hpp>

#include "opt.h"
#include "boost_helpers.h"
#include "asio_pch.h"
#include "processor.h"


//using namespace std;
using namespace boost::asio;

int main(int argc, char *argv[])
{
  try {
        if( Opt.parse_args(argc, argv) <= 0 )
            return 0;

        asio_lib::Executor e;
        e.OnWorkerThreadError = [](io_context&, boost::system::error_code ec) {
            std::cerr<<"\n!!! error (asio): "<<ec.message();
        };
        e.OnWorkerThreadException = [](io_context&, const std::exception &ex) {
            std::cerr<<"\n!!! exception (asio): "<<ex.what();
        };
        e.AddCtrlCHandling();

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        processor pro( e.GetIOContext() );
        pro.open();
        e.GetIOContext().post( [&pro]() { pro.process_polling(); } );
        e.GetIOContext().post( [&pro]() { pro.process_broadcast(); } );
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        e.Run(1);
        pro.close();
    }
    catch (std::exception& e) {
        std::cerr<<"\nexception: "<<e.what()<<"\n";
    }
    return 0;
}
//------------------------------------------------------------------------------
