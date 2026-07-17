#pragma once
#include<vector>
#include<numeric>
#include "../datastructure/dynamic/splay.hpp"
template<typename Compare>
std::vector<int>merge_insertion_sort(int n,const Compare&compare){
  struct node{
    node *left=nullptr,*right=nullptr,*par=nullptr;
    int key=-1;
    int sz=1;
    inline void update(){
      sz=1;
      if(left)sz+=left->sz;
      if(right)sz+=right->sz;
    }
  };
  auto dfs=[&](auto self,std::vector<int>&idx)->void {
    int m=idx.size();
    if(m==1)return;
    int amari=-1;
    if(m&1){
      amari=idx.back();
      m--;
    }
    std::vector<std::pair<int,int>>small;
    std::vector<int>large;
    small.reserve(m/2);
    large.reserve(m/2);
    for(int i=0;i<m;i+=2){
      int a=idx[i],b=idx[i+1];
      if(compare(a,b))small.emplace_back(a,b),large.emplace_back(b);
      else small.emplace_back(b,a),large.emplace_back(a);
    }
    self(self,large);
    std::vector<int>pos(n,-1);
    for(int i=0;i<m/2;i++)pos[large[i]]=i;
    std::vector<int>small_v(m/2+(amari!=-1));
    for(const auto&[a,b]:small)small_v[pos[b]]=a;
    if(amari!=-1){
      small_v.back()=amari;
      m++;
    }
    std::vector<node>nds;
    nds.reserve(m);
    nds.resize(large.size()+1);
    nds[0].key=small_v[0];
    node *root=&nds[0];
    for(int i=0;i<(int)large.size();i++){
      root->par=&nds[i+1];
      nds[i+1].left=root;
      root=root->par;
      root->key=large[i];
      root->update();
    }
    for(int i=1,j=2,k=4;i<(int)small_v.size();i+=j,j=k-j,k<<=1){
      for(int l=std::min<int>(small_v.size(),i+j)-1;l>=i;l--){
        int left=-1,right=std::min<int>(root->sz,i*2+j-1);
        while(right-left>1){
          int mid=(left+right)/2;
          root=get_k(root,mid);
          if(compare(root->key,small_v[l]))left=mid;
          else right=mid;
        }
        nds.emplace_back();
        node *nd=&nds.back();
        nd->key=small_v[l];
        if(right==root->sz){
          nd->left=root;
          root->par=nd;
          root=nd;
          root->update();
        }
        else{
          root=get_k(root,right);
          nd->left=root->left;
          if(nd->left)nd->left->par=nd;
          nd->update();
          nd->par=root;
          root->left=nd;
          root->update();
        }
      }
    }
    for(int i=0;i<m;i++){
      root=left_most(root);
      idx[i]=root->key;
      if(i+1==m)break;
      node *nd=root->right;
      nd->par=nullptr;
      root->right=nullptr;
      root=nd;
    }
  };
  std::vector<int>res(n);
  std::iota(res.begin(),res.end(),0);
  if(n>1)dfs(dfs,res);
  return res;
}