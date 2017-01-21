#include "../include/Element.h"
#include "../include/Bencoder.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <string>

int main(){

  std::fstream err_log;
  err_log.open("err/ErrorLog.out", std::ios::out);

  try{
    std::string bs1("i523e5:abcdel4:spam4:eggsed3:cow3:moo4:spam4:eggse");
    std::vector<std::shared_ptr<Element>> elements1 = Bencoder::decode(bs1);

    std::cout << bs1 << std::endl;
    for (auto&& element : elements1){
      element->print();
    }

    std::string bs2("d9:publisher7:Penguin4:site20:www.penguinbooks.come");
    std::vector<std::shared_ptr<Element>> elements2 = Bencoder::decode(bs2);

    std::cout << bs2 << std::endl;
    for (auto&& element : elements2){
      element->print();
    }
    
  } catch (EXCEPTION e){
    std::cout << "Error " << e << std::endl;
  }
  err_log << "END LOG";
  err_log.close();  
  
  return 0;
}
