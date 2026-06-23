#define PROBLEM "https://judge.yosupo.jp/problem/range_add_range_min"
#include<iostream>
#include "datastructure/rangeadd_rangemin.hpp"
int main(){
  std::cin.tie(nullptr)->sync_with_stdio(false);
  int n,q;
  std::cin>>n>>q;
  std::vector<long long>a(n);
  for(long long&x:a)std::cin>>x;
  RangeAddRangeMin<long long>seg(a);
  while(q--){
    int t,l,r;
    std::cin>>t>>l>>r;
    if(t==0){
      long long x;
      std::cin>>x;
      seg.add(l,r,x);
    }
    else{
      std::cout<<seg.min(l,r)<<'\n';
    }
  }
}