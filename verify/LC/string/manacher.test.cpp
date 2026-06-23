#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_palindromes"
#include<iostream>
#include "string/manacher.hpp"
int main(){
  std::string s;
  std::cin>>s;
  std::string t;
  for(int i=0;i<(int)s.size();i++)t+=s[i],t+='*';
  t.pop_back();
  std::vector<int>man=manacher(t);
  for(int i=0;i<(int)man.size();i++){
    if(i%2==0)std::cout<<(man[i]-1)/2*2+1<<" \n"[i+1==(int)man.size()];
    else std::cout<<man[i]/2*2<<' ';
  }
}