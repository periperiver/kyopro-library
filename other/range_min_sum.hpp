#pragma once
#include "../datastructure/static_range_min.hpp"
#include "../datastructure/bit.hpp"
#include "../monoid/add.hpp"
#include "pair_limits.hpp"
#include "count_sort.hpp"
#include<ranges>
#include<numeric>
template<typename T,typename T2>
std::vector<T2>range_min_sum(std::vector<T>a,std::vector<std::pair<int,int>>query){
  int n=a.size(),q=query.size();
  std::vector<T2>res(q);
  StaticRangeMin<std::pair<T,int>>s([](const std::vector<T>&a){
    std::vector<std::pair<T,int>>res(a.size());
    for(auto [i,j]:a|std::views::enumerate)res[i]=std::make_pair(j,i);
    return res;
  }(a));
  std::vector<std::pair<int,int>>lr(n);
  auto dfs=[&](auto self,int l,int r)->void{
    if(l<r){
      int m=s.min(l,r).second;
      lr[m]=std::make_pair(l,r);
      self(self,l,m);
      self(self,m+1,r);
    }
  };
  dfs(dfs,0,n);
  for(int i=0;i<q;i++){
    auto [l,r]=query[i];
    int m=s.min(l,r).second;
    if(lr[m].first<l&&r<lr[m].second)res[i]=T2(a[m])*(m-l+1)*(r-m);
  }
  {
    std::vector<int>ord=count_sort(n,n,[&](int i){return n-1-lr[i].first;});
    int j=0;
    BinaryIndexedTree<MonoidAdd<T2>>BIT(n);
    for(int i:count_sort(q,n,[&](int i){return n-1-query[i].first;})){
      auto [l,r]=query[i];
      while(j<n&&lr[ord[j]].first>=l){
        int id=ord[j++];
        T2 val=a[id];
        val*=id-lr[id].first+1;
        val*=lr[id].second-id;
        BIT.add(lr[id].second-1,val);
      }
      res[i]+=BIT.sum(l,r);
    }
  }
  {
    std::vector<int>ord=count_sort(n,n,[&](int i){return n-1-lr[i].first;});
    BinaryIndexedTree<MonoidAdd<T2>>BIT1(n),BIT2(n);
    int nl=n;
    int j=0;
    for(int i:count_sort(q,n,[&](int i){return n-1-query[i].first;})){
      auto [l,r]=query[i];
      while(l<nl){
        nl--;
        T2 val=a[nl];
        val*=lr[nl].second-nl;
        BIT1.add(lr[nl].second-1,val*(nl+1));
        BIT2.add(lr[nl].second-1,val);
      }
      while(j<n&&lr[ord[j]].first>=l){
        int id=ord[j++];
        T2 val=-a[id];
        val*=lr[id].second-id;
        BIT1.add(lr[id].second-1,val*(id+1));
        BIT2.add(lr[id].second-1,val);
      }
      res[i]+=BIT1.sum(l,r)-BIT2.sum(l,r)*l;
    }
  }
  {
    std::vector<int>ord=count_sort(n,n,[&](int i){return lr[i].second-1;});
    BinaryIndexedTree<MonoidAdd<T2>>BIT1(n),BIT2(n);
    int nr=0;
    int j=0;
    for(int i:count_sort(q,n,[&](int i){return query[i].second-1;})){
      auto [l,r]=query[i];
      while(nr<r){
        T2 val=a[nr];
        val*=nr-lr[nr].first+1;
        BIT1.add(lr[nr].first,val*nr);
        BIT2.add(lr[nr].first,val);
        nr++;
      }
      while(j<n&&lr[ord[j]].second<=r){
        int id=ord[j++];
        T2 val=-a[id];
        val*=id-lr[id].first+1;
        BIT1.add(lr[id].first,val*id);
        BIT2.add(lr[id].first,val);
      }
      res[i]+=-BIT1.sum(l,r)+BIT2.sum(l,r)*r;
    }
  }
  return res;
}