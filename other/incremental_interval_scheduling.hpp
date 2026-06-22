#pragma once
#include<vector>
#include<algorithm>
#include "datastructure/dynamic/linkcuttree.hpp"
#include "datastructure/veb_tree.hpp"
template<typename T>
std::vector<int>incremental_interval_scheduling(const std::vector<std::pair<T,T>>&a){
  struct node{
    node *left=nullptr,*right=nullptr,*par=nullptr;
    int v=0,sum=0;
    void update(){
      sum=v;
      if(left)sum+=left->sum;
      if(right)sum+=right->sum;
    }
  };
  std::vector<T>z;
  z.reserve(a.size()*2);
  for(const auto&[l,r]:a)z.push_back(l),z.push_back(r);
  std::sort(z.begin(),z.end());
  z.erase(std::unique(z.begin(),z.end()),z.end());
  std::vector<node>nds(z.size()+1);
  for(int i=0;i<(int)nds.size()-1;i++)nds[i].par=&nds[i+1];
  VanEmdeBoasTree veb(z.size()+1);
  std::vector<int>right(z.size()+1);
  std::vector<int>res(a.size());
  veb.insert(0);
  int base=0;
  for(int i=0;i<(int)a.size();i++){
    int l=(int)(std::lower_bound(z.begin(),z.end(),a[i].first)-z.begin())+1;
    int r=(int)(std::lower_bound(z.begin(),z.end(),a[i].second)-z.begin())+1;
    if(l<r){
      int s=veb.successor(l);
      if(s>=(int)z.size()||r<right[s]){
        for(int pos=veb.predecessor(l);r<right[pos];pos=veb.predecessor(pos)){
          lct_expose(&nds[pos]);
          if(nds[pos].left){
            nds[pos].left->par=nullptr;
            nds[pos].left=nullptr;
          }
          nds[pos].par=&nds[pos+1];
          nds[pos].v=0;
          nds[pos].sum--;
          veb.erase(pos);
        }
        lct_expose(&nds[l]);
        nds[l].v=1;
        if(nds[l].left){
          nds[l].left->par=nullptr;
          nds[l].left=nullptr;
        }
        nds[l].update();
        nds[l].par=&nds[r];
        veb.insert(l);
        right[l]=r;
      }
    }
    else base++;
    lct_expose(&nds[0]);
    res[i]=nds[0].sum+base;
  }
  return res;
}