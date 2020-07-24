
#ifndef _COMMENT_H_
#define _COMMENT_H_

// Comment.h
#include "Core.h"
#include "Tag.h"
#include "Reaction.h"
#include "Message.h"
#include "Person.h"
#include "JvTime.h"

class Comment
{
 private:
 protected:
 public:
  std::string profile_id;
  std::string post_id;
  std::string comment_id;

  Person * author;
  Message * msg;
  JvTime * created;

  std::vector<Reaction *> * reactions;
  std::vector<Tag *> * tags;

  Comment(std::string arg_profile_id, std::string arg_post_id, std::string arg_comment_id,
	  Person * arg_author, Message * arg_msg, JvTime * arg_created);
  Json::Value * dumpJ();
};

#endif /* _COMMENT_H_ */
