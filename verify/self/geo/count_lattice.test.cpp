#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<cassert>
#include "geo/count_lattice.hpp"
#include "random/generator.hpp"
long long naive(std::vector<std::tuple<long long,long long,long long>>lines,int lim){
  long long res=0;
  for(int i=-lim;i<=lim;i++)for(int j=-lim;j<=lim;j++){
    bool ok=true;
    for(auto [a,b,c]:lines){
      if(!(a*i+b*j<=c)){
        ok=false;
        break;
      }
    }
    res+=ok;
  }
  return res;
}
template<bool check_naive,bool exist_ans>
void test(int n,long long lim,long long abmax,long long cmax){
  std::vector<std::tuple<long long,long long,long long>>lines(n);
  long long x=Random::range(-lim,lim+1);
  long long y=Random::range(-lim,lim+1);
  for(int i=0;i<n;i++){
    long long a=Random::range(-abmax,abmax+1);
    long long b=Random::range(-abmax,abmax+1);
    long long c=Random::range(-cmax,cmax+1);
    if(exist_ans&&!(a*x+b*y<=c)){
      a=-a,b=-b,c=-c;
    }
    lines[i]={a,b,c};
  }
  lines.emplace_back(1,0,lim);
  lines.emplace_back(0,1,lim);
  lines.emplace_back(-1,0,lim);
  lines.emplace_back(0,-1,lim);
  long long ans=count_lattice<long long,__int128_t>(lines);
  if constexpr(check_naive){
    long long na=naive(lines,lim);
    assert(ans==na);
  }
}
int main(){
  for(int i=0;i<100;i++){
    int n=Random::range(20);
    test<true,false>(n,100,10,100);
    test<true,true>(n,100,10,100);
  }
  for(int i=0;i<100;i++){
    int n=Random::range(20);
    test<false,false>(n,1ll<<61,1ll<<31,1ll<<61);
    test<false,true>(n,1ll<<61,1ll<<31,1ll<<61);
  }
  for(int i=0;i<5;i++){
    int n=Random::range(100000);
    test<false,false>(n,1ll<<61,1ll<<31,1ll<<61);
    test<false,true>(n,1ll<<61,1ll<<31,1ll<<61);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}