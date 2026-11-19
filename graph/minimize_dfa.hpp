#pragma once
#include<vector>
#include<array>
#include<algorithm>
#include<queue>
#include<cassert>
#include "../datastructure/csr_array.hpp"
template<std::size_t K>
std::pair<std::vector<std::array<int,K>>,std::vector<bool>>minimize_dfa(std::vector<std::array<int,K>>g,std::vector<bool>accept,int s=0){
  assert(g.size()==accept.size());
  int n=g.size();
  assert(0<=s&&s<n);
  {
    bool f=false;
    for(const std::array<int,K>&a:g){
      for(int x:a){
        assert(-1<=x&&x<n);
        f|=x==-1;
      }
    }
    if(f){
      g.emplace_back([&n](){std::array<int,K>res;res.fill(n);return res;}());
      accept.push_back(false);
      for(std::array<int,K>&a:g){
        for(int&x:a)if(x==-1)x=n;
      }
      n++;
    }
  }
  std::queue<int>que;
  std::vector<bool>reach(n,false);
  reach[s]=true;
  que.push(s);
  while(!que.empty()){
    int x=que.front();que.pop();
    for(int i=0;i<(int)K;i++){
      if(!reach[g[x][i]]){
        reach[g[x][i]]=true;
        que.push(g[x][i]);
      }
    }
  }
  csr_array<int>rev=[&](){
    std::vector<std::pair<int,int>>res;
    for(int i=0;i<n;i++)if(reach[i]){
      for(int j=0;j<(int)K;j++){
        res.emplace_back(g[i][j]*K+j,i);
      }
    }
    return csr_array<int>(n*K,res);
  }();
  std::vector<std::vector<int>>block(2);
  for(int i=0;i<n;i++)if(reach[i])block[accept[i]].push_back(i);
  std::erase_if(block,[](const std::vector<int>&x){return x.empty();});
  std::vector<int>belong(n),pos(n);
  for(int i=0;i<(int)block.size();i++){
    for(int j=0;j<(int)block[i].size();j++){
      belong[block[i][j]]=i;
      pos[block[i][j]]=j;
    }
  }
  std::queue<std::pair<int,int>>que2;
  std::vector<std::array<bool,K>>inq(n,std::array<bool,K>{});
  auto add=[&](int i,int j)->void {
    if(!inq[i][j]){
      inq[i][j]=true;
      que2.emplace(i,j);
    }
  };
  if((int)block.size()==2){
    int id=block[0].size()<block[1].size();
    for(int i=0;i<(int)K;i++)add(id,i);
  }
  std::vector<int>vis(block.size());
  std::vector<int>head(block.size(),-1);
  std::vector<int>nxt(n,-1);
  int t=0;
  std::vector<int>pre,touched;
  while(!que2.empty()){
    auto [a,c]=que2.front();que2.pop();
    pre.clear();
    for(int u:block[a]){
      for(int v:rev[u*K+c])pre.push_back(v);
    }
    t++;
    touched.clear();
    for(int u:pre){
      int b=belong[u];
      if(vis[b]!=t){
        vis[b]=t;
        touched.push_back(b);
        head[b]=-1;
      }
      nxt[u]=head[b];
      head[b]=u;
    }
    for(int b:touched){
      int r=block[b].size();
      for(int u=head[b];u!=-1;u=nxt[u]){
        int i=pos[u];
        int j=--r;
        int v=block[b][j];
        std::swap(block[b][i],block[b][j]);
        pos[u]=j;
        pos[v]=i;
      }
      if(r==0||r==(int)block[b].size())continue;
      block.emplace_back(block[b].begin()+r,block[b].end());
      block[b].resize(r);
      inq.emplace_back(std::array<bool,K>{});
      vis.emplace_back(0);
      head.emplace_back(-1);
      for(int i=0;i<(int)block.back().size();i++){
        int u=block.back()[i];
        belong[u]=block.size()-1;
        pos[u]=i;
      }
      for(int i=0;i<(int)K;i++){
        if(inq[b][i]){
          add(block.size()-1,i);
        }
        else{
          if(block[b].size()<block.back().size())add(b,i);
          else add(block.size()-1,i);
        }
      }
    }
  }
  int m=block.size();
  std::vector<std::array<int,K>>res(m,std::array<int,K>{});
  std::vector<int>idx(n,-1);
  for(int i=0;i<m;i++)for(int j:block[i])idx[j]=i;
  for(int i=0;i<n;i++)if(reach[i]){
    int ni=idx[i];
    for(int j=0;j<(int)K;j++){
      int to=idx[g[i][j]];
      res[ni][j]=to;
    }
  }
  std::vector<bool>acc(m);
  for(int i=0;i<n;i++)if(reach[i]&&accept[i])acc[idx[i]]=true;
  int d=idx[s];
  std::swap(res[d],res[0]);
  int temp=acc[0];
  acc[0]=acc[d];
  acc[d]=temp;
  for(std::array<int,K>&to:res)for(int&x:to){
    if(x==0||x==d)x^=d;
  }
  return std::make_pair(res,acc);
}