
#include <iostream>

// JSON RPC part
#include <stdlib.h>
#include "hw6client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

#include "Post.h"

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

  //Post * my_post_ptr = new Post((Person *) NULL, (Message *) NULL);
  Post * my_post_ptr = Json_2_Post(myv_message);
  //int rc = Json_2_Post(myv_message, my_post_ptr);

  HttpClient httpclient("http://169.237.6.102:55407");
  hw6Client myClient(httpclient, JSONRPC_CLIENT_V2);

  Json::Value result_json;

  // below: just simple examples

  try {
    std::cout << (my_post_ptr->dumpJ())->toStyledString() << std::endl;
   // result_json = myClient.validate((my_post_ptr->dumpJ())->toStyledString());
    //std::cout << result_json.toStyledString() << endl;
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }

 // try {
   // std::string search_str;
    //search_str += "{\"keyword\" : \"ecs36b\"}";
    //result_json = myClient.search(search_str);
    //std::cout << result_json.toStyledString() << endl;
  //} catch (JsonRpcException &e) {
    //cerr << e.what() << endl;
  //}

  return 0;
}
