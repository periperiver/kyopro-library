#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include "dp/egz.hpp"
#include "random/generator.hpp"
void test(int n){
  std::vector<int>a(n*2-1);
  for(int i=0;i<n*2-1;i++)a[i]=Random::range(n);
  std::vector<bool>ans=egz(n,a);
  assert((int)ans.size()==n*2-1);
  assert(std::count(ans.begin(),ans.end(),true)==n);
  int sum=0;
  for(int i=0;i<n*2-1;i++)if(ans[i]){
    sum+=a[i];
    if(sum>=n)sum-=n;
  }
  assert(sum==0);
}
int main(){
  for(int n=1;n<=1000;n++)test(n);
  for(int i=0;i<10;i++){
    int n=Random::range(1001,500000);
    test(n);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}