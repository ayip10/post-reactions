
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

// Message.h
#include "Core.h"

class Message
{
 private:
 protected:
 public:
  std::string content;
  Message(std::string arg_s) { content = arg_s; }
  void setContent(std::string arg_s) { content = arg_s; }
  std::string getContent(void) { return content; }
};

#endif /* _MESSAGE_H_ */
