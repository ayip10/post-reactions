#ifndef _TAG_H_
#define _TAG_H_

// Tag.h
#include "Core.h"
#include "Person.h"
#include "JvTime.h"



class Tag
{
 private:
 protected:
 public:
  int index;
  Person * author;
  std::string vsID;
  std::string comment;
  std::string avatar_name;
  Tag();
  Tag(int arg_index,Person * arg_author,std::string arg_comment);
  Json::Value * dumpJ();
};

#endif /* _TAG_H_ */

  