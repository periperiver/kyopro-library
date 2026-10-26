#pragma once
#include<cassert>
#include "hash_map.hpp"
#include "../other/count_sort.hpp"
template<typename T>
std::vector<int>static_range_frequency(const std::vector<T>&a,const std::vector<std::tuple<int,int,T>>&query){
  int n=a.size(),q=query.size();
  std::vector<int>res(q);
  int r=0;
  HashMap<T,int>mp(n);
  for(int i:count_sort(q*2,n+1,[&](int j){return j<q?std::get<0>(query[j]):std::get<1>(query[j-q]);})){
    int p=i<q?std::get<0>(query[i]):std::get<1>(query[i-q]);
    while(r<p)mp[a[r++]]++;
    if(i<q)res[i]-=mp.get(std::get<2>(query[i]));
    else res[i-q]+=mp.get(std::get<2>(query[i-q]));
  }
  return res;
}