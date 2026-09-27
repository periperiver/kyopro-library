#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<cassert>
#include "datastructure/static_rectangle_add_rectangle_sum.hpp"
#include "monoid/xor.hpp"
#include "monoid/mul.hpp"
#include "math/modint.hpp"
#include "random/generator.hpp"
template<typename M>
void test(int n,int q,int lim){
  using S=typename M::S;
  std::vector<std::vector<S>>a(lim,std::vector<S>(lim,M::e()));
  std::vector<std::tuple<int,int,int,int,S>>b;
  for(int i=0;i<n;i++){
    auto [lx,rx]=Random::distinct(lim+1);
    auto [ly,ry]=Random::distinct(lim+1);
    S val=Random::range(1,1<<20);
    b.emplace_back(lx,rx,ly,ry,val);
    for(int j=lx;j<rx;j++)for(int k=ly;k<ry;k++)a[j][k]=M::op(a[j][k],val);
  }
  std::vector<std::tuple<int,int,int,int>>query;
  std::vector<S>ans(q);
  for(int i=0;i<q;i++){
    auto [lx,rx]=Random::distinct(lim+1);
    auto [ly,ry]=Random::distinct(lim+1);
    query.emplace_back(lx,rx,ly,ry);
    S v=M::e();
    for(int j=lx;j<rx;j++)for(int k=ly;k<ry;k++)v=M::op(v,a[j][k]);
    ans[i]=v;
  }
  assert((static_rectangle_add_rectangle_sum<int,M>(b,query)==ans));
}
int main(){
  for(int n=1;n<=20;n++){
    for(int q:{1,2,3,10,100}){
      for(int t=0;t<10;t++){
        test<MonoidXor<int>>(n,q,10);
        test<MonoidXor<int>>(n,q,200);
        test<MonoidXor<long long>>(n,q,10);
        test<MonoidXor<long long>>(n,q,200);
        test<MonoidMul<mint998>>(n,q,10);
        test<MonoidMul<mint998>>(n,q,200);
        test<MonoidMul<mint61>>(n,q,10);
        test<MonoidMul<mint61>>(n,q,200);
      }
    }
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}