#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "math/count_hook.hpp"
#include<iostream>
#include<cassert>
#include<algorithm>
#include "math/modint.hpp"
#include "math/factorial.hpp"
#include "random/generator.hpp"
using mint1=mint998;
using mint2=modint<167772161>;
template<typename T>
T naive(std::vector<int>a){
  if(a.empty())return 1;
  int sum=0;
  int n=a.size();
  T res=1;
  std::vector<int>down(a[0]);
  for(int i=n-1;i>=0;i--){
    sum+=a[i];
    for(int j=0;j<a[i];j++){
      res*=down[j]+a[i]-j;
      down[j]++;
    }
  }
  res=F<T>::factorial(sum)/res;
  return res;
}
template<typename T>
void test(std::vector<int>a){
  assert(count_hook<T>(a)==naive<T>(a));
}
int main(){
  for(int n=0;n<=100;n++){
    for(int t=0;t<40;t++){
      std::vector<int>a(n);
      for(int j=0;j<n;j++)a[j]=Random::range(1,100);
      std::sort(a.begin(),a.end(),std::greater<int>());
      test<mint1>(a);
      test<mint2>(a);
    }
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}