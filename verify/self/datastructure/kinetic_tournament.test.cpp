#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include "datastructure/kinetic_tournament.hpp"
#include "random/generator.hpp"
#include<algorithm>
using Solver=KineticTournament<long long>;
struct Naive{
  std::vector<std::pair<long long,long long>>dat;
  Naive(std::vector<std::pair<long long,long long>>init):dat(std::move(init)){}
  void set(int i,long long a,long long b){dat[i]={a,b};}
  long long min(int l,int r,long long x){
    long long res=std::numeric_limits<long long>::max();
    for(int i=l;i<r;i++){
      auto [a,b]=dat[i];
      long long v=a*x+b;
      if(res>v)res=v;
    }
    return res;
  }
};
void test(){
  int n=Random::range(1,1000);
  int q=Random::range(1,1000);
  std::vector<std::pair<long long,long long>>init(n);
  for(int i=0;i<n;i++){
    init[i].first=Random::range(-1ll<<31,1ll<<31);
    init[i].second=Random::range(-1ll<<61,1ll<<61);
  }
  std::vector<long long>time(q);
  for(int i=0;i<q;i++)time[i]=Random::range(-1ll<<31,1ll<<31);
  std::sort(time.begin(),time.end());
  Solver solver(init,time[0]);
  Naive naive(init);
  for(int i=0;i<q;i++){
    int type=Random::range(2);
    if(type==0){
      int id=Random::range(n);
      long long a=Random::range(-1ll<<31,1ll<<31);
      long long b=Random::range(-1ll<<61,1ll<<61);
      solver.set(id,a,b);
      naive.set(id,a,b);
    }
    else{
      auto [l,r]=Random::distinct(n+1);
      long long ans=solver.min(l,r,time[i]);
      long long na=naive.min(l,r,time[i]);
      assert(ans==na);
    }
  }
}
int main(){
  for(int i=0;i<10;i++)test();
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}