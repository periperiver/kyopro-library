#pragma once
#include<vector>
#include<climits>
#include<type_traits>
#include<bit>
#include<cstdint>
#include<string>
#include "../datastructure/trie.hpp"
#include "suffix_array.hpp"
#include "lcp_array.hpp"
template<int sigma=26>
struct SuffixTree{
private:
  struct Data{
    std::pair<int,int>range;
    int len;
    Data():range(-1,-1),len(0){}
  };
  Trie<Data,sigma>trie;
public:
  using node=typename Trie<Data,sigma>::node;
  SuffixTree(){}
  explicit SuffixTree(std::vector<int>s){
    std::vector<Data>dat(s.size()+1);
    std::vector<std::tuple<int,int,int>>edge;
    if(!s.empty()){
      std::vector<int>sa=suffix_array(s);
      std::vector<int>lcp=lcp_array(s,sa);
      std::vector<int>par(s.size()+1,-1),last(s.size()+1,-1),pos(s.size()+1);
      int now=s.size();
      edge.emplace_back(now,s[sa[0]],sa[0]);
      dat[sa[0]].range=std::make_pair(sa[0],s.size());
      par[sa[0]]=now;
      dat[sa[0]].len=s.size()-sa[0];
      last[now]=sa[0];
      pos[now]=0;
      now=sa[0];
      for(int i=1;i<(int)s.size();i++){
        while(lcp[i-1]<dat[now].len)now=par[now];
        if(lcp[i-1]!=dat[now].len){
          int newidx=dat.size();
          Data&d=dat.emplace_back();
          par.emplace_back();
          last.emplace_back();
          pos.emplace_back();
          int c=last[now];
          par[c]=newidx;
          par[newidx]=now;
          std::get<2>(edge[pos[now]])=newidx;
          last[now]=newidx;
          dat[newidx].range=dat[c].range;
          dat[newidx].range.second-=dat[c].len-lcp[i-1];
          dat[newidx].len=lcp[i-1];
          dat[c].range.first+=dat[newidx].len-dat[now].len;
          last[newidx]=c;
          pos[newidx]=edge.size();
          edge.emplace_back(newidx,s[dat[newidx].range.second],c);
          now=newidx;
        }
        par[sa[i]]=now;
        last[now]=sa[i];
        pos[now]=edge.size();
        edge.emplace_back(now,s[sa[i]+lcp[i-1]],sa[i]);
        dat[sa[i]].len=s.size()-sa[i];
        dat[sa[i]].range=std::make_pair(dat[now].len+sa[i],s.size());
        now=sa[i];
      }
    }
    trie=Trie<Data,sigma>(std::move(dat),std::move(edge));
  }
  explicit SuffixTree(std::string s):SuffixTree([](std::string s){
    std::vector<int>res(s.size());
    for(int i=0;i<(int)s.size();i++)res[i]=s[i];
    return res;
  }(std::move(s))){}
  node& operator[](int x){return trie[x];}
  const node& operator[](int x)const{return trie[x];}
  int size()const{return trie.size();}
};