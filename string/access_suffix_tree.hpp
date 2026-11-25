#pragma once
#include<numeric>
#include "suffix_tree.hpp"
template<int sigma>
struct AccessSuffixTree:SuffixTree<sigma>{
  int n;
  std::vector<int>top,length,in;
  AccessSuffixTree(){}
  AccessSuffixTree(std::vector<int>s):SuffixTree(s),n(s.size()),top(this->size()),length(this->size()),in(this->size()){
    std::vector<int>sub(thihs->size()),heavy(this->size());
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
            heavy[x]=i;
          }
        }
      }
    };
    int ptr=0;
    auto dfs2=[&](auto self,int x)->void {
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
  //node,len
  std::pair<int,int>access(int l,int r)const{
    assert(0<=l&&l<r&&r<=n);
    int v=l;
    int len=n-r;
    while(len>0){
      int t=top[v];
    }
  }
};