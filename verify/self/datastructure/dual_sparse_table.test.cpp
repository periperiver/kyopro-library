#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<cassert>
#include "datastructure/dual_sparse_table.hpp"
#include "monoid/min.hpp"
#include "random/generator.hpp"
std::vector<std::tuple<int,int,int>>gen(int n,int q){
  std::vector<std::tuple<int,int,int>>res(q);
  for(int i=0;i<q;i++){
    auto [l,r]=Random::distinct(n+1);
    res[i]={l,r,Random::range(1000000000)};
  }
  return res;
}
void test(int n,std::vector<std::tuple<int,int,int>>query){
  auto ans=dual_sparse_table<MonoidMin<int>>(n,query);
  std::vector<int>a(n,std::numeric_limits<int>::max());
  for(auto [l,r,x]:query)for(int i=l;i<r;i++)a[i]=std::min(a[i],x);
  assert(ans==a);
}
int main(){
  for(int n=1;n<=100;n++){
    for(int q=1;q<=100;q++){
      test(n,gen(n,q));
    }
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}