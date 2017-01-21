#pragma once

#include <string>

class Element
{
public:
  Element();
  Element(Element*);
  ~Element();
  virtual std::string encode();
  virtual void print();
  virtual void* get();
};

