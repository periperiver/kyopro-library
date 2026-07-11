#pragma once
#include "../graph/dinic.hpp"
#include "../datastructure/unionfind.hpp"
#include "overflow_check.hpp"
#include<cassert>
#include<concepts>
#include<algorithm>
template<std::integral T>
struct ProjectSelectionProblem{
private:
  int n;
  Dinic<T> g;
  struct edge{
    int x,y;
    bool f,g;
    T cost;
    edge(){}
    edge(int x_,int y_,bool f_,bool g_,T cost_):x(x_),y(y_),f(f_),g(g_),cost(cost_){}
    bool operator<(const edge&rhs)const{return x==rhs.x?y<rhs.y:x<rhs.x;}
    bool operator==(const edge&rhs)const{return x==rhs.x&&y==rhs.y;}
  };
  std::vector<T>cost1,cost2;
  std::vector<edge>cost3;
  std::vector<bool>flip;
public:
  ProjectSelectionProblem(){}
  ProjectSelectionProblem(int n_):n(n_),g(n+2),cost1(n),cost2(n),flip(n,false){}
  void add(int x,bool f,T cost){
    assert(!is_overflow_mul<T>(n,cost));
    if(!f)cost1[x]+=cost;
    else cost2[x]+=cost;
  }
  void add(int x,bool f,int y,bool g,T cost){
    assert(!is_overflow_mul<T>(n,cost));
    if(x==y){
      if(f==g)add(x,f,cost);
      return;
    }
    if(x>y){
      std::swap(x,y);
      std::swap(f,g);
    }
    cost3.emplace_back(x,y,f,g,cost);
  }
  T solve(){
    std::sort(cost3.begin(),cost3.end());
    T cost[2][2];
    UnionFind uf(n*2);
    for(int i=0;i<(int)cost3.size();){
      int j=i;
      cost[0][0]=cost[0][1]=cost[1][0]=cost[1][1]=0;
      while(j<(int)cost3.size()&&cost3[i]==cost3[j]){
        cost[cost3[j].f][cost3[j].g]+=cost3[j].cost;
        j++;
      }
      int x=cost3[i].x,y=cost3[i].y;
      if(cost[0][0]+cost[1][1]>cost[0][1]+cost[1][0])uf.merge(x,y+n),uf.merge(x+n,y);
      if(cost[0][0]+cost[1][1]<cost[0][1]+cost[1][0])uf.merge(x,y),uf.merge(x+n,y+n);
      i=j;
    }
    for(int i=0;i<n;i++)assert(!uf.same(i,i+n));
    std::vector<bool>vis(n,false);
    for(std::vector<int>g:uf.get_all()){
      if(g[0]>=n||vis[g[0]])continue;
      for(int i:g){
        if(i<n)vis[i]=true;
        else{
          vis[i-n]=true;
          flip[i-n]=true;
        }
      }
    }
    for(int i=0;i<n;i++)if(flip[i])std::swap(cost1[i],cost2[i]);
    std::make_unsigned_t<T>ans=0;
    for(int i=0;i<(int)cost3.size();){
      int j=i;
      cost[0][0]=cost[0][1]=cost[1][0]=cost[1][1]=0;
      while(j<(int)cost3.size()&&cost3[i]==cost3[j]){
        cost[cost3[j].f][cost3[j].g]+=cost3[j].cost;
        j++;
      }
      int x=cost3[i].x,y=cost3[i].y;
      if(flip[x])std::swap(cost[0][0],cost[1][0]),std::swap(cost[0][1],cost[1][1]);
      if(flip[y])std::swap(cost[0][0],cost[0][1]),std::swap(cost[1][0],cost[1][1]);
      ans+=cost[0][0]+cost[1][1];
      cost2[x]-=cost[0][0]-cost[1][0];
      cost2[y]-=cost[1][0];
      cost1[y]-=cost[1][1];
      g.add_edge(y,x,cost[0][1]+cost[1][0]-cost[0][0]-cost[1][1]);
      i=j;
    }
    for(int i=0;i<n;i++){
      T mn=std::min(cost1[i],cost2[i]);
      ans+=mn;
      cost1[i]-=mn;
      cost2[i]-=mn;
      if(cost1[i]>0)g.add_edge(n,i,cost1[i]);
      if(cost2[i]>0)g.add_edge(i,n+1,cost2[i]);
    }
    return ans+g.flow(n,n+1);
  }
  std::vector<bool>restore(){
    std::vector<bool>cut=g.min_cut(n);
    cut.resize(n);
    for(int i=0;i<n;i++)if(flip[i])cut[i]=!cut[i];
    return cut;
  }
};