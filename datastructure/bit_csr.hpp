#pragma once
#include<vector>
#include<algorithm>
#include<type_traits>
#include<cstdint>
#include<cassert>
template<typename T,int sigma>
struct bit_csr{
  static_assert(1<=sigma&&sigma<=64);
  using Iterator=typename std::vector<int>::iterator;
  using mask_type=std::conditional_t<(sigma<=32),std::uint32_t,std::uint64_t>;
  struct node:T{
    Iterator head;
    mask_type mask;
    node(){}
    node(T x):T(std::move(x)),mask(0){}
    int operator[](int x)const{
      return mask>>x&1?head[std::popcount(mask&((mask_type(1)<<x)-1))]:-1;
    }
  };
private:
  std::vector<node>nds;
  std::vector<int>child;
  template<int x>
  static void inplace_stable_sort(std::vector<std::tuple<int,int,int>>&a,int lim){
    int n=a.size();
    std::vector<int>cnt(lim);
    for(int i=0;i<n;i++)cnt[std::get<x>(a[i])]++;
    for(int i=1;i<lim;i++)cnt[i]+=cnt[i-1];
    std::vector<std::tuple<int,int,int>>b(n);
    for(int i=n-1;i>=0;i--)b[--cnt[std::get<x>(a[i])]]=std::move(a[i]);
    a=std::move(b);
  }
public:
  bit_csr(){}
  bit_csr(std::vector<T>a,std::vector<std::tuple<int,int,int>>edge):nds(a.size()),child(edge.size()){
    int n=a.size();
    assert(std::all_of(edge.begin(),edge.end(),[&](const std::tuple<int,int,int>&x){
      const auto&[u,e,to]=x;
      return 0<=u&&u<n&&0<=e&&e<sigma&&0<=to&&to<n;
    }));
    for(int i=0;i<n;i++)nds[i]=node(std::move(a[i]));
    inplace_stable_sort<1>(edge,sigma);
    inplace_stable_sort<0>(edge,n);
    child.clear();
    for(const auto&[u,e,to]:edge){
      nds[u].mask|=mask_type(1)<<e;
      child.push_back(to);
    }
    int cnt=0;
    for(int i=0;i<n;i++){
      nds[i].head=child.begin()+cnt;
      cnt+=std::popcount(nds[i].mask);
    }
  }
  node& operator[](int x){return nds[x];}
  const node& operator[](int x)const{return nds[x];}
  int size()const{return nds.size();}
};