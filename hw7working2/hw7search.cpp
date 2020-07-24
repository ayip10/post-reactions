
// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <string>

// for JsonRPCCPP
#include <iostream>
#include "hw7server.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>

// JSON RPC part
#include <stdlib.h>
#include "hw7client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

#include "Post.h"
#include <cstring>

using namespace jsonrpc;
using namespace std;

int
main(int argc, char *argv[])
{
  if (argc != 2) return -1;

  FILE *jf1 = fopen(argv[1], "r");
  if (jf1 == NULL) return -1;

  Json::CharReaderBuilder builder;
  Json::CharReader* reader;
  std::string errors;
  bool parsingSuccessful;

  // *** process JSON File ***
  long lSize1;
  size_t lresult1;

  // obtain file size:
  fseek(jf1, 0 , SEEK_END);
  lSize1 = ftell(jf1);
  rewind(jf1);

  // allocate memory to contain the whole file:
  char *jf1_ptr = (char *) malloc(sizeof(char)*lSize1);
  // copy the file into the buffer:
  lresult1 = fread(jf1_ptr, 1, lSize1, jf1);    
  if (lresult1 != lSize1)
    {
      fputs("Reading error", stderr);
      exit (-3);
    }

  Json::Value myv_message;
  reader = builder.newCharReader();
  parsingSuccessful = reader->parse(jf1_ptr, jf1_ptr + lSize1, &myv_message, &errors);
  delete reader;

  if (!parsingSuccessful) {
    std::cout << "Failed to parse the content of the first JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }
  fclose(jf1);

  HttpClient httpclient("http://169.237.6.102:55407");
  hw7Client myClient(httpclient, JSONRPC_CLIENT_V2);

  Json::Value result_json;

  try 
  {
    //std::string search_str = "{\"keyword\":\"ecs36b\"}";
    std::string search_str = (myv_message.toStyledString());
    result_json = myClient.search(search_str);
    std::cout << result_json.toStyledString() << endl;
    if ((result_json["status"]).isNull() != true)
    {
      return 0;
    }
    else if ((result_json["count"]).asInt() > 0)
    {
      //Post * my_post_ptr = Json_2_Post(result_json["data"][0]);
      int k;

      for(k = 0; k < (result_json["count"]).asInt(); k ++)
      {
        char fname_buf[1024];    // read/write to post_<profile_id>_<post_id>.json
        FILE * post_f_ptr;       // FILE to the file above

        Post * post_ptr = NULL;
        bzero(fname_buf, 1024);
        sprintf(fname_buf, "post_%s.json", ((result_json["data"][k]["id"]).asString()).c_str());

        long lSize;
        size_t lresult;

        Json::Value creating_json = result_json["data"][k];
        post_ptr = Json_2_Post(creating_json);

        post_f_ptr = fopen(fname_buf, "w");
        if (post_f_ptr != NULL)
        {
          char post_json_buf[8192]; // limited size
          bzero(post_json_buf, 8192);
          sprintf(post_json_buf, "%s", ((*(post_ptr->dumpJ())).toStyledString()).c_str());
          lSize = strlen(post_json_buf);
          std::cout << fname_buf << " " << lSize <<  std::endl;
          lresult = fwrite(post_json_buf, 1, lSize, post_f_ptr);    
          if (lresult != lSize)
	        {
	          fputs("Reading error", stderr);
	          exit (-3);
	        }
          fclose(post_f_ptr);
        }
      }
    } 
  }
  catch (JsonRpcException &e) 
  {
    cerr << e.what() << endl;
  }

  return 0;
}