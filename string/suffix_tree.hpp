#pragma once
#include<vector>
#include<climits>
#include<type_traits>
#include<bit>
#include<cstdint>
#include<string>
#include "../datastructure/bit_csr.hpp"
#include "suffix_array.hpp"
#include "lcp_array.hpp"
namespace suffix_tree_impl{
struct Data{
  std::pair<int,int>range;
  int len;
  int par;
  Data():range(-1,-1),len(0),par(-1){}
  int edge_length()const{return range.second-range.first;}
};
template<int sigma=26>
struct SuffixTree:bit_csr<Data,sigma>{
  SuffixTree(){}
  explicit SuffixTree(std::vector<int>s):bit_csr<Data,sigma>([](std::vector<int>s){
    std::vector<Data>dat(s.size()+1);
    std::vector<std::tuple<int,int,int>>edge;
    if(!s.empty()){
      std::vector<int>sa=suffix_array(s);
      std::vector<int>lcp=lcp_array(s,sa);
      std::vector<int>pos(s.size()+1);
      int now=s.size();
      edge.emplace_back(now,s[sa[0]],sa[0]);
      dat[sa[0]].range=std::make_pair(sa[0],s.size());
      dat[sa[0]].par=now;
      dat[sa[0]].len=s.size()-sa[0];
      pos[now]=0;
      now=sa[0];
      for(int i=1;i<(int)s.size();i++){
        while(lcp[i-1]<dat[now].len)now=dat[now].par;
        if(lcp[i-1]!=dat[now].len){
          int newidx=dat.size();
          Data&d=dat.emplace_back();
          pos.emplace_back();
          int c=std::get<2>(edge[pos[now]]);
          dat[c].par=newidx;
          dat[newidx].par=now;
          std::get<2>(edge[pos[now]])=newidx;
          dat[newidx].range=dat[c].range;
          dat[newidx].range.second-=dat[c].len-lcp[i-1];
          dat[newidx].len=lcp[i-1];
          dat[c].range.first+=dat[newidx].len-dat[now].len;
          pos[newidx]=edge.size();
          edge.emplace_back(newidx,s[dat[newidx].range.second],c);
          now=newidx;
        }
        dat[sa[i]].par=now;
        pos[now]=edge.size();
        edge.emplace_back(now,s[sa[i]+lcp[i-1]],sa[i]);
        dat[sa[i]].len=s.size()-sa[i];
        dat[sa[i]].range=std::make_pair(dat[now].len+sa[i],s.size());
        now=sa[i];
      }
    }
    return bit_csr<Data,sigma>(std::move(dat),std::move(edge));
  }(std::move(s))){}
  explicit SuffixTree(std::string s):SuffixTree([](std::string s){
    std::vector<int>res(s.size());
    for(int i=0;i<(int)s.size();i++)res[i]=s[i];
    return res;
  }(std::move(s))){}
};
}
using suffix_tree_impl::SuffixTree;