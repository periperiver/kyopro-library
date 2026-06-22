#pragma once
#include<chrono>
struct Timer{
  std::chrono::system_clock::time_point start;
  Timer():start(std::chrono::system_clock::now()){}
  int get()const{return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-start).count();}
  void reset(){start=std::chrono::system_clock::now();}
};