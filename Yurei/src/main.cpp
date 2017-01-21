#include "../include/Element.h"
#include "../include/Bencoder.h"

#include <iostream>
#include <fstream>
#include <string>

int main(){

  std::fstream err_log;
  err_log.open("err/ErrorLog.out", std::ios::out);

  try{
    Bencoder bs1("i523e5:abcdel4:spam4:eggsed3:cow3:moo4:spam4:eggse"); 
    //Bencoder bs2("d9:publisher7:Penguin4:site20:www.penguinbooks.come");

    bs1.print();
    //bs2.print();
    
  } catch (EXCEPTION e){
    std::cout << "Error " << e << std::endl;
  }
  err_log << "END LOG";
  err_log.close();  
  
  return 0;
}
