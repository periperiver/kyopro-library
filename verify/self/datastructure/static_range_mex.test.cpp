#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<cassert>
#include "datastructure/static_range_mex.hpp"
#include "math/mex.hpp"
#include "random/generator.hpp"
void test(std::vector<int>a,std::vector<std::pair<int,int>>query){
  StaticRangeMex s(a);
  for(auto [l,r]:query){
    int ans=mex(std::vector<int>{a.begin()+l,a.begin()+r});
    assert(ans==s.mex(l,r));
  }
}
int main(){
  for(int t=0;t<10;t++){
    int n=Random::range(1,1000);
    std::vector<int>a(n);
    for(int i=0;i<n;i++){
      a[i]=Random::range(n+1);
      if(t%2==0){
        for(int j=0;j<20;j++)a[i]=std::min(a[i],Random::range(n+1));
      }
    }
    std::vector<std::pair<int,int>>query;
    for(int i=0;i<n;i++)for(int j=i;j<=n;j++)query.emplace_back(i,j);
    std::shuffle(query.begin(),query.end(),Random::mt);
    test(a,query);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}