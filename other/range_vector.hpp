#pragma once
#include<iostream>
#include<vector>
template<typename T,typename I=int>
struct RangeVector:public std::vector<T> {
private:
  I shift;
public:
  RangeVector():shift(0){}
  RangeVector(I l,I r):std::vector<T>(r-l),shift(l){}
  RangeVector(I l,I r,T v):std::vector<T>(r-l,v),shift(l){}
  T& operator[](I i){return std::vector<T>::operator[](i-shift);}
  const T& operator[](I i)const{return std::vector<T>::operator[](i-shift);}
  friend std::ostream &operator<<(std::ostream&os,const RangeVector&v){
    os<<"["<<v.shift<<","<<v.size()<<"){";
    if(v.empty()){
      os<<'}';
      return os;
    }
    os<<*v.begin();
    for(auto itr=v.begin()+1;itr!=v.end();++itr)os<<','<<*itr;
    os<<'}';
    return os;
  }
};