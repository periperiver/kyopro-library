#pragma once
#include<algorithm>
#include<cassert>
#include "../matrix/matrix_rank.hpp"
#include "../random/generator.hpp"
template<typename T>
int linear_matroid_parity(std::vector<std::pair<std::vector<T>,std::vector<T>>>a){
  if(a.empty())return 0;
  assert(std::all_of(a.begin(),a.end(),[&](const std::pair<std::vector<T>,std::vector<T>>&p){return p.first.size()==a[0].first.size()&&p.second.size()==a[0].first.size();}));
  int n=a[0].first.size();
  std::vector<std::vector<T>>mat(n,std::vector<T>(n));
  for(const auto&[u,v]:a){
    T x=Random::range(T::mod());
    std::vector<std::pair<int,T>>l,r;
    for(int i=0;i<n;i++){
      if(u[i].val())l.emplace_back(i,u[i]);
      if(v[i].val())r.emplace_back(i,v[i]);
    }
    for(auto [i,u2]:l)for(auto [j,v2]:r){
      mat[i][j]+=u2*v2*x;
      mat[j][i]-=u2*v2*x;
    }
  }
  return matrix_rank(mat)/2;
}