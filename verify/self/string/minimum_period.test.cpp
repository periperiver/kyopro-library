#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<cassert>
#include "string/minimum_period.hpp"
#include "random/generator.hpp"
std::vector<int>naive(std::string s){
  int n=s.size();
  std::vector<int>res(n);
  for(int i=0;i<n;i++){
    std::string t=s.substr(0,i+1);
    int now=i+1;
    for(int j=1;j<t.size();j++)if(t.size()%j==0){
      bool f=true;
      for(int k=j;k<t.size();k+=j)f&=t.substr(k,j)==t.substr(0,j);
      if(f){
        now=j;
        break;
      }
    }
    res[i]=now;
  }
  return res;
}
void test(std::string s){
  assert(minimum_period(s)==naive(s));
}
std::string gen(int n,int sigma){
  std::string res;
  for(int i=0;i<n;i++)res+='a'+Random::range(sigma);
  return res;
}
std::string gen2(int loop,int rep){
  std::string res=""+char('a'+Random::range(26));
  for(int i=0;i<loop;i++){
    int r=Random::range(1,rep+1);
    std::string nres;
    while(r--)nres+=res;
    res=std::move(nres);
    res+='a'+Random::range(26);
  }
  return res;
}
int main(){
  for(int n=1;n<=100;n++){
    for(int sigma:{1,2,3,4,10,26}){
      for(int i=0;i<10;i++)test(gen(n,sigma));
    }
  }
  for(int i=1;i<6;i++){
    for(int j=2;j<6;j++){
      test(gen2(i,j));
    }
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}