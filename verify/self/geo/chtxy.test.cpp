#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "geo/chtxy.hpp"
#include "random/generator.hpp"
#include<vector>
#include<cassert>
#include "io.hpp"
void test(int q,long long lim){
  CHTxy<long long>cht;
  std::vector<Point<long long>>a;
  std::vector<std::tuple<int,long long,long long>>his;
  while(q--){
    int t=Random::range(3);
    assert(a.empty()==cht.empty());
    if(a.empty()){
      t=0;
    }
    if(t==0){
      Point<long long>p;
      p.x=Random::range(-lim,lim+1);
      p.y=Random::range(-lim,lim+1);
      a.emplace_back(p);
      cht.add(p);
      his.emplace_back(0,p.x,p.y);
    }
    else if(t==1){
      Point<long long>p;
      p.x=Random::range(-lim,lim+1);
      p.y=Random::range(-lim,lim+1);
      long long ans=dot(cht.min(p),p);
      long long na=9e18;
      for(Point<long long>q:a)na=std::min(na,dot(p,q));
      his.emplace_back(1,p.x,p.y);
      assert(ans==na);
    }
    else{
      Point<long long>p;
      p.x=Random::range(-lim,lim+1);
      p.y=Random::range(-lim,lim+1);
      long long ans=dot(cht.max(p),p);
      long long na=-9e18;
      for(Point<long long>q:a)na=std::max(na,dot(p,q));
      his.emplace_back(2,p.x,p.y);
      assert(ans==na);
    }
  }
}
int main(){
  for(int t=0;t<10000;t++)test(50,20);
  for(int t=0;t<1000;t++)test(500,20);
  for(int t=0;t<100;t++)test(5000,20);
  for(int t=0;t<10000;t++)test(50,1000000000);
  for(int t=0;t<1000;t++)test(500,1000000000);
  for(int t=0;t<100;t++)test(5000,1000000000);
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}