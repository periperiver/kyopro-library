#pragma once
#include<vector>
#include "factorial.hpp"
using namespace std;
template<typename T>
struct BinomialPrefixSum{
private:
  static vector<vector<T>>table;
  static constexpr int b=256;
  static const T inv2;
  static void resize(int n){
    if(table.size()>n)return;
    int pre=table.size();
    table.resize(n+1);
    for(int i=pre;i<=n;i++){
      table[i].resize(i+1);
      table[i][0]=1;
      int half=i/2;
      T pow2i=table[i][i]=T::raw(2).pow(i*b);
      for(int j=1;j<=half;j++){
        T now=table[i][j-1];
        for(int k=1;k<b;k++)now+=F<T>::C(i*b,b*(j-1)+k);
        table[i][i-j]=pow2i-now;
        table[i][j]=now+=F<T>::C(i*b,j*b);
      }
    }
  }
public:
  static T get(int n,int k){
    if(n<k)k=n;
    if(k<0)return 0;
    int sn=n/b,sk=k/b;
    int sn2=sn*b,sk2=sk*b;
    if(n-sn2<sn2+b-n){
      if(k-sk2<sk2+b-k){
        if(n-sn2+k-sk2<n-k){
          resize(sn);
          T now=table[sn][sk];
          for(int i=sn2;i<n;i++)now=now*2-F<T>::C(i,sk2);
          for(int i=sk2+1;i<=k;i++)now+=F<T>::C(n,i);
          return now;
        }
      }
      else if(sn2!=sk2){
        sk2+=b;
        sk++;
        resize(sn);
        T now=table[sn][sk];
        for(int i=sk2;i>k;i--)now-=F<T>::C(sn2,i);
        for(int i=sn2;i<n;i++)now=now*2-F<T>::C(i,k);
        return now;
      }
    }
    else{
      sn2+=b;
      sn++;
      if(k-sk2<sk2+b-k){
        if(sn2-n+k-sk2<n-k){
          resize(sn);
          T now=table[sn][sk];
          for(int i=sk2+1;i<=k;i++)now+=F<T>::C(sn2,i);
          for(int i=sn2-1;i>=n;i--)now=(now+F<T>::C(i,k))*inv2;
          return now;
        }
      }
      else{
        if(sn2-n+sk2-k<n-k){
          resize(sn);
          sk2+=b;
          sk++;
          T now=table[sn][sk];
          for(int i=sk2;i>k;i--)now-=F<T>::C(sn2,i);
          for(int i=sn2-1;i>=n;i--)now=(now+F<T>::C(i,k))*inv2;
          return now;
        }
      }
    }
    T now=T::raw(2).pow(n);
    for(int i=n;i>k;i--)now-=F<T>::C(n,i);
    return now;
  }
  static inline T get(int n,int k1,int k2){
    return get(n,k2)-get(n,k1-1);
  }
};
template<typename T>vector<vector<T>>BinomialPrefixSum<T>::table;
template<typename T>const T BinomialPrefixSum<T>::inv2(T(2).inv());