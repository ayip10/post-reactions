
#ifndef _PERSON_H_
#define _PERSON_H_

#include "Core.h"

// Person.h

class GPS_DD
{
 private:
  double latitude;
  double longitude;
 public:
  GPS_DD();
  GPS_DD(double, double);
  double getLatitude();
  double getLongitude();
  double distance(GPS_DD *);
  virtual Json::Value * dumpJ();
};

class IP_Address
{
 private:
  unsigned int addr;
 public:
  IP_Address();
  IP_Address(unsigned int);
  std::string& getIPaddressString();
  unsigned int getIPaddressValue();
  virtual Json::Value * dumpJ();
};

class Person : public Core
{
 private:
  static unsigned int person_count;
  std::string vsID;
  std::string avatar_name;
  std::string hometown;
  GPS_DD home;
 public:
  Person(std::string, std::string, std::string, std::string);
  Person();
  Person(std::string, std::string, std::string, std::string, std::string, std::string);
  Person(std::string, std::string);
  void setHome(std::string);
  void setHome(GPS_DD);

  virtual bool operator==(Person& aPerson);
  virtual std::string getVsID();
  virtual std::string getAvatarName();
  virtual GPS_DD getHome();
  virtual bool message(Json::Value);
  virtual Json::Value * dumpJ();
};

#endif  /* _PERSON_H_ */
