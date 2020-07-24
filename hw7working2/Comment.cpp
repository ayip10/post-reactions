
#include "Comment.h"

Comment::Comment
(std::string arg_profile_id, std::string arg_post_id, std::string arg_comment_id,
 Person * arg_author, Message * arg_msg, JvTime * arg_created)
{
  this->profile_id = arg_profile_id;
  this->post_id = arg_post_id;
  this->comment_id = arg_comment_id;
  this->author = arg_author;
  this->msg = arg_msg;
  this->created = arg_created;
  this->reactions = NULL;
  this->tags = NULL;
}

Json::Value * Comment::dumpJ
(void)
{
  int i;

  Json::Value *result_ptr = new Json::Value();
  std::string l_id = this->profile_id + "_" + this->post_id + "_" + this->comment_id;

  (*result_ptr)["id"] = l_id;
  (*result_ptr)["from"] = (*(this->author)->dumpJ());
  (*result_ptr)["message"] = (this->msg)->content;
  (*result_ptr)["created_time"] = *((this->created)->getTimeString());

  // adding the reaction part
  Json::Value json_c_reactions;
  Json::Value json_c_reactions_array;

  if ((this->reactions != NULL) &&
      ((this->reactions)->size() > 0))
    {
      for (i = 0; i < (this->reactions)->size(); i++)
	{
	  Json::Value * dumpjv_ptr = ((*(this->reactions))[i])->dumpJ();
	  json_c_reactions[i] = (*dumpjv_ptr);
	  delete dumpjv_ptr;
	}
      json_c_reactions_array["data"] = json_c_reactions;
      json_c_reactions_array["count"] = ((int) (*(this->reactions)).size());
      (*result_ptr)["reactions"] = json_c_reactions_array;
    }

  // adding the tags part
  Json::Value json_tags;
  Json::Value json_tags_array;

  if ((this->tags != NULL) &&
      ((this->tags)->size() > 0))
    {
      for (i = 0; i < (this->tags)->size(); i++)
	{
	  Json::Value * dumpjv_ptr = ((*(this->tags))[i])->dumpJ();
	  json_tags[i] = (*dumpjv_ptr);
	  delete dumpjv_ptr;
	}
      json_tags_array["data"] = json_tags;
      json_tags_array["count"] = ((int) (*(this->tags)).size());
      (*result_ptr)["tags"] = json_tags_array;
    }

#ifdef _ECS36B_DEBUG_
  std::cout << result_ptr->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */
  return result_ptr;
}
