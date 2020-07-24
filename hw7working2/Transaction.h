
#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include "Core.h"

// Transaction.h
#include <iostream>
#include "Person.h"
#include "JvTime.h"

#define TR_TYPE_FLIGHT 1

class Transaction : public Core
{
 private:
 protected:
 public:

  static unsigned int transaction_count;
  unsigned int type;
  unsigned int status;

  JvTime * when;
  Person * agent;
  GPS_DD * where;
  IP_Address * srcIP;

  Transaction(std::string, std::string, std::string, std::string);
  Transaction(unsigned int);
  Transaction(std::string, std::string, std::string, std::string,
	      unsigned int, Person*,
	      GPS_DD*, IP_Address*, JvTime*);
  virtual Json::Value* dumpJ();
};

#endif  /* _TRANSACTION_H_ */
