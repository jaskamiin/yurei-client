#include "../include/Bencoder.h"
#include "../include/Element.h"


/*INTEGER class definitions*/

std::string Bencoder::Integer::encode(){
  return  "";
}

std::shared_ptr<Bencoder::Integer> Bencoder::Integer::read(std::string content, unsigned& idx){
  if (!isdigit(content.at(idx))){
    std::size_t e = content.find("e", ++idx);
    if (e == std::string::npos) throw BENCODE_INT_BAD_FORMAT;
  }

  int val = strtoll(&content[idx], NULL, 10);
  idx += (count_digits(val) + 1);

  return std::make_shared<Integer>(new Integer(val));
}

void Bencoder::Integer::print(){
  std::cout << this->value << "\n";
}

/*STRING class definitions*/

std::string Bencoder::String::encode(){
  return  "";
}

std::shared_ptr<Bencoder::String> Bencoder::String::read(std::string content, unsigned& idx){
  //int64_t len = *((int64_t*)Integer::read(content, idx)->get());
  int64_t len = ((Integer*)Integer::read(content, idx))->value;
  if (content.at(idx-1) != ':'){
    throw BENCODE_STR_BAD_FORMAT;
  }

  std::string val = content.substr(idx, len);
  
  idx += len;
  return std::make_shared<String>(new String(val));
}

void Bencoder::String::print(){
  std::cout << this->value << "\n";
}

/*LIST class definitions*/

std::string Bencoder::List::encode(){
  return  "";
}

std::shared_ptr<Bencoder::List> Bencoder::List::read(std::string content, unsigned& idx){
  if (content.at(idx) == 'l') idx++;

  std::vector<std::shared_ptr<Element>> list;

  while (content.at(idx) != 'e'){
    list.push_back(decode(content, idx));
  }
  
  idx++;
  return std::make_shared<List>(new List(list));
}

void Bencoder::List::print(){
  for (auto&& x : this->value){
    x->print();
  }
}

/*DICT class definitions*/

std::string Bencoder::Dict::encode(){
  return "";
}

std::shared_ptr<Bencoder::Dict> Bencoder::Dict::read(std::string content, unsigned& idx){
  if (content.at(idx) == 'd') idx++;

  std::map<std::string, std::shared_ptr<Element>> dict;

  while (content.at(idx) != 'e'){
    //std::string key = *((std::string*)String::read(content, idx)->get());
    std::shared_ptr<Element> k_elem = dynamic_cast<std::shared_ptr<String>*>(String::read(content,idx));
    std::string key = k_elem->value;
    dict.insert(std::make_pair(key, std::move(decode(content,idx))));
  }

  idx++;
  return std::make_shared<Dict>(new Dict(dict));
}

void Bencoder::Dict::print(){
  for (auto&& x : this->value){
    std::cout << x.first << " --> ";
    x.second->print();
  }
}
	
/*BENCODER class definitions*/

std::vector<std::shared_ptr<Element>> Bencoder::decode(std::string content){
  unsigned idx = 0;
  std::vector<std::shared_ptr<Element>> elements;
  std::size_t len = content.length();
	
  while (idx != len){
    elements.push_back(decode(content, idx));
  }
  
  return elements;
}

std::shared_ptr<Element> Bencoder::decode(std::string content, unsigned& idx)
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
