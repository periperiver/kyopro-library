#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<cassert>
#include "string/minimum_rotation.hpp"
#include "random/generator.hpp"
using Solver=MinimumRotation;
struct Naive{
  std::string s;
  Naive(std::string s):s(std::move(s)){}
  int query(int l,int r){
    std::string t=s.substr(l,r-l);
    int id=0;
    std::string best=t;
    t=t+t;
    for(int i=1;i<r-l;i++){
      std::string now=t.substr(i,r-l);
      if(best>now)id=i,best=now;
    }
    return id;
  }
};
std::string gen(int alpha){
  std::string res;
  if(alpha==26){
    for(int i=0;i<26;i++)res+='a'+i;
    return res;
  }
  for(int i=0;i<alpha;i++)res+='a'+Random::range(26);
}
void test(int n,int q,int alpha){
  assert(1<=alpha&&1<=alpha);
  std::string t=gen(alpha);
  std::string s(n,'?');
  for(int i=0;i<n;i++)s[i]=t[Random::range(alpha)];
  Solver solver(s);
  Naive na(s);
  for(int i=0;i<q;i++){
    auto [l,r]=Random::distinct(n+1);
    assert(solver.query(l,r)==na.query(l,r));
  }
}
int main(){
  for(int i=0;i<10;i++){
    int n=Random::range(1,100);
    int q=Random::range(1,100);
    test(n,q,2);
    test(n,q,3);
    test(n,q,26);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}