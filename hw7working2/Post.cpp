
#include "Post.h"

Post::Post
(Person* arg_author, Message* arg_msg)
{
    this->author = arg_author;
    this->msg = arg_msg;
    this->receivers = NULL;
    this->links = NULL;
    this->actions = NULL;
    this->reactions = NULL;
    this->comments = NULL;
    this->tags = NULL;
    this->keys = NULL;
    this->created = NULL;
    this->updated = NULL;
    this->is_published = NULL;
    this->history = NULL;
}

Json::Value *Post::dumpJ
(void)
{
    int i;

    Json::Value *result_ptr = new Json::Value();
    std::string l_id = (this->profile_id + "_" + this->post_id);
    Json::Value *dumpjv_ptr;

    //"id"
    (*result_ptr)["id"] = l_id;

    //"from"
    if(this->author != NULL)
    {
        (*result_ptr)["from"] = (*((this->author)->dumpJ()));
    }

    //"to"
    if ((this->receivers != NULL) &&
        (((int)(*(this->receivers)).size()) > 0))
    {
        Json::Value json_c_receivers;
        Json::Value json_c_receivers_array;
        for(i = 0; i < (this->receivers)->size(); i++)
        {
            Json::Value *dumpjv_ptr = ((*(this->receivers))[i])->dumpJ();
            json_c_receivers[i] = (*dumpjv_ptr);
            delete dumpjv_ptr;
        }
        json_c_receivers_array["data"] = json_c_receivers;
        json_c_receivers_array["count"] = ((int)(*(this->receivers)).size());
        (*result_ptr)["to"] = json_c_receivers_array;
    }

    //"message"
    if(this->msg != NULL)
    {
        (*result_ptr)["message"] = (this->msg)->content;
    }

    //"links"
    if ((this->links != NULL) &&
        (((int)(*(this->links)).size()) > 0))
    {
        Json::Value json_c_links;
        Json::Value json_c_links_array;
        for(i = 0; i < (this->links)->size(); i++)
        {
            Json::Value *dumpjv_ptr = ((*(this->links))[i])->dumpJ();
            json_c_links[i] = (*dumpjv_ptr);
            delete dumpjv_ptr;
        }
        json_c_links_array["data"] = json_c_links;
        json_c_links_array["count"] = ((int)(*(this->links)).size());
        (*result_ptr)["links"] = json_c_links_array;
    }

    //"actions"
    if ((this->actions != NULL) &&
        (((int)(*(this->actions)).size()) > 0))
    {
        Json::Value json_c_actions;
        Json::Value json_c_actions_array;
        for(i = 0; i < (this->actions)->size(); i++)
        {
            Json::Value *dumpjv_ptr = ((*(this->actions))[i])->dumpJ();
            json_c_actions[i] = (*dumpjv_ptr);
            delete dumpjv_ptr;
        }
        json_c_actions_array["data"] = json_c_actions;
        json_c_actions_array["count"] = ((int)(*(this->actions)).size());
        (*result_ptr)["actions"] = json_c_actions_array;
    }

    //"tag"
    if ((this->tags != NULL) &&
        (((int)(*(this->tags)).size()) > 0))
    {
        Json::Value json_c_tags;
        Json::Value json_c_tags_array;
        for(i = 0; i < (this->tags)->size(); i++)
        {
            Json::Value *dumpjv_ptr = ((*(this->tags))[i])->dumpJ();
            json_c_tags[i] = (*dumpjv_ptr);
            delete dumpjv_ptr;
        }
        json_c_tags_array["data"] = json_c_tags;
        json_c_tags_array["count"] = ((int)(*(this->tags)).size());
        (*result_ptr)["tags"] = json_c_tags_array;
    }

    //"keys"
    if ((this->keys != NULL) &&
        (((int)(*(this->keys)).size()) > 0))
    {
        Json::Value json_c_keys;
        Json::Value json_c_keys_array;
        for(i=0; i < (this->keys)->size(); i++)
        {
            Json::Value *dumpjv_ptr = new Json::Value();
            (*dumpjv_ptr) = ((*(this->keys))[i]->okey);
            json_c_keys[i]=(*dumpjv_ptr);
            delete dumpjv_ptr;
        }
        json_c_keys_array["data"] = json_c_keys;
        json_c_keys_array["count"] = ((int)(*(this->keys)).size());
        (*result_ptr)["keys"] = json_c_keys_array;
    }    
    
    //"created_time"
    if(this->created != NULL)
    {
        (*result_ptr)["created_time"] = *((this->created)->getTimeString());
    }

    //"updated_time"
    if(this->updated != NULL)
    {
        (*result_ptr)["updated_time"] = *((this->updated)->getTimeString());
    }
    
    //"reactions"
    if ((this->reactions != NULL) &&
        (((int)(*(this->reactions)).size()) > 0))
    {
        Json::Value json_c_reactions;
        Json::Value json_c_reactions_array;
        for(i = 0; i < (this->reactions)->size(); i++)
        {
            Json::Value *dumpjv_ptr = ((*(this->reactions))[i])->dumpJ();
            json_c_reactions[i] = (*dumpjv_ptr);
            delete dumpjv_ptr;
        }
        json_c_reactions_array["data"] = json_c_reactions;
        json_c_reactions_array["count"] = ((int)(*(this->reactions)).size());
        (*result_ptr)["reactions"] = json_c_reactions_array;
    }    

    //"comments"
    if ((this->comments != NULL) &&
        (((int)(*(this->comments)).size()) > 0))
    {
        Json::Value json_c_comments;
        Json::Value json_c_comments_array;
        for(i = 0; i < (this->comments)->size(); i++)
        {
            Json::Value *dumpjv_ptr = ((*(this->comments))[i])->dumpJ();
            json_c_comments[i] = (*dumpjv_ptr);
            delete dumpjv_ptr;
        }
        json_c_comments_array["data"] = json_c_comments;
        json_c_comments_array["count"] = ((int)(*(this->comments)).size());
        (*result_ptr)["comments"] = json_c_comments_array;
    }

    //"is_published"
    if (this->is_published != NULL)
    {
        (*result_ptr)["is_published"] = ((bool)(this->is_published));
    }

    //"history"
    if ((this->history != NULL) && 
        (((int)(*(this->history)).size()) > 0))
    {
        Json::Value json_c_history;
        Json::Value json_c_history_array;
        for(i = 0; i < (this->history)->size(); i++)
        {
            Json::Value *dumpjv_ptr = ((*(this->history))[i])->dumpJ();
            json_c_history[i] = (*dumpjv_ptr);
            delete dumpjv_ptr;
        }
        json_c_history_array["data"] = json_c_history;
        json_c_history_array["count"] = ((int)(*(this->history)).size());
        (*result_ptr)["history"] = json_c_history_array;
    }

#ifdef _ECS36B_DEBUG_
  std::cout << result_ptr->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */
  return result_ptr;

}