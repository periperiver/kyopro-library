#pragma once
#include "suffix_array.hpp"
#include "lcp_array.hpp"
#include "../datastructure/static_range_min.hpp"
#include "../other/pair_limits.hpp"
#include<ranges>
struct MinimumRotation{
private:
  struct sa_rmq{
    std::vector<int>inv;
    StaticRangeMin<int>sp;
    sa_rmq(){}
    sa_rmq(const std::vector<int>&a):inv(a.size()){
      std::vector<int>sa=suffix_array(a);
      for(auto [i,j]:sa|std::views::enumerate)inv[j]=i;
      std::vector<int>lcp=lcp_array(a,sa);
      sp=StaticRangeMin<int>(lcp);
    }
    int lcp(int l1,int l2)const{
      l1=inv[l1],l2=inv[l2];
      if(l1>l2)std::swap(l1,l2);
      return sp.min(l1,l2);
    }
  };
  std::vector<int>s;
  sa_rmq sa1,sa2;
  StaticRangeMin<std::pair<int,int>>isp;
public:
  MinimumRotation(){}
  template<typename T>
  explicit MinimumRotation(const T&init){
    static_assert(std::is_integral_v<typename T::value_type>);
    if(init.empty())return;
    std::vector<typename T::value_type>z(init.begin(),init.end());
    std::sort(z.begin(),z.end());
    z.erase(std::unique(z.begin(),z.end()),z.end());
    s.resize(init.size());
    for(int i=0;i<(int)s.size();i++)s[i]=std::lower_bound(z.begin(),z.end(),init[i])-z.begin();
    sa1=sa_rmq(s);
    std::reverse(s.begin(),s.end());
    sa2=sa_rmq(s);
    std::reverse(s.begin(),s.end());
    isp=StaticRangeMin<std::pair<int,int>>([](std::vector<int>a){
      for(int&x:a)x=-x;
      std::vector<int>sa=suffix_array(a);
      std::vector<std::pair<int,int>>res(a.size());
      for(auto [i,j]:sa|std::views::enumerate)res[j]=std::make_pair(-i,j);
      return res;
    }(s));
  }
  int query(int l,int r)const{
    assert(0<=l&&l<r&&r<=(int)s.size());
    int res=0;
    auto eval=[&](int x)->void {
      if(x==res)return;
      int lcp=sa1.lcp(l+res,l+x);
      if(l+x+lcp<r){
        if(s[l+res+lcp]>s[l+x+lcp])res=x;
      }
      else{
        int lcp2=sa1.lcp(res-x+r,l);
        if(res+r-x+lcp2<r){
          if(s[res+r-x+lcp2]>s[l+lcp2])res=x;
        }
        else{
          int lcp3=sa1.lcp(l,l+x-res);
          if(l+x-res+lcp3<r&&s[l+lcp3]>s[l+x-res+lcp3])res=x;
        }
      }
    };
    for(int l2=l;l2+1<r;){
      int mid=(l2+r)/2;
      int p=isp.min(l2,mid).second;
      if(l2<p){
        int q=isp.min(l2,p).second;
        if(sa1.lcp(p,q)>=r-p){
          if(q==l2)p=l2;
          else{
            int alpha=std::min(sa2.lcp(s.size()-p,s.size()-q)/(p-q)+1,(p-l2)/(p-q));
            p-=alpha*(p-q);
          }
        }
      }
      eval(p-l);
      l2=mid;
    }
    eval(r-l-1);
    return res;
  }
};