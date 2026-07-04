#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include "math/crt.hpp"
#include "random/generator.hpp"
template<typename T,bool check>
void test(std::vector<std::pair<T,T>>a){
  auto [r,m]=crt(a);
  if(m==-1){
    if constexpr(!check)return;
    m=1;
    for(int i=0;i<(int)a.size();i++)m=std::lcm(m,a[i].second);
    bool found=false;
    for(T i=0;i<m;i++){
      bool ok=true;
      for(auto [r1,m1]:a){
        if(i%m1!=r1){
          ok=false;
          break;
        }
      }
      if(ok){
        found=true;
        break;
      }
    }
    assert(!found);
  }
  else{
    for(auto [r1,m1]:a)assert(r%m1==r1);
    T m2=1;
    for(auto [r1,m1]:a)m2=std::lcm(m2,m1);
    assert(m==m2);
  }
}
int main(){
  for(int i=0;i<80;i++)for(int j=i+1;j<=80;j++){
    for(int k=0;k<80;k++)for(int l=k+1;l<=80;l++){
      test<int,true>({{i,j},{k,l}});
    }
  }
  for(int i=0;i<10000;i++){
    long long t=Random::range(100000000000000000ll);
    std::vector<std::pair<long long,long long>>a;
    int cnt=Random::range(2,8);
    for(int j=0;j<cnt;j++){
      int mod=Random::range(1,1000);
      a.emplace_back(t%mod,mod);
    }
    test<long long,false>(a);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}