#pragma once
#include<vector>
template<typename T>
struct KMP{
  std::vector<int>mp,kmp;
  std::vector<T>now;
  KMP():mp{-1},kmp{-1}{}
  void push_back(T x){
    int n=size();
    n=kmp[n];
    while(n>=0&&now[n]!=x)n=kmp[n];
    mp.push_back(n+1),kmp.push_back(n+1);
    now.push_back(x);
    n=size()-1;
    if(kmp[n]>=0&&now[kmp[n]]==x)kmp[n]=kmp[kmp[n]];
  }
  void pop_back(){
    mp.pop_back(),kmp.pop_back();
    now.pop_back();
    kmp.back()=mp.back();
  }
  int size()const{return now.size();}
};