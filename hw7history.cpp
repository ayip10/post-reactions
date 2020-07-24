
// hw7history.cpp
// #define _ECS36B_DEBUG_

#include "Transaction.h"
#include <time.h>

// ./hw7history <vsID> <avatar_name> <type> <status> <latitude> <longitude> <IP_address> <post_json>

JvTime *
getNowJvTime
(void)
{
  time_t ticks; 
  ticks = time(NULL);
  struct std::tm * my_tm_ptr = gmtime(&ticks);
  JvTime * jvT_ptr = new JvTime();
  int rc = jvT_ptr->setStdTM(my_tm_ptr);
  if (rc != 0)
    {
      std::cout << "error: failed to set time!" << std::endl;
      delete jvT_ptr;
      return NULL;
    }
  return jvT_ptr;
}

int
main(int argc, char *argv[])
{
  if (argc != 6)
    {
      char arg_string[] = " <vsID> <avatar_name> <latitude> <longitude> <IP_address>";
      std::cout << "usage: " << argv[0] << arg_string << std::endl; 
      return -1;
    }

  Transaction * transaction_ptr = new Transaction(0);
  std::string tr_agent_avatar_name { argv[2] };
  std::string tr_agent_vsID        { argv[1] };
  Person *agent_ptr = new Person(tr_agent_avatar_name, tr_agent_vsID);
  transaction_ptr->agent = agent_ptr;

  double tr_latitude   = atof(argv[3]);
  double tr_longitude = atof(argv[4]);
  GPS_DD * gps_dd_ptr = new GPS_DD(tr_latitude, tr_longitude);
  transaction_ptr->where = gps_dd_ptr;

  IP_Address * ip_ptr = new IP_Address(inet_addr(argv[5]));
  transaction_ptr->srcIP = ip_ptr;
  transaction_ptr->when = getNowJvTime();

  std::cout << (*(transaction_ptr->dumpJ())).toStyledString() << std::endl;
  return 0;
}
