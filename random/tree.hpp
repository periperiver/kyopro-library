#pragma once
#include "../graph/prufer.hpp"
#include "generator.hpp"
namespace Random{
Tree<>tree(int n){
  if(n<=1){
    Tree<>t(n);
    t.build();
    return t;
  }
  std::vector<int>c(n-2);
  for(int i=0;i<n-2;i++)c[i]=range(n);
  return prufer(c);
}
}