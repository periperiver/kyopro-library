#pragma once
#include "../convolution/ntt.hpp"
#include "multipoint_evaluation.hpp"
#include "polynomial_product.hpp"
#include "fps_inv.hpp"
#include<cassert>
template<typename T>
std::vector<T>multipoint_interpolate(std::vector<T>x,std::vector<T>y){
  assert(x.size()==y.size());
  int n=x.size();
  std::vector<T>g(n);
  for(int i=0;i<n;i++)g[i]=-x[i];
  g=polynomial_product1(g);
  std::reverse(g.begin(),g.end());
  std::vector<T>g1(n);
  for(int i=0;i<n;i++)g1[i]=g[i+1]*(i+1);
  g1=multipoint_evaluation(g1,x);
  std::vector<std::pair<std::vector<T>,std::vector<T>>>poly(n);
  for(int i=0;i<n;i++)poly[i]=std::make_pair(std::vector<T>{y[i]},std::vector<T>{-x[i]*g1[i],g1[i]});
  for(int i=n;--i;){
    std::vector<T>num1=ntt_convolution(poly[i/2].first,poly[i].second);
    std::vector<T>num2=ntt_convolution(poly[i/2].second,poly[i].first);
    std::vector<T>den=ntt_convolution(poly[i/2].second,poly[i].second);
    poly[i/2].first.assign(std::max(num1.size(),num2.size()),0);
    poly[i/2].second.assign(den.size(),0);
    for(int j=0;j<std::ssize(num1);j++)poly[i/2].first[j]+=num1[j];
    for(int j=0;j<std::ssize(num2);j++)poly[i/2].first[j]+=num2[j];
    for(int j=0;j<std::ssize(den);j++)poly[i/2].second[j]+=den[j];
  }
  int pos=0;
  while(pos<std::ssize(poly[0].second)&&poly[0].second[pos].val()==0)pos++;
  poly[0].second.erase(poly[0].second.begin(),poly[0].second.begin()+pos);
  g.erase(g.begin(),g.begin()+pos);
  poly[0].first=ntt_convolution(poly[0].first,fps_inv(poly[0].second));
  g=ntt_convolution(g,poly[0].first);
  g.resize(n);
  return g;
}