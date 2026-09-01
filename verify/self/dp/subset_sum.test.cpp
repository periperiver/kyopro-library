#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<bitset>
#include<cassert>
#include "dp/subset_sum.hpp"
#include "random/generator.hpp"
template<int N=1<<16>
bool check(std::vector<int>a,int s){
  if(N/2>s)return check<N/2>(std::move(a),s);
  std::bitset<N>bs;
  bs.set(0);
  for(int x:a)bs|=bs<<x;
  return bs[s];
}
void test(std::vector<int>a,int s){
  auto ans=subset_sum(a,s);
  if(ans.empty()){
    assert(!check(a,s));
  }
  else{
    int x=0;
    for(int i=0;i<(int)a.size();i++)if(ans[i])x+=a[i];
    assert(x==s);
  }
}
int main(){
  for(int n=1;n<=200;n++){
    for(int i=0;i<10;i++){
      for(int m:{10,20,100,200}){
        std::vector<int>a(n);
        for(int j=0;j<n;j++)a[j]=Random::range(m);
        int s=Random::range(std::reduce(a.begin(),a.end())+1);
        test(a,s);
      }
    }
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}