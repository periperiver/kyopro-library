#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "graph/path_matching.hpp"
#include<iostream>
#include<cassert>
#include "random/generator.hpp"
template<typename T,bool MIN>
std::vector<T>naive(std::vector<T>a){
  static constexpr T inf=std::numeric_limits<T>::max()/2;
  auto choice=[](T&a,T b){
    if constexpr(MIN){
      if(a>b)a=b;
    }
    else{
      if(a<b)a=b;
    }
  };
  std::vector<std::vector<T>>dp(a.size()+1);
  dp[0]={0};
  dp[1]={0};
  for(int i=2;i<=a.size();i++){
    dp[i].resize(dp[i-2].size()+1,MIN?inf:-inf);
    for(int j=0;j<(int)dp[i].size();j++){
      if(j<(int)dp[i-1].size())choice(dp[i][j],dp[i-1][j]);
      if(j-1>=0)choice(dp[i][j],dp[i-2][j-1]+a[i-2]+a[i-1]);
    }
  }
  return dp.back();
}
template<typename T,bool MIN>
void test(std::vector<T>a){
  assert((path_matching<T,MIN>(a)==naive<T,MIN>(a)));
}
template<typename T>
std::vector<T>gen(int n,int l,int r){
  std::vector<T>res(n);
  for(int i=0;i<n;i++)res[i]=Random::range<T>(l,r+1);
  return res;
}
int main(){
  for(int n=1;n<=300;n++){
    for(int t=0;t<20;t++){
      test<int,true>(gen<int>(n,0,10));
      test<int,true>(gen<int>(n,-10,0));
      test<int,true>(gen<int>(n,-10,10));
      test<int,false>(gen<int>(n,0,10));
      test<int,false>(gen<int>(n,-10,0));
      test<int,false>(gen<int>(n,-10,10));
      test<long long,true>(gen<long long>(n,0,1000000000));
      test<long long,true>(gen<long long>(n,-1000000000,0));
      test<long long,true>(gen<long long>(n,-1000000000,1000000000));
      test<long long,false>(gen<long long>(n,0,1000000000));
      test<long long,false>(gen<long long>(n,-1000000000,0));
      test<long long,false>(gen<long long>(n,-1000000000,1000000000));
    }
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}