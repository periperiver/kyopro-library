#pragma once
#include<vector>
#include<algorithm>
std::vector<std::pair<int,int>>bipartite_matching(int L,int R,const std::vector<std::pair<int,int>>&edge){
  static constexpr int h=0x3c07e67b;
  std::vector<int>ptr(L+R+1);
  std::vector<int>g(edge.size()*2);
  for(const auto&[u,v]:edge){
    ptr[u]++,ptr[L+v]++;
  }
  for(int i=1;i<=L+R;i++)ptr[i]+=ptr[i-1];
  for(const auto&[u,v]:edge){
    g[--ptr[u]]=L+v;
    g[--ptr[L+v]]=u;
  }
  std::vector<int>mate(L+R,-1);
  std::vector<bool>seen(L+R,false);
  auto dfs=[&](auto self,int x)->void {
    seen[x]=true;
    for(int i=ptr[x];i<ptr[x+1];i++){
      int j=g[i];
      if(!seen[j]){
        self(self,j);
        if(mate[x]==-1&&mate[j]==-1){
          mate[x]=j;
          mate[j]=x;
        }
      }
    }
  };
  for(int i=0;i<seen.size();i++)if(!seen[i]){
    dfs(dfs,i);
  }
  std::vector<int>que(L);
  int p,q;
  std::vector<int>vis(L,-1),d(L);
  int s=0;
  while(true){
    p=q=0;
    for(int i=0;i<L;i++){
      if(mate[i]==-1){
        d[i]=0;
        que[q++]=i;
      }
      else d[i]=h;
    }
    while(p<q){
      int x=que[p++];
      for(int i=ptr[x];i<ptr[x+1];i++){
        int j=g[i];
        if(mate[j]!=-1&&d[mate[j]]==h){
          d[mate[j]]=d[x]+1;
          que[q++]=mate[j];
        }
      }
    }
    auto dfs=[&](auto self,int x)->bool {
      vis[x]=s;
      for(int i=ptr[x];i<ptr[x+1];i++){
        int j=g[i];
        if(mate[j]==-1||(d[mate[j]]-d[x]==1&&vis[mate[j]]!=s&&self(self,mate[j]))){
          mate[x]=j;
          mate[j]=x;
          return true;
        }
      }
      return false;
    };
    bool f=false;
    for(int i=0;i<L;i++){
      if(vis[i]!=s&&mate[i]==-1){
        f|=dfs(dfs,i);
      }
    }
    if(!f)break;
    s++;
  }
  std::vector<std::pair<int,int>>res;
  res.reserve(L-std::count(mate.begin(),mate.begin()+L,-1));
  for(int i=0;i<L;i++)if(mate[i]!=-1)res.emplace_back(i,mate[i]-L);
  return res;
}