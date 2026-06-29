#pragma once
#include<queue>
#include<cassert>
#include<ranges>
#include "static_convex_hull3d.hpp"
#include "argsort.hpp"
#include "../datastructure/dynamic/splay.hpp"
#include "../math/util.hpp"
#include "../math/cyclic_ternary_search.hpp"
template<typename T,typename T4>
std::vector<T>linear_maximization(std::vector<Point3d<T>>points,std::vector<Point3d<T>>query){
  static constexpr T minf=std::numeric_limits<T>::min();
  struct node{
    node *left,*right,*par,*pre,*nxt;
    int idx;
    node():left(nullptr),right(nullptr),par(nullptr),pre(nullptr),nxt(nullptr),idx(-1){}
  };
  std::sort(points.begin(),points.end()),points.erase(std::unique(points.begin(),points.end()),points.end());
  int n=points.size(),q=query.size();
  auto ch_f=static_convex_hull3d<T,T4>(points);
  if(ch_f.empty()){
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
  Point3d<T>C=cross(points[1]-points[0],points[2]-points[0]);
  if(std::all_of(points.begin()+3,points.end(),[&](const Point3d<T>&p){
    T4 sum=0;
    sum+=T4(C.x)*T4(p.x);
    sum+=T4(C.y)*T4(p.y);
    sum+=T4(C.z)*T4(p.z);
    return sum==0;
  })){
    std::vector<T>res(q,minf);
    std::vector<Point3d<T>>ch;
    ch.push_back(points[ch_f[0].vs[0]]);
    for(int i=0;i<(int)ch_f.size()/2;i++)ch.push_back(points[ch_f[i].vs[1]]);
    ch.push_back(points[ch_f[ch_f.size()/2-1].vs[2]]);
    for(auto [i,p]:query|std::views::enumerate){
      res[i]=-cyclic_ternary_search(n,[&](int id){return -dot(p,ch[id]);}).second;
    }
    return res;
  }
  std::vector<Point<T4>>diryz;
  std::vector<std::tuple<int,int,int>>ch;
  std::vector<int>init_v;
  auto perpendicular=[&](int id)->Point3d<T> {
    const Point3d<T>&a=points[ch_f[id].vs[0]],&b=points[ch_f[id].vs[1]],&c=points[ch_f[id].vs[2]];
    return cross(b-a,c-a);
  };
  {
    std::vector<bool>vis(n,false);
    std::vector<int>link(n);
    auto sign=[](T x)->int {return x>0?1:(x<0?-1:0);};
    for(int i=0;i<n;i++)if(!vis[i]){
      vis[i]=true;
      Point3d<T>d=perpendicular(i);
      if(d.y==0&&d.z==0){
        std::queue<int>que;
        que.push(i);
        int start=-1;
        while(!que.empty()){
          int u=que.front();que.pop();
          for(int j=0;j<3;j++){
            int v=ch_f[u].es[j];
            Point3d<T>d2=perpendicular(v);
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
        auto aux_2d=[&](int u,int v)->Point<T4> {
          const Point3d<T>&up=points[u],&vp=points[v];
          return Point<T4>(vp.z-up.z,vp.y-up.y)*sig;
        };
        do{
          int v=link[u];
          int w=link[v];
          Point<T4>uv=aux_2d(u,v),vw=aux_2d(v,w);
          if(sig<0)std::swap(uv,vw);
          if(arg_comp(vw,uv)){
            init_v.push_back(w);
          }
          diryz.push_back(uv);
          if(sig<0)ch.emplace_back(u,v,n+2);
          else ch.emplace_back(v,u,n+3);
          u=v;
        }while(u!=start);
      }
      else{
        Point<T4>now(d.y,d.z);
        for(int j=0;j<3;j++){
          int ni=ch_f[i].es[j];
          Point3d<T4>d2=perpendicular(ni).template convert<T4>();
          if(d2.y==0&&d2.z==0)continue;
          Point<T4>adj(d2.y,d2.z);
          if(arg_comp(now,adj)&&cross(now,adj)<0){
            init_v.push_back(ch_f[i].vs[j]);
            init_v.push_back(ch_f[i].vs[(j+1)%3]);
          }
        }
        diryz.emplace_back(now);
        int a=ch_f[i].vs[0],b=ch_f[i].vs[1],c=ch_f[i].vs[2];
        if(a>b)std::swap(a,b);
        if(b>c)std::swap(b,c);
        if(a>b)std::swap(a,b);
        if(points[a].x==points[b].x){
          if(d.z>0||(d.y<0&&d.z==0))std::swap(a,b);
        }
        else if(points[b].x==points[c].x){
          if(d.z>0||(d.y<0&&d.z==0))std::swap(b,c);
        }
        ch.emplace_back(a,b,c);
      }
    }
  }
  for(int i=0;i<q;i++){
    diryz.emplace_back(Point<T4>(query[i].y,query[i].z));
  }
  std::sort(init_v.begin(),init_v.end(),[&](int lhs,int rhs){return points[lhs].x<points[rhs].x;});
  init_v.erase(std::unique(init_v.begin(),init_v.end()),init_v.end());
  init_v.insert(init_v.begin(),n+2);
  init_v.push_back(n+3);
  std::vector<node>nds(n+4);
  for(int i=0;i<n+4;i++)nds[i].idx=i;
  std::vector<bool>use(n+4);
  node *root=&nds[init_v[0]];
  for(int i=0;i<(int)init_v.size()-1;i++){
    node*lnd=&nds[init_v[i]];
    node*rnd=&nds[init_v[i+1]];
    lnd->right=rnd;
    lnd->nxt=rnd;
    rnd->pre=lnd;
  }
  auto eval=[&](int i,int j)->T {
    if(i==n+2||i==n+3)return minf;
    return dot(points[i],query[j]);
  };
  for(int i=0;i<(int)init_v.size();i++)use[init_v[i]]=true;
  std::vector<T>res(q,minf);
  for(int i:arg_sort(diryz)){
    if(i<(int)ch.size()){
      auto [a,b,c]=ch[i];
      if(use[b]){
        splay(&nds[b]);
        nds[b].pre->nxt=nds[b].nxt;
        nds[b].nxt->pre=nds[b].pre;
        nds[b].left->par=nds[b].right->par=nullptr;
        root=merge(nds[b].left,nds[b].right);
        use[b]=false;
      }
      else{
        Point3d<T>target=points[b];
        while(true){
          if(b<root->idx){
            if(!root->left)break;
            root=root->left;
          }
          else{
            if(!root->right)break;
            root=root->right;
          }
        }
        if(b<root->idx){
          root->left=&nds[b];
          nds[b].par=root;
          nds[b].pre=root->pre;
          nds[b].nxt=root;
          nds[b].pre->nxt=&nds[b];
          nds[b].nxt->pre=&nds[b];
          root=&nds[b];
        }
        else{
          root->right=&nds[b];
          nds[b].par=root;
          nds[b].nxt=root->nxt;
          nds[b].pre=root;
          nds[b].pre->nxt=&nds[b];
          nds[b].nxt->pre=&nds[b];
          root=&nds[b];
        }
        splay(root);
      }
    }
    else{
      i-=ch.size();
      while(true){
        T now=eval(root->idx,i);
        if(root->pre){
          T pre=eval(root->pre->idx,i);
          if(pre>now){
            root=root->left;
            continue;
          }
        }
        if(root->nxt){
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
  }
  return res;
}