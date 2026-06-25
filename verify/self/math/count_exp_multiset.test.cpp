#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<cassert>
#include<map>
#include "math/count_exp_multiset.hpp"
#include "math/lpf_table.hpp"
#include "random/generator.hpp"
static constexpr int naive_max=1000000;
std::vector<std::pair<std::vector<int>,unsigned long long>>naive(int n){
  static std::vector<int>lpf=lpf_table(naive_max);
  static int pre=0;
  static std::map<std::vector<int>,unsigned long long>mp;
  assert(pre<=n);
  auto add=[&](int v)->void {
    std::vector<int>now;
    while(v>1){
      int p=lpf[v];
      int e=0;
      do v/=p,e++;while(v%p==0);
      now.push_back(e);
    }
    std::sort(now.begin(),now.end(),std::greater<int>());
    mp[now]++;
  };
  while(pre<n)add(++pre);
  return std::vector<std::pair<std::vector<int>,unsigned long long>>(mp.begin(),mp.end());
}
void test(){
  for(int n=1;n<=100;n++){
    assert(count_exp_multiset(n)==naive(n));
  }
  std::vector<long long>pos;
  for(int i=0;i<20;i++)pos.push_back(Random::range(101,naive_max+1));
  std::sort(pos.begin(),pos.end());
  for(long long n:pos)assert(count_exp_multiset(n)==naive(n));
}
int main(){
  test();
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}