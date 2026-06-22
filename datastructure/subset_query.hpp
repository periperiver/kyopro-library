#pragma once
#include<vector>
#include<cassert>
#include<functional>
#include<algorithm>
#include "../random/generator.hpp"
struct SubsetQuery{
private:
  int lg;
  int mask[3];
public:
  SubsetQuery():lg(0){}
  explicit SubsetQuery(int lg):lg(lg){
    assert(0<=lg&&lg<=30);
    mask[0]=mask[1]=mask[2]=0;
    for(int i=0;i<lg;i++)mask[Random::range(3)]|=1<<i;
  }
  SubsetQuery(int lg,const std::vector<std::pair<int,int>>&lr,const std::vector<int>&p):lg(lg){
    assert(0<=lg&&lg<=30);
    assert(std::all_of(lr.begin(),lr.end(),[&](std::pair<int,int>a){return 0<=a.first&&(a.first|a.second)==a.second&&a.second<(1<<lg);}));
    assert(std::all_of(p.begin(),p.end(),[&](int x){return 0<=x&&x<(1<<lg);}));
    mask[0]=mask[1]=mask[2]=0;
    long long cost[3];
    for(int i=0;i<lg;i++){
      cost[0]=cost[1]=cost[2]=0;
      for(const auto&[l,r]:lr){
        int s=(l^r)&mask[0];
        s|=l&mask[1];
        s|=~r&mask[2];
        s=std::popcount<unsigned>(s);
        if((l^r)>>i&1)cost[0]+=1<<(s+1);
        else cost[0]+=1<<s;
        if(l>>i&1)cost[1]+=1<<(s+1);
        else cost[1]+=1<<s;
        if((~r)>>i&1)cost[2]+=1<<(s+1);
        else cost[2]+=1<<s;
      }
      for(int j:p){
        int s=(~j&mask[1])|(j&mask[2]);
        s=std::popcount<unsigned>(s);
        cost[0]++;
        if(j>>i&1)cost[1]+=1<<s,cost[2]+=1<<(s+1);
        else cost[1]+=1<<(s+1),cost[2]+=1<<s;
      }
      mask[std::min_element(cost,cost+3)-cost]|=1<<i;
    }
  }
  template<typename Func>
  void get_range(int l,int r,const Func&f){
    static_assert(std::is_convertible_v<Func,std::function<void(int,bool)>>);
    assert(0<=l&&((l|r)==r)&&r<(1<<lg));
    int x=l&mask[0];
    int s=(l^r)&mask[0];
    int b=0;
    x|=~r&mask[1];
    s|=l&mask[1];
    b|=l&mask[1];
    x|=l&mask[2];
    s|=~r&mask[2];
    b|=~r&mask[2];
    for(int i=s;;i=(i-1)&s){
      f(x|i,__builtin_parity(i&b));
      if(i==0)break;
    }
  }
  template<typename Func>
  void get_vs(int j,const Func&f){
    static_assert(std::is_convertible_v<Func,std::function<void(int)>>);
    assert(0<=j&&j<(1<<lg));
    int x=j&mask[0];
    int s=(~j&mask[1])|(j&mask[2]);
    for(int i=s;;i=(i-1)&s){
      f(x|i);
      if(i==0)break;
    }
  }
};