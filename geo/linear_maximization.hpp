#pragma once
#include<queue>
#include<cassert>
#include<ranges>
#include "static_convex_hull3d.hpp"
#include "argsort.hpp"
#include "../datastructure/dynamic/top_down_splay.hpp"
#include "../math/util.hpp"
template<typename T,typename T3>
std::vector<T>linear_maximization(std::vector<Point3d<T>>points,std::vector<Point3d<T>>query){
  static constexpr T minf=std::numeric_limits<T>::min();
  struct node{

  };
  std::sort(points.begin(),points.end()),points.erase(std::unique(points.begin(),points.end()),points.end());
  int n=points.size(),q=query.size();
  auto ch_f=static_convex_hull3d<T,T3>(points);
  if(ch_f.empty()){
    std::vector<T>res(q,inf);
    if(n==0)return res;
    for(auto [i,p]:query|std::views::enumerate){
      for(Point3d<T>q:{points[0],points.back()}){
        T now=dot(p,q);
        if(res[i]<now)res[i]=now;
      }
    }
    return res;
  }
  std::vector<Point<T>>diryz;
  std::vector<std::tuple<int,int,int>>ch;
  {
    std::vector<bool>vis(n,false);
    std::vector<int>link(n);
    auto sign=[](T x)->int {return x>0?1:(x<0?-1:0);};
    for(int i=0;i<n;i++)if(!vis[i]){
      vis[i]=true;
      const Point3d<T>&a=points[ch_f[i].vs[0]],&b=points[ch_f[i].vs[1]],&c=points[ch_f[i].vs[2]];
      Point3d<T>d=cross(b-a,c-a);
      if(d.y==0&&d.z==0){
        std::queue<int>que;
        que.push(i);
        int start=-1;
        while(!que.empty()){
          int u=que.front();que.pop();
          for(int j=0;j<3;j++){
            int v=ch_f[u].es[j];
            const Point3d<T>&a2=points[ch_f[v].vs[0]],&b2=points[ch_f[v].vs[1]],&c2=points[ch_f[v].vs[2]];
            Point3d<T>d2=cross(b2-a2,c2-a2);
            if(d2.y==0&&d2.z==0&&sign(d.x)==sign(d2.x)){
              if(vis[v])continue;
              vis[v]=true;
              que.push(v);
            }
            else{
              link[ch_f[u].vs[j]]=ch_f[u].vs[(j+1)%3];
              start=ch_f[u].vs[j];
            }
          }
        }
        assert(start!=-1);
        int u=start;
        int sig=sign(d.x);
        do{
          int v=link[u];
          int w=link[v];
          const Point3d<T>&up=points[u],&vp=points[v];
          diryz.emplace_back(vp.z-up.z,vp.y-up.y)*=sig;
          if(sig>0){
            
          }
          else{

          }
          u=v;
          diryz.emplace_back()
        }while(u!=start);
      }
      else{

      }
    }
  }
}