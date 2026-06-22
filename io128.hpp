#pragma once
#include <iostream>
#include <string>
#include <algorithm>
std::istream& operator>>(std::istream&is,__int128_t&x){
  x=0;
  std::string s;
  is>>s;
  bool minus=false;
  if(s[0]=='-'){
    minus=true;
    s.erase(s.begin());
  }
  for(int i=0;i<s.size();i++){
    x*=10;
    x+=s[i]-'0';
  }
  if(minus)x=-x;
  return is;
}
std::ostream& operator<<(std::ostream&os,__int128_t x){
  if(x==0){
    os<<0;
    return os;
  }
  if(x<0){
    os<<'-';
    x=-x;
  }
  std::string s;
  while(x){
    s+='0'+x%10;
    x/=10;
  }
  std::reverse(s.begin(),s.end());
  os<<s;
  return os;
}