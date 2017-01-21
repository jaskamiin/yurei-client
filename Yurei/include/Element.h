#pragma once

#include <map>
#include <string>
#include <vector>

enum element_type {INTEGER, STRING, LIST, DICT};

class Element
{
public:
  struct value {
    int64_t _integer;
    std::string _string;
    std::vector<Element*> _list;
    std::map<std::string, Element*> _dict;
    element_type type;
  };

public:
  Element();
  virtual ~Element()=0;
  virtual std::string encode();
  virtual void print();
  virtual short get_type();
  
  // element_type type;
  value* val;
};

