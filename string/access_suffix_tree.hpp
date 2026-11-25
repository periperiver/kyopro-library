#pragma once
#include<numeric>
#include "suffix_tree.hpp"
template<int sigma>
struct AccessSuffixTree:SuffixTree<sigma>{
  int n;
  std::vector<int>top,in,ord;
  AccessSuffixTree(){}
  AccessSuffixTree(std::vector<int>s):SuffixTree<sigma>(s),n(s.size()),top(this->size()),in(this->size()),ord(this->size()){
    std::vector<int>sub(this->size()),heavy(this->size());
    auto dfs=[&](auto self,int x)->void {
      sub[x]=1;
      int mx=-1;
      for(int i=0;i<sigma;i++){
        int s=this->operator[](x)[i];
        if(s!=-1){
          self(self,s);
          sub[x]+=sub[s];
          if(mx<sub[s]){
            mx=sub[s];
            heavy[x]=s;
          }
        }
      }
    };
    int ptr=0;
    auto dfs2=[&](auto self,int x)->void {
      ord[ptr]=x;
      in[x]=ptr++;
      if(sub[x]!=1){
        top[heavy[x]]=top[x];
        self(self,heavy[x]);
      }
      for(int i=0;i<sigma;i++){
        int s=this->operator[](x)[i];
        if(s!=-1&&s!=heavy[x])self(self,s);
      }
    };
    std::iota(top.begin(),top.end(),0);
    dfs(dfs,n);
    dfs2(dfs2,n);
  }
  AccessSuffixTree(std::string s):AccessSuffixTree([](std::string s){
    std::vector<int>res(s.size());
    for(int i=0;i<(int)s.size();i++){
      res[i]=s[i];
    }
    return res;
  }(std::move(s))){}
  int query(int l,int r)const{
    assert(0<=l&&l<=r&&r<=n);
    if(l==r)return n;
    int v=l;
    int len=n-r;
    while(true){
      int t=top[v];
      const auto&nd_v=this->operator[](v);
      const auto&nd_t=this->operator[](t);
      if(len>=nd_v.len-nd_t.len){
        len-=nd_v.len-nd_t.len;
        if(len<nd_t.edge_length())return t;
        len-=nd_t.edge_length();
        v=nd_t.par;
      }
      else break;
    }
    int ok=in[v],ng=in[top[v]];
    len=this->operator[](v).len-len;
    while(ok-ng>1){
      int mid=(ok+ng)/2;
      if(len<=this->operator[](ord[mid]).len)ok=mid;
      else ng=mid;
    }
    return ord[ok];
  }
};