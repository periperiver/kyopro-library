#pragma once
#include<iostream>
template<typename T=int>
struct Edge{
  int from,to;
  T weight;
  int index;
  Edge(int from_,int to_,T weight_=T(),int index_=-1):from(from_),to(to_),weight(weight_),index(index_){}
  Edge():from(-1),to(-1),weight(),index(-1){}
  friend std::ostream &operator<<(std::ostream &os,const Edge&e){
    os<<'[';
    os<<"from:"<<e.from;
    os<<"to:"<<e.to;
    os<<"weight:"<<e.weight;
    os<<"index:"<<e.index;
    os<<']';
    return os;
  }
};