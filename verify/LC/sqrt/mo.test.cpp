#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"
#include<iostream>
#include<algorithm>
#include<cassert>
#include "sqrt/mo.hpp"
#include "datastructure/bit.hpp"
#include "monoid/add.hpp"
int main(){
  int n,q;
  std::cin>>n>>q;
  std::vector<int>a(n);
  for(int&x:a)std::cin>>x;
  Mo mo(n);
  for(int i=0;i<q;i++){
    int l,r;
    std::cin>>l>>r;
    mo.add(l,r);
  }
  std::vector<int>z(a);
  std::sort(z.begin(),z.end()),z.erase(std::unique(z.begin(),z.end()),z.end());
  for(int&x:a)x=std::lower_bound(z.begin(),z.end(),x)-z.begin();
  BinaryIndexedTree<MonoidAdd<int>>BIT(z.size());
  int nl=0,nr=0;
  long long now=0;
  std::vector<long long>ans(q);
  auto add=[&](int l,int r){
    if(nr==l){
      for(int i=l;i<r;i++){
        now+=BIT.sum(a[i]+1,z.size());
        BIT.add(a[i],1);
      }
      nr=r;
    }
    else if(r==nl){
      for(int i=r-1;i>=l;i--){
        now+=BIT.sum(0,a[i]);
        BIT.add(a[i],1);
      }
      nl=l;
    }
    else assert(false);
  };
  auto del=[&](int l,int r){
    if(nr==r){
      for(int i=r-1;i>=l;i--){
        now-=BIT.sum(a[i]+1,z.size());
        BIT.add(a[i],-1);
      }
      nr=l;
    }
    else if(nl==l){
      for(int i=l;i<r;i++){
        now-=BIT.sum(0,a[i]);
        BIT.add(a[i],-1);
      }
      nl=r;
    }
    else assert(false);
  };
  auto out=[&](int i){
    ans[i]=now;
  };
  mo.build(add,del,out);
  for(int i=0;i<q;i++)std::cout<<ans[i]<<'\n';
}