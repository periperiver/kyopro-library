#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<cassert>
#include<algorithm>
#include "geo/dynamic_cht.hpp"
#include "random/generator.hpp"
using Solver=DynamicConvexHullTrick<long long,__int128_t>;
struct Naive{
  std::vector<std::tuple<long long,long long,int>>lines;
  std::pair<long long,int>eval(int i,long long x){
    auto [a,b,id]=lines[i];
    return std::make_pair(a*x+b,id);
  }
  void add(long long a,long long b,int idx){lines.emplace_back(a,b,idx);}
  bool erase(long long a,long long b,int idx){
    auto itr=std::find(lines.begin(),lines.end(),std::make_tuple(a,b,idx));
    if(itr==lines.end())return false;
    lines.erase(itr);
    return true;
  }
  std::tuple<long long,long long,int>min(long long x){
    if(lines.empty())return std::make_tuple(0,std::numeric_limits<long long>::max(),-1);
    int id=0;
    for(int i=1;i<(int)lines.size();i++){
      if(eval(id,x)>eval(i,x))id=i;
    }
    return  lines[id];
  }
};
void test(long long axmax,long long bmax){
  int q=Random::range(2,1000);
  Solver solver;
  Naive naive;
  for(int i=0;i<q;i++){
    int type=Random::range(3);
    if(type==0){
      long long a=Random::range(-axmax,axmax+1);
      long long b=Random::range(-bmax,bmax+1);
      solver.add(a,b,i);
      naive.add(a,b,i);
    }
    else if(type==1){
      long long a,b;
      int id;
      if(naive.lines.empty()||Random::range(3)){
        a=Random::range(-axmax,axmax+1);
        b=Random::range(-bmax,bmax+1);
        id=Random::range(q);
      }
      else{
        std::tie(a,b,id)=naive.lines[Random::range(naive.lines.size())];
      }
      solver.erase(a,b,id);
      naive.erase(a,b,id);
    }
    else{
      long long x=Random::range(-axmax,axmax+1);
      auto [ans_a,ans_b,ans_id]=solver.min(x);
      auto [na_a,na_b,na_id]=naive.min(x);
      assert(ans_a==na_a);
      assert(ans_b==na_b);
      assert(ans_id==na_id);
    }
  }
}
int main(){
  for(int i=0;i<20;i++)test(10,100);
  for(int i=0;i<20;i++)test(1ll<<31,1ll<<61);
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}