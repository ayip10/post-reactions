
#include "JvTime.h"

JvTime::JvTime()
{ 
}

JvTime::JvTime
(const char *time_str)
{
  if (time_str == NULL) return;
  sscanf(time_str, "%4d-%2d-%2dT%2d:%2d:%2d+%4s",
	 &(this->year), &(this->month), &(this->day),
	 &(this->hour), &(this->minute), &(this->second),
	 this->tail4);
}

int JvTime::setStdTM(struct std::tm* input)
{
  this->second = input->tm_sec;
  this->minute = input->tm_min;
  this->hour = input->tm_hour;
  this->day = input->tm_mday;
  this->month = (input->tm_mon) + 1;
  this->year = (input->tm_year) + 1900;

  return 0;
}

struct std::tm *
JvTime::getStdTM
(void)
{
  struct std::tm * result = (struct std::tm *) malloc(sizeof(struct std::tm));
  bzero(result, sizeof(struct std::tm));

  result->tm_sec = this->second;
  result->tm_min = this->minute;
  result->tm_hour = this->hour;
  result->tm_mday = this->day;
  result->tm_mon = (this->month) - 1;
  result->tm_year = (this->year) - 1900;

  return result;
}

std::string *
JvTime::getTimeString
(void)
{
  struct std::tm * tm_ptr = this->getStdTM();

  char buffer[64];
  bzero(buffer, 64);
  std::strftime(buffer, 32, "%Y-%m-%dT%H:%M:%S+", tm_ptr);
  sprintf(buffer, "%s%s", buffer, this->tail4);
  return (new std::string(buffer));
}

