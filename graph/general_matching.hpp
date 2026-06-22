#pragma once
#include<vector>
#include<queue>
std::vector<std::pair<int,int>>general_matching(const std::vector<std::vector<bool>>&graph){
  int n=graph.size();
  std::vector<std::vector<std::pair<int,int>>>g(n+1);
  std::vector<std::pair<int,int>>edge;
  {
    int k=n+1;
    for(int i=0;i<n-1;i++)for(int j=i+1;j<n;j++){
      if(graph[i][j]){
        g[i+1].emplace_back(j+1,k);
        g[j+1].emplace_back(i+1,k);
        edge.emplace_back(i+1,j+1);
        ++k;
      }
    }
  }
  std::vector<int>mate(n+1,0),label(n+1,-1),first(n+1,0);
  std::queue<int>que;
  auto eval_first=[&](auto self,int x)->int {
    if(label[first[x]]<0)return first[x];
    return first[x]=self(self,first[x]);
  };
  auto rematch=[&](auto self,int v,int w)->void {
    int t=mate[v];
    mate[v]=w;
    if(mate[t]!=v)return;
    if(label[v]<=n){
      mate[t]=label[v];
      self(self,label[v],t);
    }
    else{
      auto [x,y]=edge[label[v]-n-1];
      self(self,x,y);
      self(self,y,x);
    }
  };
  auto assignlabel=[&](int x,int y,int num)->void {
    int r=eval_first(eval_first,x);
    int s=eval_first(eval_first,y);
    int join=0;
    if(r==s)return;
    label[r]=-num;
    label[s]=-num;
    while(true){
      if(s)std::swap(r,s);
      r=eval_first(eval_first,label[mate[r]]);
      if(label[r]==-num){
        join=r;
        break;
      }
      label[r]=-num;
    }
    int v=first[x];
    while(v!=join){
      que.push(v);
      label[v]=num;
      first[v]=join;
      v=first[label[mate[v]]];
    }
    v=first[y];
    while(v!=join){
      que.push(v);
      label[v]=num;
      first[v]=join;
      v=first[label[mate[v]]];
    }
  };
  auto augmentcheck=[&](int u)->bool {
    first[u]=label[u]=0;
    que.push(u);
    while(!que.empty()){
      int x=que.front();
      que.pop();
      for(auto e:g[x]){
        int y=e.first;
        if(mate[y]==0&&y!=u){
          mate[y]=x;
          rematch(rematch,x,y);
          return true;
        }
        else if(label[y]>=0){
          assignlabel(x,y,e.second);
        }
        else if(label[mate[y]]<0){
          label[mate[y]]=x;
          first[mate[y]]=y;
          que.push(mate[y]);
        }
      }
    }
    return false;
  };
  for(int i=1;i<=n;i++){
    while(!que.empty())que.pop();
    if(mate[i]){
      continue;
    }
    if(augmentcheck(i))std::fill(label.begin(),label.end(),-1);
  }
  std::vector<std::pair<int,int>>res;
  for(int i=1;i<=n;i++)if(i<mate[i])res.emplace_back(i-1,mate[i]-1);
  return res;
}