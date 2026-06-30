#pragma once
#include "io128.hpp"
#include<queue>
#include<cassert>
#include "static_convex_hull3d.hpp"
#include "argsort.hpp"
#include "../datastructure/dynamic/splay.hpp"
#include "../math/util.hpp"
namespace linear_maximization_impl{
struct node{
  node *left,*right,*par,*pre,*nxt;
  int idx;
  node():left(nullptr),right(nullptr),par(nullptr),pre(nullptr),nxt(nullptr),idx(-1){}
};
template<typename T>
struct Event{
  std::vector<int>vs;
  Point<T>dir;
  bool operator<(const Event&rhs)const{return arg_comp(dir,rhs.dir);}
};
template<typename T,typename T4>
std::vector<T>linear_maximization(std::vector<Point3d<T>>points,std::vector<Point3d<T>>query){
  static constexpr T minf=std::numeric_limits<T>::min();
  static constexpr T inf=std::numeric_limits<T>::max();
  std::sort(points.begin(),points.end()),points.erase(std::unique(points.begin(),points.end()),points.end());
  int n=points.size(),q=query.size();
  auto ch=static_convex_hull3d<T,T4>(points);
  if(ch.empty()){
    std::vector<T>res(q,minf);
    if(n==0)return res;
    for(auto [i,p]:query|std::views::enumerate){
      for(Point3d<T>q:{points[0],points.back()}){
        T now=dot(p,q);
        if(res[i]<now)res[i]=now;
      }
    }
    return res;
  }
  auto chp=reduce_degenerate<T>(points,ch);
  std::vector<Event<T4>>events;
  auto boundary_hull=[&](std::vector<int>idx,int aux)->void {
    if((int)idx.size()==1)return;
    if(aux==0)std::reverse(idx.begin(),idx.end());
    for(int i=0;i<(int)idx.size();i++){
      Event<T4>e;
      e.vs={idx[i]+1,idx[(i+1)%idx.size()]+1,aux};
      const Point3d<T>&a=points[e.vs[0]-1],&b=points[e.vs[1]-1];
      e.dir=Point<T4>(b.z-a.z,a.y-b.y);
      events.push_back(std::move(e));
    }
  };
  for(auto f:chp){
    const Point3d<T>&a=points[f.vs[0]],&b=points[f.vs[1]],&c=points[f.vs[2]];
    Point3d<T>d=cross(b-a,c-a);
    if(d.y==0&&d.z==0)boundary_hull(f.vs,d.x>0?n+1:0);
    else{
      for(int&x:f.vs)x++;
      events.push_back({f.vs,Point<T4>(d.y,d.z)});
    }
  }
  std::sort(events.begin(),events.end());
  std::vector<int>init_v{0};
  for(auto [i,c]:points|std::views::enumerate){
    if(i>0&&points[i-1].x==c.x&&points[i-1].y==c.y)continue;
    while((int)init_v.size()>=3){
      const Point3d<T>&a=points[init_v[init_v.size()-2]-1],&b=points[init_v[init_v.size()-1]-1];
      Point<T>A(b.x-a.x,b.y-a.y),B(c.x-a.x,c.y-a.y);
      if(cross(A,B)<=0)init_v.pop_back();
      else break;
    }
    init_v.push_back(i+1);
  }
  while((int)init_v.size()>=3){
    if(points[init_v[init_v.size()-2]-1].x==points.back().x)init_v.pop_back();
    else break;
  }
  init_v.push_back(n+1);
  std::vector<node>nds(n+2);
  for(int i=0;i<n+2;i++)nds[i].idx=i;
  node*root=&nds[0];
  for(int i=0;i<(int)init_v.size()-1;i++){
    node *lnd=&nds[init_v[i]];
    node *rnd=&nds[init_v[i+1]];
    lnd->nxt=rnd;
    rnd->pre=lnd;
    lnd->right=rnd;
    rnd->par=lnd;
  }
  std::vector<bool>use(n+2);
  for(int i=0;i<(int)init_v.size();i++)use[init_v[i]]=true;
  std::vector<Point<T4>>q_dir(q);
  for(auto [i,p]:query|std::views::enumerate)q_dir[i]=Point<T4>(p.y,p.z);
  std::vector<T>res(q);
  int ptr=0;
  auto eval=[&](int i,int j)->T {
    if(i==0||i==n+1)return minf;
    return dot(points[i-1],query[j]);
  };
  std::vector<Point<T4>>buf(n+2);
  buf[0].x=points[0].x-1;
  buf[n+1].x=points.back().x+1;
  auto insert=[&](int id)->void {
    if(use[id])return;
    use[id]=true;
    while(true){
      if(id<root->idx){
        if(!root->left)break;
        root=root->left;
      }
      else{
        if(!root->right)break;
        root=root->right;
      }
    }
    if(id<root->idx){
      root->left=&nds[id];
      nds[id].pre=root->pre;
      nds[id].nxt=root;
    }
    else{
      root->right=&nds[id];
      nds[id].nxt=root->nxt;
      nds[id].pre=root;
    }
    nds[id].par=root;
    nds[id].pre->nxt=nds[id].nxt->pre=&nds[id];
    nds[id].left=nds[id].right=nullptr;
    root=&nds[id];
    splay(root);
  };
  auto erase=[&](int id)->void {
    if(!use[id])return;
    use[id]=false;
    splay(&nds[id]);
    nds[id].pre->nxt=nds[id].nxt;
    nds[id].nxt->pre=nds[id].pre;
    nds[id].left->par=nds[id].right->par=nullptr;
    root=merge(nds[id].left,nds[id].right);
  };
  auto apply=[&](const Event<T4>&e)->void {
    Point<T4>dir90=e.dir.rot90();
    for(int v:e.vs){
      const Point3d<T>&p=points[v-1];
      buf[v]=Point<T4>(p.x,p.y*dir90.x+p.z*dir90.y);
    }
    if(e.vs[2]==0||e.vs[2]==n+1){
      int u=e.vs[0],v=e.vs[1];
      if(buf[u]>buf[v])std::swap(u,v);
      erase(u);
      insert(v);
    }
    else{
      for(int j=0;j<(int)e.vs.size();j++){
        int u=e.vs[j],v=e.vs[(j+1)%e.vs.size()],w=e.vs[(j+2)%e.vs.size()];
        if(buf[u].x<buf[v].x||buf[v].x<buf[w].x)insert(v);
        else erase(v);
      }
    }
  };
  for(const Event<T4>&e:events)if(e.dir.x<0&&e.dir.y==0)apply(e);
  for(int i:arg_sort(q_dir)){
    while(ptr<(int)events.size()){
      if(arg_comp(events[ptr].dir,q_dir[i]))apply(events[ptr++]);
      else break;
    }
    while(true){
      T now=eval(root->idx,i);
      if(root->left){
        T pre=eval(root->pre->idx,i);
        if(now<pre){
          root=root->left;
          continue;
        }
      }
      if(root->right){
        T nxt=eval(root->nxt->idx,i);
        if(now<nxt){
          root=root->right;
          continue;
        }
      }
      break;
    }
    splay(root);
    res[i]=eval(root->idx,i);
  }
  return res;
}
}
using linear_maximization_impl::linear_maximization;