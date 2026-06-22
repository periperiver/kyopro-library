#pragma once
#include<memory>
template<typename T>
struct fast_stack{
private:
  T *st;
  int p;
public:
  fast_stack(int n):p(0){
    st=new T[n];
  }
  fast_stack(){}
  inline void push(const T&x){st[p++]=x;}
  template<typename...Args>
  inline T& emplace(Args&&...args){
    st[p++]=T(std::forward<Args>(args)...);
    return st[p-1];
  }
  inline T& pop(){return st[--p];}
  inline T top()const{return st[p-1];}
  inline T& top(){return st[p-1];}
  inline int size()const{return p;}
  inline bool empty()const{return !p;}
  inline void clear(){p=0;}
  ~fast_stack(){delete[] st;}
};