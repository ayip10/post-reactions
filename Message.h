
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

// Message.h
#include "JvTime.h"

class Message
{
 private:
 protected:
 public:
  std::string content;
  // JvTime *revision_time;
  // Message(std::string arg_s) { this->content = arg_s; this->revision_time = NULL; }
  Message(std::string arg_s) { this->content = arg_s; }
  void setContent(std::string arg_s) { this->content = arg_s; }
  std::string getContent(void) { return this->content; }
#ifdef _WITH_REVISIONS_
  virtual Json::Value * dumpJ(void) {
    // only called for the revisions
    Json::Value *result_ptr = new Json::Value();
    (*result_ptr)["message"] = this->content;
    if (this->revision_time != NULL)
      {
	std::string * str_ptr =  (this->revision_time)->getTimeString();
	(*result_ptr)["revision_time"] = *str_ptr;
	delete str_ptr;
      }
    return result_ptr;
  }
#endif /* _WITH_REVISIONS_ */
};

#endif /* _MESSAGE_H_ */
