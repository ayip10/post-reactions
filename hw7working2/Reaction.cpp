
#include "Reaction.h"

Reaction::Reaction
(std::string arg_type, Person * arg_author, JvTime * arg_created)
{
  this->type = arg_type;
  this->author = arg_author;
  this->created = arg_created;
}

Json::Value * Reaction::dumpJ
(void)
{
  Json::Value * result_ptr = (this->author)->dumpJ();
  (*result_ptr)["type"] = this->type;
  (*result_ptr)["created_time"] = *((this->created)->getTimeString());
#ifdef _ECS36B_DEBUG_
  std::cout << result_ptr->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */
  return result_ptr;
}
