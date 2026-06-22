#pragma once
#include "bipartite_matching.hpp"
#include "scc.hpp"
#include<queue>
std::vector<std::pair<std::vector<int>,std::vector<int>>>dulmage_mendelsohn_decomposition(int l,int r,const std::vector<std::pair<int,int>>&edge){
  std::vector<std::pair<int,int>>match=bipartite_matching(l,r,edge);
  std::vector<int>usedl(l+1,0),usedr(r+1,0);
  Graph<>g(l+r,true);
  for(const auto&[u,v]:edge)g.add_edge(u,v+l);
  for(const auto&[u,v]:match){
    usedl[u]=true;
    usedr[v]=true;
    g.add_edge(v+l,u);
  }
  g.build();
  std::vector<int>w0l,w0r,wkl,wkr;
  std::vector<bool>w0k(l+r,false);
  std::queue<int>que;
  for(int i=0;i<l;i++)if(!usedl[i]){
    que.push(i);
    w0k[i]=true;
  }
  while(!que.empty()){
    int x=que.front();
    que.pop();
    if(x<l)wkl.push_back(x);
    else wkr.push_back(x-l);
    for(const Edge<>&e:g[x])if(!w0k[e.to]){
      w0k[e.to]=true;
      que.push(e.to);
    }
  }
  g.reverse();
  for(int i=0;i<r;i++)if(!usedr[i]){
    que.push(i+l);
    w0k[i+l]=true;
  }
  while(!que.empty()){
    int x=que.front();
    que.pop();
    if(x<l)w0l.push_back(x);
    else w0r.push_back(x-l);
    for(const Edge<>&e:g[x])if(!w0k[e.to]){
      w0k[e.to]=true;
      que.push(e.to);
    }
  }
  for(int i=0;i<l;i++)if(w0k[i])usedl[i]=0;
  for(int i=0;i<r;i++)if(w0k[i+l])usedr[i]=0;
  for(int i=l-1;i>=0;i--)usedl[i]+=usedl[i+1];
  for(int i=r-1;i>=0;i--)usedr[i]+=usedr[i+1];
  if(usedl[0]+usedr[0]==0){
    std::vector<std::pair<std::vector<int>,std::vector<int>>>res(2);
    res[0]=std::make_pair(std::move(w0l),std::move(w0r));
    res[1]=std::make_pair(std::move(wkl),std::move(wkr));
    return res;
  }
  Graph<>g2(usedl[0]+usedr[0],true);
  for(const auto&[u,v]:edge){
    if(usedl[u]==usedl[u+1])continue;
    if(usedr[v]==usedr[v+1])continue;
    g2.add_edge(usedl[u+1],usedr[v+1]+usedl[0]);
  }
  for(const auto&[u,v]:match){
    if(usedl[u]==usedl[u+1])continue;
    if(usedr[v]==usedr[v+1])continue;
    g2.add_edge(usedr[v+1]+usedl[0],usedl[u+1]);
  }
  g2.build();
  std::vector<int>scc=strongly_connected_components(g2);
  int sz=*std::max_element(scc.begin(),scc.end())+1;
  std::vector<std::pair<std::vector<int>,std::vector<int>>>res(sz+2);
  res[0]=std::make_pair(std::move(w0l),std::move(w0r));
  res[sz+1]=std::make_pair(std::move(wkl),std::move(wkr));
  for(int i=0;i<l;i++)if(usedl[i]!=usedl[i+1]){
    res[scc[usedl[i+1]]+1].first.push_back(i);
  }
  for(int i=0;i<r;i++)if(usedr[i]!=usedr[i+1]){
    res[scc[usedl[0]+usedr[i+1]]+1].second.push_back(i);
  }
  return res;
}