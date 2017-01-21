#pragma once

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <ostream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "yurei_utils.h"
#include "Element.h"


class Bencoder
{
private:

  struct value {
    int64_t _integer;
    std::string _string;
    std::vector<Element*> _list;
    std::map<std::string, Element*> _dict;
    element_type type;
  };
  
  class Integer : public Element
  {
  public:
    value* val;
    
    Integer();
    Integer(const int64_t);
    ~Integer();

    //element_type type = INTEGER;
    
    virtual std::string encode();
    virtual void print();
    virtual short get_type() { return this->val->type; }
    
    static Element* read(std::string, unsigned&);
    int64_t get() { return val->_integer; };    
  };

  class String : public Element
  {
  public:
    value* val;
    
    String();
    String(const std::string);
    ~String();

    //element_type type = STRING;
    
    virtual std::string encode();
    virtual void print();
    virtual short get_type() { return this->val->type; }
    
    static Element* read(std::string, unsigned&);
    std::string get() { return val->_string; };
  };

  class List : public Element
  {
  public:
    value* val;
    
    List();
    List(const std::vector<Element*>);
    ~List();

    //element_type type = LIST;
    
    virtual std::string encode();
    virtual void print();
    virtual short get_type() { return this->val->type; }
        
    static Element* read(std::string, unsigned&);
    void add(Element* obj) { val->_list.push_back(obj); };
    std::vector<Element*> get() {return val->_list;};
  };

  struct Dict : public Element
  {
    value* val;
    
    Dict();
    Dict(const std::map<std::string, Element*>);
    ~Dict();

    virtual std::string encode();
    virtual void print();
    
    static Element* read(std::string, unsigned&);
    void add(std::string k, Element* v) { val->_dict.insert({ k, v }); };
    std::map<std::string, Element*> get() {return val->_dict;}
  };

private:
  std::vector<Element*> bencoded;

  // void freeList(Element*);
  // void freeDict(Element*);

  void deleteItem(Integer*&);
  void deleteItem(String*&);
  void deleteItem(List*&);
  void deleteItem(Dict*&);
  
  short get_type(Element* &e) { return e->val->type; }
  //void  print_element(Integer& e);
  //void  print_element(String& e);
  //void  print_element(List& e);
  //void  print_element(Dict& e);
  
public:
  Bencoder() {};
  Bencoder(std::string);
  ~Bencoder();

  static std::vector<Element*> decode(std::string);
  static Element* decode(std::string, unsigned&);

  int bdecode(std::string);

  void print();

};
