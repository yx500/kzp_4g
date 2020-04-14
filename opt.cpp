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
            ("config-file,c", po::value< std::string >(&config_file), "dio configuration")
            ("polling_timeout,p", po::value< int >(&polling_timeout), "polling timeout milliseconds")
            ("broadcast_timeout,t", po::value< int >(&broadcast_timeout), "broadcast timeout milliseconds")
            ("local-address,l", po::value< std::string >(&local_address), "interface to bind gtsocket")
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

