// JSON_Post.cpp
// #define _ECS36B_DEBUG_

#include <iostream>
#include <stdlib.h>
#include <arpa/inet.h>
#include "Post.h"
#include "Transaction.h"

int Json_Post_Common (Json::Value myv, Post *my_post_ptr); 


std::map<std::string, OKey *> All_Keys;

std::vector<Post *> *getPostbyKey(std::string key)
{
  // searches all keys for the keyword
  if (All_Keys.find(key) == All_Keys.end())
    return NULL;

  OKey *okey_ptr = All_Keys[key];

  return okey_ptr->posts;
}

OKey *getOKey(std::string key)
{

  if (All_Keys.find(key) != All_Keys.end())
    return All_Keys[key];

  OKey *new_okey_ptr = new OKey(key);
  new_okey_ptr->posts = new std::vector<Post *>();
  All_Keys[key] = new_okey_ptr;
  return new_okey_ptr;
}

std::string Json_2_PostID(Json::Value myv)
{
  return ((myv["id"]).asString());
}


Post *Json_2_Post(Json::Value myv)
{
  int i, j;

  if (myv.isArray() == true)
  {
    return NULL;
  }

  Post * my_post_ptr = new Post((Person *) NULL, (Message *) NULL);
  bzero(my_post_ptr, sizeof(class Post));

  // check "id"
  if ((myv["id"]).isNull() != true)
  {
    char idstr[1024];
    bzero(idstr, 1024);
    sprintf(idstr, "%s", ((myv["id"]).asString()).c_str());
    if (idstr[0] != '\0')
    {
      char c_prof_buf[256];
      char c_post_buf[256];
      bzero(c_prof_buf, 256);
      bzero(c_post_buf, 256);
      sscanf(idstr, "%[^_]_%s", c_prof_buf, c_post_buf);
      std::string profile_id = { c_prof_buf };
      std::string post_id = { c_post_buf };
      my_post_ptr->profile_id = profile_id;
      my_post_ptr->post_id = post_id;
    }
    else
    {
      std::cerr << "No Post ID presented 1" << std::endl;
      exit(-1);
    }
  }
  else
  {
    std::cerr << "No Post ID presented 2" << std::endl; 
    exit(-2);
  }

  int rc = Json_Post_Common(myv, my_post_ptr);
  if(rc != 0)
  {
    std::cerr << "Error Json_Post_Common" << rc << std::endl;
    exit(-3);
  }

  return my_post_ptr;
}

// merge new info and existing Post
int Json_Post_Merging(Json::Value myv, Post *my_post_ptr)
{
  int i;

  char idstr[1024];
  bzero(idstr, 1024);

  if ((myv).isArray() == true) return -3;
  if ((myv["id"]).isNull() == true) return -1;

  // compare ID first
  sprintf(idstr, "%s", ((myv["id"]).asString()).c_str());

  if (idstr[0] != '\0')
  {
    char c_prof_buf[256];
    char c_post_buf[256];
    bzero(c_prof_buf, 256);
    bzero(c_post_buf, 256);
    sscanf(idstr, "%[^_]_%s", c_prof_buf, c_post_buf);
    std::string profile_id = { c_prof_buf };
    std::string post_id = { c_post_buf };
    if ((my_post_ptr->profile_id != profile_id) ||
        (my_post_ptr->post_id != post_id))
    {	  
      std::cerr << "Post IDs mismatched!!" << std::endl;
      return -1;
    }
  }
  else
  {
    std::cerr << "No Post ID presented for the second JSON" << std::endl;
    return -2;
  }

  // now that they are matched, let us proceed with the merge action
  return Json_Post_Common(myv, my_post_ptr);
}

Transaction *
Json_Transaction_Common
(Json::Value myv){
  Transaction *transaction_ptr = NULL;
  if(((myv["avatar_name"]).isNull()!=true) && 
    ((myv["avatar_name"]).isString()==true) && 
    (((myv["avatar_name"]).asString()).size() > 0) &&
    ((myv["vsID"]).isNull()!=true) &&
    (((myv["vsID"]).asString()).size() > 0))
    {
      transaction_ptr = new Transaction(0);
      transaction_ptr->status=0;
    

    Person *agent_ptr = new Person((myv["avatar_name"]).asString(),
                        (myv["vsID"]).asString());
    transaction_ptr->agent = agent_ptr;

    if(((myv["location"]).isNull()!=true) &&
      ((myv["location"]["latitude"]).isNull()!=true) &&
      ((myv["location"]["longitude"]).isDouble()==true) &&
      ((myv["location"]["latitude"]).isDouble()==true) &&
      ((myv["location"]["latitude"]).isDouble()==true)){

      GPS_DD * gps_dd_ptr = new GPS_DD((myv["location"]["latitude"]).asDouble(),
                                        (myv["location"]["longitude"]).asDouble());

      transaction_ptr->where = gps_dd_ptr;
    }
  
    if(((myv["network"]).isNull()!=true) &&
      ((myv["network"]["IPv4"]).isNull() != true) &&
      ((myv["network"]["IPv4"]).isString()==true) &&
      (((myv["network"]["IPv4"]).asString()).size() > 0))
    {


      IP_Address * ip_ptr = new IP_Address(inet_addr((myv["IPv4"].asString()).c_str()));
      //IP_Address * ip_ptr = new IP_Address(inet_addr(myv["IPv4"].asString()).c_str());
      //  (inet_addr(((myv["network"]["IPv4"]).asString()).c_str()));
      transaction_ptr->srcIP = ip_ptr;
    }

    if(((myv["created_time"]).isNull()!=true) &&
      ((myv["created_time"]).isString()==true) &&
      (((myv["created_time"]).asString()).size() > 0));
    {
      JvTime * cr_time_ptr = 
        new JvTime(((myv["created_time"]).asString()).c_str());
      transaction_ptr->when = cr_time_ptr;
    }
}
return transaction_ptr;
}

int Json_Post_Common(Json::Value myv, Post * my_post_ptr)
{
	int i, j;

  // "actions" done
  if (((myv["actions"]).isNull() != true) && 
      ((myv["actions"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->actions == NULL)
      my_post_ptr->actions = new vector<Action *>();
    
    for (i = 0; i < (myv["actions"]["count"]).asInt(); i++)
    {
      Json::Value l_jv = myv["actions"]["data"][i];
      if (((l_jv["host_url"]).isNull() != true) &&
          ((l_jv["owner_vsID"]).isNull() != true) &&
          ((l_jv["class_id"]).isNull() != true) &&
          ((l_jv["object_id"]).isNull() != true) &&
          ((l_jv["name"]).isNull() != true))
      {
        Action * action_ptr = new Action((l_jv["host_url"]).asString(), 
                        (l_jv["owner_vsID"]).asString(),
                        (l_jv["class_id"]).asString(),
                        (l_jv["object_id"]).asString(),
                        (l_jv["name"]).asString());
        // push them back 
        (*(my_post_ptr->actions)).push_back(action_ptr);
      }
    }
  }

  // "comments" done
  Comment *comment_ptr;
  vector <Reaction*>* vec_c_reactions_ptr;
  vector <Tag *>* vec_tag_ptr = new vector <Tag*>();
  if (((myv["comments"]).isNull() != true) &&
      ((myv["comments"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->comments == NULL)
      my_post_ptr->comments = new vector<Comment *>();
    
    for (i = 0; i < (myv["comments"]["count"]).asInt(); i++)
    {
      Json::Value l_jv = myv["comments"]["data"][i];

      if (((l_jv["created_time"]).isNull() != true) &&
          ((l_jv["from"]).isNull() != true) &&
          ((l_jv["id"]).isNull() != true) &&
          ((l_jv["message"]).isNull() != true)) 
      {
        // set profile, post, and comment ids
        char profile_id[40];
        char post_id[16];
        char comment_id[16];
        sscanf(((l_jv["id"]).asString()).c_str(),"%[^_]_%[^_]_%s", profile_id, post_id, comment_id);
                
        // set author
        Person *author_ptr = new Person((l_jv["from"]["avatar_name"]).asString(),
                                         (l_jv["from"]["vsID"]).asString());

        // set JvTime
        JvTime *jvtime_ptr = new JvTime(((l_jv["created_time"]).asString()).c_str());

        // set message
        Message *msg_ptr = new Message(l_jv["message"].asString());

        // input set values
        Comment *comment_ptr = new Comment(profile_id, post_id, comment_id, author_ptr, msg_ptr, jvtime_ptr);

  
        if (((l_jv["reactions"]).isNull() != true) &&
        ((l_jv["reactions"]["count"]).asInt() > 0))
        {
          vec_c_reactions_ptr = new vector<Reaction*> ();
          for (i = 0; i < (l_jv["reactions"]["count"]).asInt(); i++)
          {
            Json::Value l_jv_react = l_jv["reactions"]["data"][i];
            if (((l_jv_react["avatar_name"]).isNull() != true) &&
                ((l_jv_react["vsID"]).isNull() != true) &&
                ((l_jv_react["created_time"]).isNull() != true) &&
                ((l_jv_react["type"]).isNull() != true))
            {
              Person *react_p_ptr = new Person((l_jv_react["avatar_name"]).asString(),
                                                (l_jv_react["vsID"]).asString()); 
              JvTime *react_cr_time_ptr = new JvTime(((l_jv_react["created_time"]).asString()).c_str());
              Reaction *react_ptr = new Reaction(l_jv_react["type"].asString(),
                                                 react_p_ptr, react_cr_time_ptr);
              // pushing it back
            	vec_c_reactions_ptr->push_back(react_ptr);
			      }
          }
        }

        if (((l_jv["tags"]).isNull() != true) &&
            ((l_jv["tags"]["count"]).asInt() > 0))
        {
          vec_tag_ptr = new vector<Tag*> ();
          for (i = 0; i < (l_jv["tags"]["count"]).asInt(); i++)
          {
            Json::Value l_jv_tag = l_jv["tags"]["data"][i];
            
            if (((l_jv_tag["index"]).isNull() != true) &&
                ((l_jv_tag["avatar_name"]).isNull() != true) &&
                ((l_jv_tag["comment"]).isNull() != true) &&
                ((l_jv_tag["vsID"]).isNull() != true))
            {
              Tag * tag_ptr = new Tag();
              tag_ptr->index = (l_jv_tag["index"]).asInt();
              tag_ptr->avatar_name = (l_jv_tag["avatar_name"]).asString();
              tag_ptr->comment = (l_jv_tag["comment"]).asString();
              tag_ptr->vsID = (l_jv_tag["vsID"]).asString();
              // pushing it back
              vec_tag_ptr->push_back(tag_ptr);
            }
          }
        }

        comment_ptr->reactions = vec_c_reactions_ptr;
        comment_ptr->tags = vec_tag_ptr;

        // push
        (*(my_post_ptr->comments)).push_back(comment_ptr);
      }
    }
  }

  // "keys" done
  if (((myv["keys"]).isNull() != true) &&
  ((myv["keys"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->keys == NULL)
      my_post_ptr->keys = new vector<OKey *>();
    
    for (i = 0; i < (myv["keys"]["count"]).asInt(); i++)
    {
      Json::Value l_jv = myv["keys"]["data"][i];
    
      if (l_jv.isNull() != true)
      {
        OKey *okey_ptr = getOKey(l_jv.asString());
        // append all posts w/ matching key to a vector
        (*(okey_ptr->posts)).push_back(my_post_ptr);
        (*(my_post_ptr->keys)).push_back(okey_ptr);
       // std::cout << (*(okey_ptr->posts)) << std::endl;
        //std::cout << (*(my_post_ptr->keys)) << std::endl;
      }
    }
  }

  // "links" done
  if (((myv["links"]).isNull() != true) &&
      ((myv["links"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->links == NULL)
      my_post_ptr->links = new vector<Link *>();
    
    for (i = 0; i < (myv["links"]["count"]).asInt(); i++)
    {
      Json::Value l_jv = myv["links"]["data"][i];
      if (((l_jv["url"]).isNull() != true) &&
          ((l_jv["class_id"]).isNull() != true) &&
          ((l_jv["object_id"]).isNull() != true))
      {
        Link * link_ptr = new Link((l_jv["url"]).asString(),
                      (l_jv["class_id"]).asString(), 
                      (l_jv["object_id"]).asString());
        (*(my_post_ptr->links)).push_back(link_ptr);
      }
    }
  }

  // "message" done
  if ((myv["message"]).isNull() != true)
  {
    my_post_ptr->msg = new Message(myv["message"].asString());
  }

  // "reactions" done
  if (((myv["reactions"]).isNull() != true) &&
      ((myv["reactions"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->reactions == NULL)
      my_post_ptr->reactions = new vector<Reaction *>();
    
    for (i = 0; i < (myv["reactions"]["count"]).asInt(); i++)
    {
      Json::Value l_jv = myv["reactions"]["data"][i];
      if (((l_jv["avatar_name"]).isNull() != true) &&
          ((l_jv["vsID"]).isNull() != true) &&
          ((l_jv["created_time"]).isNull() != true) &&
          ((l_jv["type"]).isNull() != true))
      {
        Person *react_p_ptr = new Person((l_jv["avatar_name"]).asString(),
                        (l_jv["vsID"]).asString()); 
        JvTime *react_cr_time_ptr =
            new JvTime(((l_jv["created_time"]).asString()).c_str());
        Reaction *react_ptr = new Reaction(l_jv["type"].asString(),
                            react_p_ptr, react_cr_time_ptr);
        (*(my_post_ptr->reactions)).push_back(react_ptr);
      }
    }
  }

    //history
    if(((myv["history"]).isNull()!=true)&&
      ((myv["history"]["data"]).isNull()!=true)&&
      ((myv["history"]["data"]).isArray()==true))
    {
      if(my_post_ptr->history == NULL)
        my_post_ptr->history = new vector<Transaction *>();
      for(i=0;i<(myv["history"]["data"]).size();i++){
        Json::Value l_jv = myv["history"]["data"][i];
        Transaction * transaction_ptr = Json_Transaction_Common(l_jv);
        if(transaction_ptr!=NULL)
        {
          //check if the transaction already exists
          vector<Transaction *>::iterator my_it_hs;
          int flag_hs=0;
          for(my_it_hs = (*(my_post_ptr->history)).begin();
            my_it_hs < (*(my_post_ptr->history)).end();my_it_hs++)
          {
            if((*((*my_it_hs)->agent)) == (*(transaction_ptr->agent))) 
             // && ((*((*my_it_hs)->when)) == (*(transaction_ptr->when))))
            {
              flag_hs = 1;
            }
          }
          if(flag_hs == 0){
            (*(my_post_ptr->history)).push_back(transaction_ptr);
        }
        else{
          if(transaction_ptr->agent != NULL)
            delete transaction_ptr->agent;
          if(transaction_ptr->where != NULL)
            delete transaction_ptr->where;
          if(transaction_ptr->srcIP != NULL)
            delete transaction_ptr->srcIP;
          if(transaction_ptr->when != NULL)
            delete transaction_ptr->when;

          delete transaction_ptr;
        }
      }

    }
}

  // "from" done
  if ((myv["from"]).isNull() != true)
  {
    my_post_ptr->author = new Person((myv["from"]["avatar_name"]).asString(),
                                     (myv["from"]["vsID"]).asString());
  }

  // "to" done
  if (((myv["to"]).isNull() != true) &&
  ((myv["to"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->receivers == NULL)
      my_post_ptr->receivers = new vector<Person *>();
    
    for (i = 0; i < (myv["to"]["count"]).asInt(); i++)
      {
        Json::Value l_jv = myv["to"]["data"][i];
        
        if (((l_jv["avatar_name"]).isNull() != true) &&
            ((l_jv["vsID"]).isNull() != true))
        {
          Person *to_ptr = new Person((l_jv["avatar_name"]).asString(),
                                      (l_jv["vsID"]).asString());
          (*(my_post_ptr->receivers)).push_back(to_ptr);
        }
      }
  }

  // "created_time" done
  if ((myv["created_time"]).isNull() != true)
  {
    my_post_ptr->created = new JvTime(((myv["created_time"]).asString()).c_str());
  }

  // "updated_time" done
  if ((myv["updated_time"]).isNull() != true)
  {
    my_post_ptr->updated = new JvTime(((myv["updated_time"]).asString()).c_str());
  }
  
  // "is_published" done
  my_post_ptr->is_published = (myv["is_published"]).asBool();

  return 0;
}