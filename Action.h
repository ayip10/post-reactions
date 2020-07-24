
#ifndef _ACTION_H_
#define _ACTION_H_

#include "Core.h"

class Action : public Core
{
 private:
 protected:
 public:
  std::string name;
  Action(std::string, std::string, std::string, std::string, std::string);
  Json::Value * dumpJ();
};

#endif /* _ACTION_H_ */
