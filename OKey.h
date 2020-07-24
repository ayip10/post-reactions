
#ifndef _OKEY_H_
#define _OKEY_H_

// OKey.h
#include "Core.h"
#include "Post.h"

class Post;

class OKey
{
 private:
 protected:
 public:
  std::string okey;
  std::vector<Post *> * posts;
  OKey(std::string arg_okey) { okey = arg_okey; }
};

#endif /* _OKEY_H_ */
