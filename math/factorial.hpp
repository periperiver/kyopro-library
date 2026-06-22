#pragma once
#include<vector>
#include<initializer_list>
#include<cassert>
#include "other/type.hpp"
template<typename T>
struct F{
private:
  static int capacity;
  static std::vector<T>fact,factinv,inv;
public:
  static void resize(int n){
    if(capacity>=n)return;
    fact.resize(n+1),factinv.resize(n+1),inv.resize(n+1);
    for(int i=capacity+1;i<=n;i++){
      fact[i]=fact[i-1]*T::raw(i);
      if constexpr(is_uso_modint_v<T>)inv[i]=T(1)/T(i);
      else inv[i]=-inv[T::mod()%i]*(T::mod()/i);
      factinv[i]=factinv[i-1]*inv[i];
    }
    capacity=n;
  }
  static T C(int n,int k){
    if(n<k)return 0;
    if(k<0)return 0;
    resize(n);
    return fact[n]*factinv[k]*factinv[n-k];
  }
  static T P(int n,int k){
    if(n<k)return 0;
    if(k<0)return 0;
    resize(n);
    return fact[n]*factinv[n-k];
  }
  static T H(int n,int k){
    if(n==0&&k==0)return 1;
    return C(n+k-1,k);
  }
  static T factorial(int n){
    resize(n);
    return fact[n];
  }
  static T factorial_inv(int n){
    resize(n);
    return factinv[n];
  }
  static T inverse(int n){
    resize(n);
    return inv[n];
  }
  static T S(long long n,int k){
    if(n<0)return 0;
    if(n<k)return 0;
    T ret=0;
    resize(k);
    for(int i=0;i<=k;i++){
      ret+=fact[k]*factinv[i]*factinv[k-i]*T::raw(i).pow(n)*((k-i)&1?-1:1);
    }
    return ret*factinv[k];
  }
  template<typename... INT>
  static T O(INT...k){
    int n=0;
    for(int i:std::initializer_list<int>{k...}){
      if(i<0)return 0;
      n+=i;
    }
    resize(n);
    T ret=fact[n];
    for(int i:std::initializer_list<int>{k...})ret*=factinv[i];
    return ret;
  }
  static T rising_factorial(int x,int n){
    assert(n>=0);
    if(x>0){
      resize(x+n-1);
      return fact[x+n-1]*factinv[x-1];
    }
    else if(x+n>0)return T();
    else{
      resize(-x);
      T res=fact[-x]*factinv[-x-n];
      if(n&1)res=-res;
      return res;
    }
  }
  static T falling_factorial(int x,int n){return rising_factorial(x-n+1,n);}
};
template<typename T>int F<T>::capacity=1;
template<typename T>std::vector<T>F<T>::fact{1,1};
template<typename T>std::vector<T>F<T>::factinv{1,1};
template<typename T>std::vector<T>F<T>::inv{0,1};