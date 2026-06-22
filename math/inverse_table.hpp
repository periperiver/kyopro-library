#pragma once
#include<vector>
#include<cassert>
template<typename T>
std::vector<T>inverse_table(long long l,long long r){
  assert(l<=r);
  if(l==r)return std::vector<T>{};
  l=(l-1)%T::mod();
  r=(r-1)%T::mod();
  if(l<0)l+=T::mod();
  if(r<0)r+=T::mod();
  if(l>r){
    int n=std::max(r,T::mod()-l);
    std::vector<T>fact(n+1),factinv(n+1);
    fact[0]=1;
    for(int i=1;i<=n;i++)fact[i]=fact[i-1]*T::raw(i);
    factinv[n]=fact[n].inv();
    for(int i=n-1;i>=0;i--)factinv[i]=factinv[i+1]*T::raw(i+1);
    std::vector<T>ret;
    ret.reserve(r-l+T::mod());
    for(int i=l+1;i<T::mod();i++)ret.push_back(-fact[T::mod()-i-1]*factinv[T::mod()-i]);
    ret.push_back(0);
    for(int i=1;i<=r;i++)ret.push_back(fact[i-1]*factinv[i]);
    return ret;
  }
  else{
    int n=r-l;
    std::vector<T>ret(n);
    ret[0]=1;
    for(int i=1;i<n;i++)ret[i]=ret[i-1]*T::raw(l+i);
    T inv=(ret[n-1]*(n+l)).inv();
    for(int i=n-1;i>=0;i--){
      ret[i]*=inv;
      inv*=T::raw(i+1+l);
    }
    return ret;
  }
}