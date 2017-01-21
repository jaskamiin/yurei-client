#include "../include/Bencoder.h"
#include "../include/Element.h"

Bencoder::Bencoder(std::string encoded){
  this->bencoded = decode(encoded);
}

Bencoder::~Bencoder(){
  std::cout << "Destroying Bencoder\n";
  for (auto&& item : this->bencoded){
    deleteItem(*item);
    /*
    std::cout << get_type(item);
    switch(item->val->type){
      case LIST:
	freeList(item);
	break;
      case DICT:
	freeDict(item);
	break;
      case INTEGER:
	//std::cout << "Deleting Integer\n";
	std::cout << "Deleting " << ((Bencoder::Integer*)item)->get() << std::endl;
	break;
      case STRING:
	//std::cout << "Deleting String\n";// << item->get() << std::endl;
	std::cout << "Deleting " << ((Bencoder::String*)item)->get() << std::endl;
	break;
	}
    delete item;
    */
  }
}

void Bencoder::deleteItem(Bencoder::Integer*& e){
  delete e;
}

void Bencoder::deleteItem(Bencoder::String*& e){
  delete e;
}

void Bencoder::deleteItem(Bencoder::List*& e){
  for (auto&& item : e->val->_list){
    deleteItem(item);
  }
}

void Bencoder::deleteItem(Bencoder::Dict*& e){
  for (auto&& entry : e->val->_dict){
    deleteItem(entry.second);
  }
}

/*
void Bencoder::freeList(Element* list){
  std::cout << "Freeing list...\n";
  for (auto&& item : list->val->_list){
    switch (item->val->type){
      case LIST:
	freeList(item);
	break;
      case DICT:
	freeDict(item);
	break;
      case INTEGER:
	//std::cout << "\tDeleting Integer\n";
	std::cout << "Deleting " << ((Bencoder::Integer*)item)->get() << std::endl;
	break;
      case STRING:
	//std::cout << "\tDeleting String\n";
	std::cout << "Deleting " << ((Bencoder::String*)item)->get() << std::endl;
	break;
    }
    delete item;
  }
}

void Bencoder::freeDict(Element* dict){
  std::cout << "Deleting Dict\n";
  for (auto&& entry : dict->val->_dict){
    switch(entry.second->val->type){
      case LIST:
	freeList(entry.second);
	break;
      case DICT:
	freeDict(entry.second);
	break;
      case INTEGER:
	//std::cout << "\tDeleting Integer\n";
	std::cout << "Deleting " << ((Bencoder::Integer*)entry.second)->get() << std::endl;
	break;
      case STRING:
	//std::cout << "\tDeleting String\n";
	std::cout << "Deleting " << ((Bencoder::String*)entry.second)->get() << std::endl;
	break;
    }
    delete entry.second;
  }
}
*/

void Bencoder::print(){
  for (auto&& item : this->bencoded){
    item->print();
  }
}

/*INTEGER class definitions*/
Bencoder::Integer::Integer(){
  this->val = new value;
  this->val->type = INTEGER;
}

Bencoder::Integer::Integer(const int64_t i){
  this->val = new value;
  this->val->_integer=i;
}

Bencoder::Integer::~Integer(){
  delete this->val;
}

std::string Bencoder::Integer::encode(){
  return  "";
}

Element* Bencoder::Integer::read(std::string content, unsigned& idx){
  if (!isdigit(content.at(idx))){
    std::size_t e = content.find("e", ++idx);
    if (e == std::string::npos) throw BENCODE_INT_BAD_FORMAT;
  }

  int val = strtoll(&content[idx], NULL, 10);
  idx += (count_digits(val) + 1);

  return new Integer(val);
}

void Bencoder::Integer::print(){
  std::cout << this->val->_integer << "\n";
}

/*STRING class definitions*/
Bencoder::String::String(){
  this->val = new value;
  this->val->type = STRING;
}

Bencoder::String::String(const std::string s){
  this->val = new value;
  this->val->_string=s;
}

Bencoder::String::~String(){
  delete this->val;
}

std::string Bencoder::String::encode(){
  return  "";
}

Element* Bencoder::String::read(std::string content, unsigned& idx){
  int64_t len = ((Integer*)Integer::read(content, idx))->get();
	
  if (content.at(idx-1) != ':'){
    throw BENCODE_STR_BAD_FORMAT;
  }

  std::string val = content.substr(idx, len);
  
  idx += len;
  return new String(val);
}

void Bencoder::String::print(){
  std::cout << this->val->_string << "\n";
}

/*LIST class definitions*/
Bencoder::List::List(){
  this->val = new value;
  this->val->type = LIST;
}

Bencoder::List::List(const std::vector<Element*> v){
  this->val = new value;
  this->val->_list=v;
}

Bencoder::List::~List(){
  delete this->val;
}

std::string Bencoder::List::encode(){
  return  "";
}

Element* Bencoder::List::read(std::string content, unsigned& idx){
  if (content.at(idx) == 'l') idx++;

  std::vector<Element*> list;

  while (content.at(idx) != 'e'){
    list.push_back(decode(content, idx));
  }
  
  idx++;
  return new List(list);
}

void Bencoder::List::print(){
  for (auto&& x : this->val->_list){
    x->print();
  }
}

/*DICT class definitions*/
Bencoder::Dict::Dict(){
  this->val = new value;
  this->val->type = DICT;
}

Bencoder::Dict::Dict(const std::map<std::string, Element*> m){
  this->val = new value;
  this->val->_dict=m;
}

Bencoder::Dict::~Dict(){
  delete this->val;
}

std::string Bencoder::Dict::encode(){
  return "";
}

Element* Bencoder::Dict::read(std::string content, unsigned& idx){
  if (content.at(idx) == 'd') idx++;

  std::map<std::string, Element*> dict;

  while (content.at(idx) != 'e'){
    std::string key = ((String*)String::read(content, idx))->get();
    Element* value  = decode(content, idx);
    dict.insert({key, value});
  }

  idx++;
  return new Dict(dict);
}

void Bencoder::Dict::print(){
  for (auto&& x : this->val->_dict){
    std::cout << x.first << " --> ";
    x.second->print();
  }
}
	
/*BENCODER class definitions*/
std::vector<Element*> Bencoder::decode(std::string content){
  unsigned idx = 0;
  std::vector<Element*> elements;
  std::size_t len = content.length();
	
  while (idx != len){
    elements.push_back(decode(content, idx));
  }
  
  return elements;
}


Element* Bencoder::decode(std::string content, unsigned& idx)
{
  switch (content[idx]){
  case 'i':
    return Integer::read(content, idx);
  case 'l':
    return List::read(content, idx);
  case 'd':
    return Dict::read(content, idx);
  default:
    if (!isdigit(content[idx])){
      break;
    }
    return String::read(content, idx);
  }
		
  throw BENCODE_FAILURE;
}
