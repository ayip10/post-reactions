
#ifndef _REACTION_H_
#define _REACTION_H_

// Reaction.h
#include "Core.h"
#include "Person.h"
#include "JvTime.h"

class Reaction
{
 private:
 protected:
 public:
  std::string type;
  Person * author;
  JvTime * created;
  Reaction(std::string arg_type, Person * arg_author, JvTime * arg_created);
  Json::Value * dumpJ();
};

#endif /* _REACTION_H_ */
