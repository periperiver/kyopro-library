#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<cassert>
#include "datastructure/bit3d.hpp"
#include "monoid/add.hpp"
#include "random/generator.hpp"
void test(int n,int q,int lim){
  std::vector<std::tuple<int,int,int>>points;
  for(int i=0;i<n;i++)points.emplace_back(Random::range(lim),Random::range(lim),Random::range(lim));
  BinaryIndexedTree3d<int,MonoidAdd<int>>BIT(points);
  std::vector<std::tuple<int,int,int,int>>naive;
  while(q--){
    int t=Random::range(4);
    if(t==0){
      int id=Random::range(n);
      auto [x,y,z]=points[id];
      int val=Random::range(-lim,lim+1);
      naive.emplace_back(x,y,z,val);
      BIT.add(x,y,z,val);
    }
    else{
      auto [lx,rx]=Random::distinct(lim+1);
      auto [ly,ry]=Random::distinct(lim+1);
      auto [lz,rz]=Random::distinct(lim+1);
      int ans=BIT.sum(lx,rx,ly,ry,lz,rz);
      int na=0;
      for(auto [x,y,z,val]:naive){
        if(lx<=x&&x<rx){
          if(ly<=y&&y<ry){
            if(lz<=z&&z<rz){
              na+=val;
            }
          }
        }
      }
      assert(ans==na);
    }
  }
}
int main(){
  for(int n=1;n<=100;n++){
    for(int q:{1,2,5,10,100}){
      for(int t=0;t<10;t++){
        test(n,q,10);
        test(n,q,100000);
      }
    }
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}