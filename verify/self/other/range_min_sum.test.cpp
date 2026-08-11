#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<cassert>
#include "other/range_min_sum.hpp"
#include "random/generator.hpp"
std::vector<int>gen_a(int n,int lim){
  std::vector<int>res(n);
  for(int i=0;i<n;i++)res[i]=Random::range(-lim,lim+1);
  return res;
}
std::vector<std::pair<int,int>>gen_q(int n,int q){
  std::vector<std::pair<int,int>>query(q);
  for(int i=0;i<q;i++)query[i]=Random::distinct(n+1);
  return query;
}
std::vector<long long>naive(std::vector<int>a,std::vector<std::pair<int,int>>query){
  std::vector<long long>res;
  for(auto [l,r]:query){
    long long sum=0;
    for(int i=l;i<r;i++){
      int now=1e9;
      for(int j=i;j<r;j++){
        if(now>a[j])now=a[j];
        sum+=now;
      }
    }
    res.push_back(sum);
  }
  return res;
}
void test(std::vector<int>a,std::vector<std::pair<int,int>>query){
  auto ans=range_min_sum<int,long long>(a,query);
  auto na=naive(a,query);
  assert(ans==na);
}
int main(){
  for(int n=1;n<=10;n++){
    auto a=gen_a(n,5);
    auto query=gen_q(n,n);
    test(a,query);
  }
  for(int t=0;t<100;t++){
    int n=Random::range(10,500);
    int q=Random::range(1,2000);
    test(gen_a(n,10),gen_q(n,q));
  }
  for(int t=0;t<100;t++){
    int n=Random::range(10,500);
    int q=Random::range(1,2000);
    test(gen_a(n,10000000),gen_q(n,q));
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}