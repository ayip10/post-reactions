
#ifndef _CORE_H_
#define _CORE_H_

// Core.h

// for Json::value
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/value.h>

// popular ones
#include <strings.h>
#include <string.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <exception> 
#include <time.h>

// c library
#include <arpa/inet.h>

using namespace std;

class Core
{
 private:
 public:
  static unsigned int core_count;
  std::string host_url;
  std::string owner_vsID;
  std::string class_id;
  std::string object_id;
  Core(void);
  Core(std::string, std::string, std::string, std::string);
};

#endif  /* _CORE_H_ */
