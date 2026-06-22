#pragma once
#include "multipoint_evaluation.hpp"
#include "polynomial_product.hpp"
//f/prod(1-ax)=sum(c/(1-ax))
template<typename T>
std::vector<T>partial_fraction_decomposition(std::vector<T>f,std::vector<T>a){
  assert(f.size()<=a.size());
  f.resize(a.size());
  std::vector<T>g(a);
  for(T&x:g)x=-x;
  g=polynomial_product1(std::move(g));
  std::reverse(f.begin(),f.end()),std::reverse(g.begin(),g.end());
  for(int i=0;i<std::ssize(g)-1;i++)g[i]=g[i+1]*T::raw(i+1);
  g.pop_back();
  f=multipoint_evaluation(std::move(f),a);
  g=multipoint_evaluation(std::move(g),a);
  for(int i=0;i<std::ssize(f);i++)f[i]/=g[i];
  return f;
}