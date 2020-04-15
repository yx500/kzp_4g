#include "opt.h"

#include <boost/program_options.hpp>
#include <iostream>

using namespace std;
namespace po = boost::program_options;

int Options::parse_args (int argc, char *argv[])
{
    po::options_description opts("Options");
    opts.add_options()
            ("help,h", "produce help message")
//            ("config-file,c", po::value< std::string >(&config_file), "kzp configuration")
            ("local-address,l", po::value< std::string >(&local_address), "interface to bind gtsocket")
            ("wr_timeout,p", po::value< int >(&wr_timeout), "write->read timeout milliseconds")
            ("tsname_address,a", po::value< std::string >(&tsname_address), "ts name for address")
            ("tsname_value,v", po::value< std::string >(&tsname_value), "ts name for value")
            ;

    po::positional_options_description p;
    p.add("config-file", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(opts).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help") /*|| argc==1*/ ) {
        std::cout << opts << "\n";
        return 0;
    }

    return 1;
}

