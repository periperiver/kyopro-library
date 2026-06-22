#pragma once
#include<vector>
#include "../math/util.hpp"
template<typename Func>
void enumerate_unlabeled_tree(int n,const Func&f){
  int m=n/2;
  std::vector<unsigned>small;
  for(int i=1;i<=m;i++){
    auto dfs=[&](auto self,int k,unsigned now)->void {
      int ec=(msb(now)+1)/2;
      if(ec+1==i)small.push_back(now);
      else{
        for(int j=k;j>=0;j--){
          int c=(msb(small[j])+1)/2;
          if(ec+c+1>i)continue;
          self(self,j,now|(small[j]<<(ec*2+1))|(1<<((ec+c)*2+1)));
        }
      }
    };
    dfs(dfs,(int)small.size()-1,0);
  }
  std::vector<std::pair<int,int>>edges;
  std::vector<int>p;
  auto yaru=[&](unsigned long long now){
    edges.clear();
    p.clear();
    p.push_back(0);
    int pos=1;
    for(int i=0;i<n*2-2;i++){
      if(now>>i&1)p.pop_back();
      else{
        edges.emplace_back(p.back(),pos);
        p.push_back(pos++);
      }
    }
    f(edges);
  };
  auto dfs=[&](auto self,int k,unsigned long long now)->void {
    int ec=(msb(now)+1)/2;
    if(ec+1==n){
      yaru(now);
    }
    else{
      for(int j=k;j>=0;j--){
        int c=(msb(small[j])+1)/2;
        if(c>=(n-1)/2)continue;
        if(ec+c+1>n)continue;
        self(self,j,now|((unsigned long long)small[j]<<(ec*2+1))|(1ull<<((ec+c)*2+1)));
      }
    }
  };
  dfs(dfs,(int)small.size()-1,0);
  if(m*2==n){
    for(unsigned long long x:small)if((msb(x)+1)/2+1==m){
      for(unsigned long long y:small)if((msb(y)+1)/2+1==m&&x<=y){
        yaru(x|(y<<(m*2-1))|(1ull<<(n*2-3)));
      }
    }
  }
}