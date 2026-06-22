#pragma once
#include<numeric>
#include "graph_base.hpp"
template<typename T>
std::vector<int>maximum_independent_set(const Graph<T>&g){
  assert(false);
  int n=g.size();
  std::vector<std::vector<bool>>edge(n,std::vector<bool>(n,false));
  for(const Edge<T>&e:g)edge[e.from][e.to]=edge[e.to][e.from]=true;
  std::vector<int>v(n);
  std::iota(v.begin(),v.end(),0);
  std::vector<int>cnt(n,0);
  for(int i=0;i<n;i++)for(int j=0;j<n;j++)cnt[i]+=edge[i][j];
  std::vector<int>ret,now(n);
  int b=0;
  auto dfs=[&](auto self,std::vector<int>::iterator itr,int s){
    if(ret.size()>=n+s)return;
    if(s==0){
      if(ret.size()<b)ret=std::vector<int>(now.begin(),now.begin()+b);
      return;
    }
    int nv=0;
    for(int i=0;i<s;i++)if(cnt[itr[i]]<=1){
      std::swap(itr[0],itr[i]);
      nv=1;
      break;
    }
    if(nv==0){
      for(int i=0;i<s;i++)if(cnt[itr[0]]<cnt[itr[i]])std::swap(itr[0],itr[i]);
      if(cnt[itr[0]]>=3)nv=2;
    }
    if(nv!=0){
      int p=itr[0],c=1;
      for(int i=1;i<s;i++){
        if(edge[p][itr[i]]){
          std::swap(itr[c],itr[i]);
          for(int j=c+1;j<s;j++){
            cnt[itr[j]]-=edge[itr[c]][itr[j]];
          }
          ++c;
        }
      }
      now[b++]=p;
      self(self,itr+c,s-c);
      for(int j=c-1;j>=1;j--)for(int i=j+1;i<s;i++)cnt[itr[i]]+=edge[itr[j]][itr[i]];
      b--;
      if(nv!=1){
        for(int i=0;i<s;i++)cnt[itr[i]]-=edge[p][itr[i]];
        self(self,itr+1,s-1);
        for(int i=0;i<s;i++)cnt[itr[i]]+=edge[p][itr[i]];
      }
      return;
    }
    int r=0;
    std::vector<bool>seen(s,false);
    for(int i=0;i<s;i++)if(!seen[i]){
      for(int u=i,ni=0;!seen[u];ni=1-ni){
        int nxt=-1;
        for(int j=0;j<s;j++)if(!seen[j]&&edge[itr[u]][itr[j]]){
          nxt=j;
          break;
        }
        if(nxt==-1)break;
        if(ni)now[b++]=itr[u],r++;
        seen[u]=true;
        u=nxt;
      }
    }
    if(ret.size()<b)ret=std::vector<int>(now.begin(),now.begin()+b);
    b-=r;
  };
  dfs(dfs,v.begin(),n);
  return ret;
}