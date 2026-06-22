#pragma once
#include "template.hpp"
struct Mollback{
  int n,b;
  vector<pair<int,int>>query;
  Mollback(int n):n(n),b(sqrt(n)){}
  void add(int l,int r){
    query.emplace_back(l,r);
  }
  void build(auto init,auto undo,auto add,auto out){
    int q=query.size();
    vector<vector<int>>c((n+b-1)/b);
    rep(i,q){
      if(query[i].second-query[i].first<=b){
        reps(j,query[i].first,query[i].second)add(j);
        out(i);
        undo(query[i].second-query[i].first);
      }
      else c[query[i].first/b].push_back(i);
    }
    rep(i,c.size()){
      sort(all(c[i]),[&](int x,int y){return query[x].second<query[y].second;});
      int r=(i+1)*b;
      init();
      for(int j:c[i]){
        while(r<query[j].second)add(r++);
        for(int k=(i+1)*b-1;k>=query[j].first;k--)add(k);
        out(j);
        undo((i+1)*b-query[j].first);
      }
    }
  }
};