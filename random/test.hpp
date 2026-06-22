#pragma once
#include "generator.hpp"
#include "parentheses.hpp"
#include "permutation.hpp"
#include "prime.hpp"
#include "tree.hpp"
#include "sum_bounded_vector.hpp"
#include "graph.hpp"
#include<iostream>
#include<set>
namespace Random{
//l≤A_i<r
template<typename T>
std::vector<T>vector(int n,T l,T r){
  std::vector<T>res(n);
  for(int i=0;i<n;i++)res[i]=range<T>(l,r);
  return res;
}
//l≤A_i<r
template<typename T>
std::vector<T>distinct_vector(int n,T l,T r){
  assert(n<=r-l);
  std::set<T>s;
  while((int)s.size()<n)s.insert(range(l,r));
  std::vector<T>res(s.begin(),s.end());
  std::shuffle(res.begin(),res.end(),mt);
  return res;
}
std::string string(int n){
  std::string res;
  res.reserve(n);
  for(int i=0;i<n;i++)res+='a'+range<int>(26);
  return res;
}
std::string STRING(int n){
  std::string res;
  res.reserve(n);
  for(int i=0;i<n;i++)res+='A'+range<int>(26);
  return res;
}
}
#ifndef INTERACTIVE
template<typename T>
void test(T x){
  std::cout<<x<<'\n';
}
template<typename T,typename ...U>
void test(T x,U...y){
  std::cout<<x<<' ';
  test(y...);
}
template<typename T>
void test(const std::vector<T>&v){
  if(v.empty())std::cout<<'\n';
  else{
    std::cout<<v[0];
    for(int i=1;i<(int)v.size();i++)std::cout<<' '<<v[i];
    std::cout<<'\n';
  }
}
template<typename T1,typename T2>
void test(const std::pair<T1,T2>&v){
  std::cout<<v.first<<' '<<v.second<<'\n';
}
template<typename T1,typename T2,typename T3>
void test(const std::tuple<T1,T2,T3>&v){
  std::cout<<std::get<0>(v)<<' '<<std::get<1>(v)<<' '<<std::get<2>(v)<<'\n';
}
#endif