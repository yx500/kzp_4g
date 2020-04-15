#pragma once

#include <string>

struct Options
{
  int parse_args (int argc, char *argv[]);

  std::string config_file {"kzp4g.conf"};
  std::string local_address {"0.0.0.0"};
  int wr_timeout {100};
  std::string tsname_address  {"kzp_addr"};
  std::string tsname_value   {"kzp_val"};
};

#include "vsingleton.h"
#define Opt Vx::Singleton<Options>::instance()



