#ifndef ABSTRACTWRAPPER_H
#define ABSTRACTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

class AbstractWrapper {
public:
  static volatile bool GlobalStop;
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename) = 0;
  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results) =0;
};

class WrapperFactory {
public:
  // static method for creating a wrapper
  static AbstractWrapper* createWrapper();
private:
  // the following three will prevent the creation of WrapperFactory objects
  WrapperFactory();
  WrapperFactory(const WrapperFactory&);
  WrapperFactory& operator=(const WrapperFactory&);

  // the Wrapper object
  static AbstractWrapper* wrapper;
};

#endif
