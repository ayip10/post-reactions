
#include "Tag.h"

Tag::Tag(int arg_index, std::string arg_avatar_name, std::string arg_vsID, std::string arg_comment)
{
  this->index = arg_index;
  this->avatar_name = arg_avatar_name;
  this->vsID = arg_vsID;
  this->comment = arg_comment;
}

Json::Value *Tag::dumpJ
(void)
{
    Json::Value *result_ptr = new Json::Value();

    if (this->avatar_name != "")
    {
    (*result_ptr)["avatar_name"] = this->avatar_name;
    }
    if ((int)this->index != 0)
    {
    (*result_ptr)["index"] = (int)this->index;
    }
    if (this->vsID != "")
    {
    (*result_ptr)["vsID"] = this->vsID;
    }
    if (this->comment != "")
    {
    (*result_ptr)["comment"] = this->comment;
    }
#ifdef _ECS36B_DEBUG_
  std::cout << result_ptr->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */
  return result_ptr;
}