#pragma once

#include <string>

struct Options
{
  int parse_args (int argc, char *argv[]);

  std::string config_file {"dio.conf"};
  std::string local_address {"0.0.0.0"};
  int polling_timeout {100};
  int broadcast_timeout {3000};
};

#include "vsingleton.h"
#define Opt Vx::Singleton<Options>::instance()



