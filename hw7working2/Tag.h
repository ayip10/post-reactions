
#ifndef _TAG_H_
#define _TAG_H_

// Tag.h
#include "Core.h"

class Tag
{
 private:
 protected:
 public:
  int index;
  std::string avatar_name;
  std::string vsID;
  std::string comment;

  Tag(int arg_index, std::string arg_avatar_name, std::string arg_vsID, std::string arg_comment);
  Json::Value * dumpJ();
};

#endif /* _TAG_H_ */
