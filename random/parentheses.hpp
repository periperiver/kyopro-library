#pragma once
#include "generator.hpp"
#include<string>
namespace Random{
std::string parentheses(int n){
  std::string res;
  res.reserve(n*2);
  int a=n,b=n;
  while(a>0||b>0){
    long long x=(long long)(a-b)*(a+1);
    long long y=(long long)(a-b+1)*(a+b);
    if(range(y)<x)res+=')',a--;
    else res+='(',b--;
  }
  return res;
}
}