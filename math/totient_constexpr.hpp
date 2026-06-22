#pragma once
constexpr unsigned int totient_constexpr(unsigned int n){
  if(n==998244353)return 998244352;
  if(n==1000000007)return 1000000006;
  unsigned int res=n;
  for(unsigned int i=2;i*i<=n;i++)if(n%i==0){
    res=res/i*(i-1);
    while(n%i==0)n/=i;
  }
  if(n!=1)res=res/n*(n-1);
  return res;
}