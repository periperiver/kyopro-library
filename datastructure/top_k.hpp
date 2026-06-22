#pragma once
#include<array>
#include<limits>
//最小値k個
template<typename T,int k>
struct Topk{
private:
  static_assert(k>=1);
  std::array<T,k>v;
public:
  Topk(){
    v.fill(std::numeric_limits<T>::max());
  }
  Topk(T x){
    v.fill(std::numeric_limits<T>::max());
    v[0]=x;
  }
  void insert(T x){
    for(int i=0;i<k;i++){
      if(v[i]>x){
        for(int j=k-1;j>i;j--)v[j]=v[j-1];
        v[i]=x;
        return;
      }
    }
  }
  void insert(const Topk&rhs){
    for(int i=0;i<k;i++)insert(rhs.v[i]);
  }
  std::array<T,k> val()const{return v;}
  T& operator[](int x){return v[x];}
  const T& operator[](int x)const{return v[x];}
};