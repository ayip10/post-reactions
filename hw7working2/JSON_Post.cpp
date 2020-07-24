

// JSON_Post.cpp
// #define _ECS36B_DEBUG_

#include "Post.h"
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

 std::map<std::string, OKey *> All_Keys;

std::vector<Post *> *
getPostbyKey
(std::string key)
{
  if (All_Keys.find(key) == All_Keys.end())
  {
    return NULL;
  }

  OKey *okey_ptr = All_Keys[key];
  return okey_ptr->posts;
}

OKey *
getOKey
(std::string key)
{
  if (All_Keys.find(key) != All_Keys.end())
  {
    return All_Keys[key];
  }

  OKey *new_okey_ptr = new OKey(key);
  new_okey_ptr->posts = new std::vector<Post *>();
  All_Keys[key] = new_okey_ptr;
  return new_okey_ptr;
}

std::string
Json_2_PostID
(Json::Value myv)
{
  return ((myv["id"]).asString());
}

Post *
Json_2_Post
(Json::Value myv)
{
  if (myv.isArray() == true)
  {
    return NULL;
  }
  Post * my_post_ptr = new Post((Person *) NULL, (Message *) NULL);


  // "id"
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
      std::cerr << "No Post ID presented" << std::endl;
      exit(-1);
    }
  }
  else
  {
    std::cerr << "No Post ID presented" << std::endl;
    exit(-2);
  }

  int rc = Json_Post_Common(myv, my_post_ptr);
  if (rc != 0)
  {
    std::cerr << "Error Json_Post_Common" << rc << std::endl;
    exit(-3);
  }
  return my_post_ptr;
}

int
Json_Post_Merging
(Json::Value myv, Post *my_post_ptr)
{
  char idstr[1024];
  bzero(idstr, 1024);

  if ((myv).isArray() == true) 
    return -3;
  if ((myv["id"]).isNull() == true)
    return -1;

  // compare ID 
  // "id" 
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
	      (my_post_ptr->post_id    != post_id))
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

  // now, they matched, let us proceed with the merge action
  return Json_Post_Common(myv, my_post_ptr);
}

int
Json_Post_Common
(Json::Value myv, Post * my_post_ptr)
{
  int i, j;
  Json::Value my_post_json = (*(my_post_ptr->dumpJ()));
  // "actions"
  if (((myv["actions"]).isNull() != true) &&
      ((myv["actions"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->actions == NULL)
    {
      my_post_ptr->actions = new vector<Action *>();
    }
    for (i=0; i < (myv["actions"]["count"]).asInt(); i++)
    {
      Json::Value l_jv = myv["actions"]["data"][i];
      if(((l_jv["name"]).isNull() != true) &&
         ((l_jv["host_url"]).isNull() != true) &&
         ((l_jv["owner_vsID"]).isNull() != true) &&
         ((l_jv["class_id"]).isNull() != true) &&
         ((l_jv["object_id"]).isNull() != true) &&
         (l_jv != my_post_json["actions"]["data"][i]))
      {
        Action *act_ptr = new Action((l_jv["name"]).asString(),
                                     (l_jv["host_url"]).asString(),
                                     (l_jv["owner_vsID"]).asString(),
                                     (l_jv["class_id"]).asString(),
                                     (l_jv["object_id"]).asString());
        (*(my_post_ptr->actions)).push_back(act_ptr);
      }
    }
  }

  // "comments"
  if (((myv["comments"]).isNull() != true) &&
      ((myv["comments"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->comments == NULL)
    {
      my_post_ptr->comments = new vector<Comment *>();
    }
    for (i=0; i < (myv["comments"]["count"]).asInt(); i++)
    {
      Json::Value l_jv = myv["comments"]["data"][i];
      if(((l_jv["created_time"]).isNull() != true) &&
         ((l_jv["from"]).isNull() != true) &&
         ((l_jv["id"]).isNull() != true) &&
         ((l_jv["message"]).isNull() != true) &&
         ((l_jv["reactions"]).isNull() != true) &&
         (l_jv != my_post_json["comments"]["data"][i]))
      {
        JvTime *comment_ctime_ptr = new JvTime (((l_jv["created_time"]).asString()).c_str());
        Message *comment_message_ptr = new Message ((l_jv["message"]).asString());
        
        Person *comment_author_ptr;
        comment_author_ptr = new Person((l_jv["from"]["avatar_name"]).asString(),
			                                  (l_jv["from"]["vsID"]).asString());
        
        char idstr[1024];
        bzero(idstr, 1024);
        sprintf(idstr, "%s", ((l_jv["id"]).asString()).c_str());
        std::string arg_profile_id;
        std::string arg_post_id;
        std::string arg_comment_id;
        if (idstr[0] != '\0')
        {
          char c_prof_buf[256];
          char c_post_buf[256];
          char c_comm_buf[256];
          bzero(c_prof_buf, 256);
          bzero(c_post_buf, 256);
          bzero(c_comm_buf, 256);
          sscanf(idstr, "%[^_]_%[^_]_%s", c_prof_buf, c_post_buf, c_comm_buf);
          std::string profile_id = { c_prof_buf };
          std::string post_id = { c_post_buf };
          std::string comment_id = { c_comm_buf};
          arg_profile_id = profile_id;
          arg_post_id = post_id;
          arg_comment_id = comment_id;
        }
        else
        {
          std::cerr << "No Post ID presented" << std::endl;
        }
        Comment *comment_ptr = new Comment(arg_profile_id, 
                                           arg_post_id, 
                                           arg_comment_id,
                                           comment_author_ptr, 
                                           comment_message_ptr, 
                                           comment_ctime_ptr);
        
        // adding the reactions part
        if (((l_jv["reactions"]).isNull() != true) &&
            ((l_jv["reactions"]["count"]).asInt() > 0))
        {
          if(comment_ptr->reactions == NULL)
          {
            comment_ptr->reactions = new vector<Reaction *>();
          }
          for (j = 0; j < l_jv["reactions"]["count"].asInt(); j++)
          {
            Json::Value crl_jv = l_jv["reactions"]["data"][j];
            if (((crl_jv["avatar_name"]).isNull() != true) &&
                ((crl_jv["vsID"]).isNull() != true) &&
                ((crl_jv["created_time"]).isNull() != true) &&
                ((crl_jv["type"]).isNull() != true) &&
                (l_jv != my_post_json["comments"]["data"][i]["reactions"]["data"][j]))
            {
              Person *react_p_ptr = new Person((crl_jv["avatar_name"]).asString(),
			                                 		     (crl_jv["vsID"]).asString()); 
	            JvTime *react_cr_time_ptr =
	              new JvTime(((crl_jv["created_time"]).asString()).c_str());
	            Reaction * react_ptr = new Reaction(crl_jv["type"].asString(),
			                                		        react_p_ptr, react_cr_time_ptr);
              (*(comment_ptr->reactions)).push_back(react_ptr);
            }
          }
        }

        //adding the tags part
        if (((l_jv["tags"]).isNull() != true) &&
            ((l_jv["tags"]["count"]).asInt() > 0))
        {
          if(comment_ptr->tags == NULL)
          {
            comment_ptr->tags = new vector<Tag *>();
          }
          for (j = 0; j < l_jv["tags"]["count"].asInt(); j++)
          {
            Json::Value crl_jv = l_jv["tags"]["data"][j];
            if (((crl_jv["index"]).isNull() != true) &&
                ((crl_jv["avatar_name"]).isNull() != true) &&
                ((crl_jv["vsID"]).isNull() != true) &&
                ((crl_jv["comment"]).isNull() != true) &&
                (l_jv != my_post_json["comments"]["data"][i]["tags"]["data"][j]))
            {
              Tag * tag_ptr = new Tag((crl_jv["index"]).asInt(),
                                      (crl_jv["avatar_name"]).asString(),
                                      (crl_jv["vsID"]).asString(),
                                      (crl_jv["comment"]).asString());
              (*(comment_ptr->tags)).push_back(tag_ptr);
            }
          }
        }
        (*(my_post_ptr->comments)).push_back(comment_ptr);
      }
    }
  }

  //"keys"
  if (((myv["keys"]).isNull() != true) &&
      ((myv["keys"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->keys == NULL)
    {
      my_post_ptr->keys = new vector<OKey *>();
    }
    for (i = 0; i < (myv["keys"]["count"]).asInt(); i++)
    {
      Json::Value l_jv = myv["keys"]["data"][i];
      if((l_jv.isNull() != true) &&
         (l_jv != my_post_json["keys"]["data"][i]))
      {
        OKey *Okey_ptr = getOKey(l_jv.asString());
        (*(Okey_ptr->posts)).push_back(my_post_ptr);
        (*(my_post_ptr->keys)).push_back(Okey_ptr);
      }
    }
  }
  // "links"
  if (((myv["links"]).isNull() != true) &&
      ((myv["links"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->links == NULL)
    {
      my_post_ptr->links = new vector<Link *>();
    }
    for (i = 0; i < (myv["links"]["count"]).asInt(); i++)
	  {
	    Json::Value l_jv = myv["links"]["data"][i];
      if(((l_jv["url"]).isNull() != true) &&
         ((l_jv["class_id"]).isNull() != true) &&
         ((l_jv["object_id"]).isNull() != true) &&
         (l_jv != my_post_json["links"]["data"][i]))
      {
	      Link * link_ptr = new Link((l_jv["url"]).asString(),
				                           (l_jv["class_id"]).asString(), 
				                           (l_jv["object_id"]).asString());
  	    (*(my_post_ptr->links)).push_back(link_ptr);
      }
	  }
  }
 
  // "message"
  if ((myv["message"]).isNull() != true)
  {
    Message *msg_ptr = new Message(myv["message"].asString());
    my_post_ptr->msg = msg_ptr;
  }

  // "reactions"
  if (((myv["reactions"]).isNull() != true) &&
      ((myv["reactions"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->reactions == NULL)
    {
      my_post_ptr->reactions = new vector<Reaction *>();
    }
    for (i = 0; i < myv["reactions"]["count"].asInt(); i++)
    {
      Json::Value l_jv = myv["reactions"]["data"][i];
      if (((l_jv["avatar_name"]).isNull() != true) &&
          ((l_jv["vsID"]).isNull() != true) &&
          ((l_jv["created_time"]).isNull() != true) &&
          ((l_jv["type"]).isNull() != true) &&
          (l_jv != my_post_json["reactions"]["data"][i]))
      {
        Person *react_p_ptr = new Person((l_jv["avatar_name"]).asString(),
			                          		     (l_jv["vsID"]).asString()); 
	      JvTime *react_cr_time_ptr =
	        new JvTime(((l_jv["created_time"]).asString()).c_str());
	      Reaction * react_ptr = new Reaction(l_jv["type"].asString(),
			                          		        react_p_ptr, react_cr_time_ptr);
        (*(my_post_ptr->reactions)).push_back(react_ptr);
      }
    }
  }

  //"history"
  if (((myv["history"]).isNull() != true) &&
      ((myv["history"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->history == NULL)
    {
      my_post_ptr->history = new vector<Transaction *>();
    }
    for (i = 0; i < myv["history"]["count"].asInt(); i++)
    {
      Json::Value l_jv = myv["history"]["data"][i];
      if (((l_jv["avatar_name"]).isNull() != true) &&
          ((l_jv["created_time"]).isNull() != true) &&
          ((l_jv["location"]).isNull() != true) &&
          ((l_jv["network"]).isNull() != true) &&
          ((l_jv["vsID"]).isNull() != true) &&
          (l_jv != my_post_json["history"]["data"][i]))
      {
        Transaction * transaction_ptr = new Transaction(0);
        Person *agent_ptr = new Person((l_jv["avatar_name"]).asString(),
			                          		  (l_jv["vsID"]).asString());
        transaction_ptr->agent = agent_ptr;

        JvTime *cr_time_ptr =
	        new JvTime(((l_jv["created_time"]).asString()).c_str());
        transaction_ptr->when = cr_time_ptr;

        double tr_latitude   = l_jv["location"]["latitude"].asDouble();
        double tr_longtitude = l_jv["location"]["longtitude"].asDouble();
        GPS_DD * gps_dd_ptr = new GPS_DD(tr_latitude, tr_longtitude);
        transaction_ptr->where = gps_dd_ptr;
        
        IP_Address * ip_ptr = new IP_Address(inet_addr(((l_jv["network"]["IPv4"]).asString()).c_str()));
        transaction_ptr->srcIP = ip_ptr;

        (*(my_post_ptr->history)).push_back(transaction_ptr);
      }
    }
  }

  // "id"
  // already done

  // "from"
  if ((myv["from"]).isNull() != true)
  {
    Person *author_ptr;
    author_ptr = new Person((myv["from"]["avatar_name"]).asString(),
		      (myv["from"]["vsID"]).asString()); 
    my_post_ptr->author = author_ptr;
  }
  // "to"
  if (((myv["to"]).isNull() != true) &&
      ((myv["to"]["count"]).asInt() > 0))
  {
    if (my_post_ptr->receivers == NULL)
    {
      my_post_ptr->receivers = new vector<Person *>();
    }
    for (i = 0; i < myv["to"]["count"].asInt(); i++)
    {
      Json::Value l_jv = myv["to"]["data"][i];
      if (((l_jv["avatar_name"]).isNull() != true) &&
          ((l_jv["vsID"]).isNull() != true) &&
          (l_jv != my_post_json["to"]["data"][i]))
      {
        Person *to_ptr = new Person((l_jv["avatar_name"]).asString(),
			                          		(l_jv["vsID"]).asString()); 
        (*(my_post_ptr->receivers)).push_back(to_ptr);
      }
    }
  }
  // "created_time"
  if ((myv["created_time"]).isNull() != true)
  {
    my_post_ptr->created = new JvTime(((myv["created_time"]).asString()).c_str());
  }
  // "updated_time"
  if ((myv["updated_time"]).isNull() != true)
  {
    my_post_ptr->updated = new JvTime(((myv["updated_time"]).asString()).c_str());
  }
  // "is_published"
  my_post_ptr->is_published = (myv["is_published"]).asBool();
return 0;
}