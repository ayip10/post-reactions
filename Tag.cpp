#include "Tag.h"

Tag::Tag()
{
}

Json::Value * Tag::dumpJ
(void)
{
    Json::Value *result_ptr = new Json::Value();
    if (this->avatar_name!= "")
    {
       (*result_ptr)["avatar_name"] = this->avatar_name; 
    }
    if (this->vsID != "")
    {
        (*result_ptr)["vsID"] = this->vsID;
    }
    if (this->comment != "")
    {
        (*result_ptr)["comment"] = this->comment;
    }
    if (this->index != 0)
    {
        (*result_ptr)["index"] = this->index;
    }
    
    #ifdef _ECS36B_DEBUG_
        std::cout << result_ptr->toStyledString() << std::endl;
    #endif /* _ECS36B_DEBUG_ */
        return result_ptr;
}