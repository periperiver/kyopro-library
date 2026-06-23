#define PROBLEM "https://judge.yosupo.jp/problem/bitwise_xor_convolution"
#include<iostream>
#include "convolution/arbitrary_xor_convolution.hpp"
constexpr int mod=998244353;
struct mint{
  int v;
  mint():v(0){}
  mint(int v):v(v){}
  mint operator+(mint r)const{
    mint res;
    res.v=v+r.v;
    if(res.v>=mod)res.v-=mod;
    return res;
  }
  mint operator-(mint r)const{
    mint res;
    res.v=v-r.v;
    if(res.v<0)res.v+=mod;
    return res;
  }
  mint operator*(mint r)const{
    return v*r.v%mod;
  }
};
int main(){
  int n;
  std::cin>>n;
  std::vector<mint>a(1<<n),b(1<<n);
  for(mint&x:a)std::cin>>x.v;
  for(mint&x:b)std::cin>>x.v;
  a=arbitrary_xor_convolution(a,b);
  for(int i=0;i<(1<<n);i++)std::cout<<a[i].v<<" \n"[i+1==(1<<n)];
}