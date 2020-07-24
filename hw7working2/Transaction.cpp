
#include "Transaction.h"

unsigned int Transaction::transaction_count {};
// unsigned int Flight::flight_count { };

Transaction::Transaction(unsigned int arg_type) 
{ 
  transaction_count++;
}

Transaction::Transaction(std::string core_arg_host_url, std::string core_arg_owner_vsID,
			 std::string core_arg_class_id, std::string core_arg_object_id,
			 unsigned int arg_type, Person* arg_agent,
			 GPS_DD* arg_where, IP_Address* arg_srcIP, JvTime* arg_when)
  : Core { core_arg_host_url, core_arg_owner_vsID, core_arg_class_id, core_arg_object_id }
{
  this->type = arg_type;
  this->agent = arg_agent;
  this->where = arg_where;
  this->srcIP = arg_srcIP;
  this->when = arg_when;
  transaction_count++;
}

Json::Value * Transaction::dumpJ
(void)
{
  Json::Value *result_ptr = new Json::Value();

  (*result_ptr)["avatar_name"] = ((this->agent)->getAvatarName());
  (*result_ptr)["vsID"] = ((this->agent)->getVsID());
  (*result_ptr)["created_time"] = *((this->when)->getTimeString());
  (*result_ptr)["location"] = (*(this->where)->dumpJ());
  (*result_ptr)["network"]= (*(this->srcIP)->dumpJ());
#ifdef _ECS36B_DEBUG_
  std::cout << result_ptr->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */
  return result_ptr;
}

Transaction::Transaction(std::string core_arg_host_url, std::string core_arg_owner_vsID,
			 std::string core_arg_class_id, std::string core_arg_object_id)
  : Core { core_arg_host_url, core_arg_owner_vsID, core_arg_class_id, core_arg_object_id }
{
  transaction_count++;
  std::cout << "Core Constructor" << std::endl;
}