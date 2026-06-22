#pragma once
#include<vector>
#include<climits>
#include<type_traits>
#include<bit>
#include<cstdint>
#include "suffix_array.hpp"
#include "lcp_array.hpp"
template<int sigma=26>
struct SuffixTree{
private:
  static_assert(sigma<=64);
  using mask_t=std::conditional_t<sigma<=32,uint32_t,uint64_t>;
  struct node{
    std::pair<int,int>range;
    int par,len;
    std::vector<int>child;
    mask_t mask;
    node():par(-1),len(0),range(-1,-1),mask(0){}
    int operator[](int i)const{return mask>>i&1?child[std::popcount(mask&((mask_t(1)<<i)-1))]:-1;}
  };
  std::vector<node>dat;
public:
  explicit SuffixTree(const std::string&s):dat(s.size()+1){
    if(s.empty())return;
    std::vector<int>sa=suffix_array(s);
    std::vector<int>lcp=lcp_array(s,sa);
    int now=s.size();
    dat[now].child.push_back(sa[0]);
    dat[now].mask|=mask_t(1)<<s[sa[0]];
    dat[sa[0]].range=std::make_pair(sa[0],s.size());
    dat[sa[0]].par=now;
    dat[sa[0]].len=s.size()-sa[0];
    now=sa[0];
    for(int i=1;i<(int)s.size();i++){
      while(lcp[i-1]<dat[now].len)now=dat[now].par;
      if(lcp[i-1]!=dat[now].len){
        int newidx=dat.size();
        dat.push_back(node());
        int c=dat[now][s[sa[i]+dat[now].len]];
        dat[c].par=newidx;
        dat[newidx].par=now;
        dat[now].child.back()=newidx;
        dat[newidx].range=dat[c].range;
        dat[newidx].range.second-=dat[c].len-lcp[i-1];
        dat[newidx].len=lcp[i-1];
        dat[c].range.first+=dat[newidx].len-dat[now].len;
        dat[newidx].child.push_back(c);
        dat[newidx].mask|=mask_t(1)<<s[dat[newidx].range.second];
        now=newidx;
      }
      dat[sa[i]].par=now;
      dat[now].child.push_back(sa[i]);
      dat[now].mask|=mask_t(1)<<s[sa[i]+dat[now].len];
      dat[sa[i]].len=s.size()-sa[i];
      dat[sa[i]].range=std::make_pair(dat[now].len+sa[i],s.size());
      now=sa[i];
    }
  }
  node& operator[](int i){return dat[i];}
  const node& operator[](int i)const{return dat[i];}
  inline int size()const{return dat.size();}
};