
#ifndef _CORE_H_
#define _CORE_H_

// Core.h

// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>

// popular ones
#include <strings.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <exception> 
#include <time.h>

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
