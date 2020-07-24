
#include "Action.h"
#include "Core.h"

Action::Action(std::string arg_name, std::string arg_host_url, std::string arg_vsID, std::string arg_class_id, std::string arg_object_id)
{
  this->host_url = arg_host_url;
  this->owner_vsID = arg_vsID;
  this->class_id = arg_class_id;
  this->object_id = arg_object_id;  
  this->name = arg_name; 
}

Json::Value * Action::dumpJ
(void)
{
    Json::Value *result_ptr = new Json::Value();

    (*result_ptr)["class_id"] = this->class_id;
    (*result_ptr)["host_url"] = this->host_url;
    (*result_ptr)["name"] = this->name;
    (*result_ptr)["object_id"] = this->object_id;
    (*result_ptr)["owner_vsID"] = this->owner_vsID;

#ifdef _ECS36B_DEBUG_
  std::cout << result_ptr->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */
  return result_ptr;
}