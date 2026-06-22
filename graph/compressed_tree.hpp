#pragma once
#include<numeric>
#include "tree_base.hpp"
template<typename T=int>
struct CompressedTree:Tree<T>{
private:
  std::vector<int>mapping;
public:
  CompressedTree(const std::vector<int>&mapping_,const Tree<T>&t_):mapping(mapping_),Tree<T>(t_){}
  CompressedTree(const Tree<T>&t_):mapping(t_.size()),Tree<T>(t_){
    iota(mapping.begin(),mapping.end(),0);
  }
  CompressedTree():Tree<T>(){}
  inline int get(int u)const{return mapping[u];}
};