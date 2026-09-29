#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include "datastructure/lazy_segmenttree2d.hpp"
#include "monoid/add.hpp"
#include "monoid/xor.hpp"
#include "monoid/mul.hpp"
#include "math/modint.hpp"
#include "random/generator.hpp"
template<typename M>
void test(int h,int w,int q,auto lim){
  using S=typename M::S;
  std::vector<std::vector<S>>a(h,std::vector<S>(w,M::e()));
  LazySegmentTree2d<M>seg(h,w);
  while(q--){
    int t=Random::range(2);
    auto [lx,rx]=Random::distinct(h+1);
    auto [ly,ry]=Random::distinct(w+1);
    if(t==0){
      S v=Random::range<decltype(lim)>(1,lim);
      for(int i=lx;i<rx;i++)for(int j=ly;j<ry;j++)a[i][j]=M::op(a[i][j],v);
      seg.apply(lx,rx,ly,ry,v);
    }
    else{
      S naive=M::e();
      for(int i=lx;i<rx;i++)for(int j=ly;j<ry;j++)naive=M::op(naive,a[i][j]);
      assert(seg.prod(lx,rx,ly,ry)==naive);
    }
  }
}
int main(){
  for(int h=1;h<=20;h++)for(int w=1;w<=20;w++){
    for(int q:{1,2,3,10,100}){
      for(int t=0;t<20;t++){
        test<MonoidAdd<int>>(h,w,q,100);
        test<MonoidAdd<long long>>(h,w,q,1000000000);
        test<MonoidXor<int>>(h,w,q,1000000000);
        test<MonoidXor<long long>>(h,w,q,1000000000000000000);
        test<MonoidMul<mint998>>(h,w,q,mint998::mod());
        test<MonoidMul<mint61>>(h,w,q,mint61::mod());
      }
    }
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}