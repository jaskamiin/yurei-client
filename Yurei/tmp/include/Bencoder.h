#pragma once

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <ostream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "yurei_utils.h"
#include "Element.h"


class Bencoder{
private:

  class Integer : public Element{
  //private:
  public:
    int64_t value;
  public:
    Integer() {};
    Integer(int64_t i) : value(i) {}
    ~Integer() {};

    virtual std::string encode();
    virtual void print();
    
    static std::shared_ptr<Integer> read(std::string, unsigned&);
    virtual void* get() { return &value; };    
  };

  class String : public Element{
  public:
    std::string value;
  public:
    String() {};
    String(std::string str) : value(str) {}
    ~String() {};

    virtual std::string encode();
    virtual void print();
    
    static std::shared_ptr<String> read(std::string, unsigned&);
    virtual void* get() { return &value; };
  };

  class List : public Element{
  public:
    std::vector<std::shared_ptr<Element>> value;
  public:
    List() {};
    List(std::vector<std::shared_ptr<Element>> l) : value(l) {}
    ~List() {};

    virtual std::string encode();
    virtual void print();
    
    static std::shared_ptr<List> read(std::string, unsigned&);
    void add(std::shared_ptr<Element> obj) { this->value.push_back(obj); };
    virtual void* get(){ return &value; }
  };

  class Dict : public Element{
  public:
    std::map<std::string, std::shared_ptr<Element>> value;
  public:
    Dict(){};
    Dict(std::map<std::string, std::shared_ptr<Element>> d) : value(d) {}
    ~Dict(){};

    virtual std::string encode();
    virtual void print();
    
    static std::shared_ptr<Dict> read(std::string, unsigned&);
    void add(std::string key, std::shared_ptr<Element> val) { value.insert({ key, val }); };
    virtual void* get() { return &value; }
  };

public:
  Bencoder() {};
  ~Bencoder() {};

  static std::vector<std::shared_ptr<Element>> decode(std::string);
  static std::shared_ptr<Element> decode(std::string, unsigned&);

  /*return decoded object*/
  int bdecode(std::string);
};
