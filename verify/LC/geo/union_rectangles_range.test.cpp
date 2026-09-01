#define PROBLEM "https://judge.yosupo.jp/problem/area_of_union_of_rectangles"
#include<iostream>
#include "geo/union_rectangles_range.hpp"
int main(){
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);
  int n;
  std::cin>>n;
  std::vector<std::tuple<long long,long long,long long,long long>>a(n);
  for(auto&[lx,rx,ly,ry]:a){
    std::cin>>lx>>ly>>rx>>ry;
  }
  std::cout<<union_rectangles_range(a,1,n+1)<<std::endl;
}