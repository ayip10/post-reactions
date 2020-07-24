
#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include "Core.h"

// Transaction.h
#include <iostream>
#include "JvTime.h"
#include "Person.h"

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
	      unsigned int, Person *,
	      GPS_DD *, IP_Address *, JvTime *);
  virtual Json::Value * dumpJ();
};

class Airline
{
 private:
 protected:
 public:
};

class Flight: public Transaction
{
 private:
  static unsigned int fl_count;
 protected:
  Person * passenger;
  Airline * airline;
  std::string ticket_number;

 public:
  Flight(std::string, std::string, std::string, std::string, unsigned int, Person *, GPS_DD *,
	 IP_Address *, JvTime *, Person *, Airline *, std::string _ticket, GPS_DD *, GPS_DD *);
  Flight(std::string, std::string, std::string, std::string);
  GPS_DD * departure;
  GPS_DD * arrival;
  virtual double getDistance();
  bool Conflict_of_Interest();
};

#endif  /* _TRANSACTION_H_ */
