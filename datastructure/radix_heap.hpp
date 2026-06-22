#pragma once
#include<cassert>
#include<type_traits>
#include<vector>
#include "math/util.hpp"
using namespace std;
template<typename T>
struct radix_heap{
  static_assert(is_integral_v<T>);
private:
  int sz;
  T last;
  vector<pair<T,int>>h[1+numeric_limits<T>::digits];
  vector<pair<int,int>>p;
public:
  radix_heap(int n):p(n,make_pair(0,-1)),sz(0),last(0){}
  inline bool empty()const{return sz==0;}
  int pop(){
    if(h[0].empty()){
      int b=0;
      while(h[b].empty())b++;
      last=h[b].back().first;
      for(int j=0;j<(int)h[b].size();j++)if(last>h[b][j].first)last=h[b][j].first;
      for(int j=0;j<(int)h[b].size();j++){
        int i=h[b][j].second;
        int nb=msb(h[b][j].first^last)+1;
        p[i]=make_pair(nb,(int)h[nb].size());
        h[nb].emplace_back(move(h[b][j]));
      }
      h[b].clear();
    }
    --sz;
    int i=h[0].back().second;
    p[i].second=-1;
    h[0].pop_back();
    return i;
  }
  void decrease_key(int i,T v){
    if(p[i].second<0){
      int b=msb(v^last)+1;
      ++sz;
      p[i]=make_pair(b,(int)h[b].size());
      h[b].emplace_back(v,i);
    }
    else if(h[p[i].first][p[i].second].first>v){
      int preb=p[i].first,nxtb=msb(v^last)+1;
      if(nxtb<preb){
        int i2=h[preb].back().second,j=p[i].second;
        swap(h[preb][j],h[preb].back());
        p[i2].second=j;
        p[i]=make_pair(nxtb,(int)h[nxtb].size());
        h[nxtb].emplace_back(v,i);
        h[preb].pop_back();
      }
      else h[preb][p[i].second].first=v;
    }
  }
};