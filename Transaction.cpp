
#include "Transaction.h"

unsigned int Transaction::transaction_count { };
// unsigned int Flight::flight_count { };

Transaction::Transaction(unsigned int arg_type) 
  : type { arg_type }
{ 
  transaction_count++;
  // std::cout << "basic Constructor" << std::endl;
  this->type = 0;
  this->status = 0;
  this->when = NULL;
  this->agent = NULL;
  this->where = NULL;
  this->srcIP = NULL;
}

Transaction::Transaction(std::string core_arg_host_url, std::string core_arg_owner_vsID,
			 std::string core_arg_class_id, std::string core_arg_object_id,
			 unsigned int arg_type, Person * arg_agent,
			 GPS_DD * arg_where, IP_Address * arg_src, JvTime * arg_when)
  : Core { core_arg_host_url, core_arg_owner_vsID, core_arg_class_id, core_arg_object_id },
    type { arg_type }, when { arg_when }, agent { arg_agent },
    where { arg_where }, srcIP { arg_src }
{
  transaction_count++;
  std::cout << "full Constructor" << std::endl;
}

Transaction::Transaction(std::string core_arg_host_url, std::string core_arg_owner_vsID,
			 std::string core_arg_class_id, std::string core_arg_object_id)
  : Core { core_arg_host_url, core_arg_owner_vsID, core_arg_class_id, core_arg_object_id }
{
  transaction_count++;
  std::cout << "Core Constructor" << std::endl;
  this->type = 0;
  this->status = 0;
  this->when = NULL;
  this->agent = NULL;
  this->where = NULL;
  this->srcIP = NULL;
}

Json::Value *
Transaction::dumpJ()
{
  if (this->agent == NULL)
    {
      std::cout << "Agent NULL\n";
      return NULL;
    }

  Json::Value * result_ptr = (this->agent)->dumpJ();

  // (*result_ptr)["type"] = this->type;
  // (*result_ptr)["status"] = this->type;

  if (this->where != NULL)
    {
      (*result_ptr)["location"] = *((this->where)->dumpJ());
    }

  if (this->srcIP != NULL)
    {
      (*result_ptr)["network"] = *((this->srcIP)->dumpJ());
    }

  if (this->when != NULL)
    {
      std::string * str_ptr =  (this->when)->getTimeString();
      (*result_ptr)["created_time"] = *str_ptr;
      delete str_ptr;
    }

#ifdef _ECS36B_DEBUG_
  std::cout << result_ptr->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */
  return result_ptr;
}

Flight::Flight
(std::string core_arg_host_url, std::string core_arg_owner_vsID,
 std::string core_arg_class_id, std::string core_arg_object_id,
 unsigned int transaction_arg_type, Person * transaction_arg_agent,
 GPS_DD * transaction_arg_where, IP_Address * transaction_arg_src, JvTime *transaction_arg_when,
 Person * arg_passenger, Airline * arg_airline, std::string arg_ticket,
 GPS_DD * arg_departure, GPS_DD * arg_arrival)
  : Transaction { core_arg_host_url, core_arg_owner_vsID, core_arg_class_id, core_arg_object_id,
    transaction_arg_type, transaction_arg_agent, transaction_arg_where,
    transaction_arg_src, transaction_arg_when },
    passenger { arg_passenger }, airline { arg_airline }, ticket_number { arg_ticket },
    departure { arg_departure }, arrival { arg_arrival }
{
}

GPS_DD dummy { 0.0, 0.0 };

Flight::Flight(std::string core_arg_host_url, std::string core_arg_owner_vsID,
	       std::string core_arg_class_id, std::string core_arg_object_id)
  : Transaction { core_arg_host_url, core_arg_owner_vsID, core_arg_class_id, core_arg_object_id },
	       departure { &dummy }, arrival { &dummy }
{
}


bool
Flight::Conflict_of_Interest()
{
  return ((*(this->agent)) == (*(this->passenger)));
}

double
Flight::getDistance()
{
  return (this->departure)->distance(this->arrival);
}
