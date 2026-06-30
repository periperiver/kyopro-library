#pragma once
#include "io128.hpp"
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
  static constexpr T inf=std::numeric_limits<T>::max();
  struct node{
    node *left,*right,*par,*pre,*nxt;
    int idx;
    node():left(nullptr),right(nullptr),par(nullptr),pre(nullptr),nxt(nullptr),idx(-1){}
  };
  std::sort(points.begin(),points.end()),points.erase(std::unique(points.begin(),points.end()),points.end());
  int n=points.size(),q=query.size();
  auto ch_f=static_convex_hull3d<T,T4>(points);
  std::cerr<<"face\n";
  for(auto f:ch_f){
    std::cerr<<f.vs[0]<<' '<<f.vs[1]<<' '<<f.vs[2]<<std::endl;
    auto a=points[f.vs[0]],b=points[f.vs[1]],c=points[f.vs[2]];
    std::cerr<<"per:"<<cross(b-a,c-a)<<std::endl;
  }
  std::cerr<<"face end\n";
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
  if(std::all_of(points.begin()+3,points.end(),[&](Point3d<T>p){
    p-=points[0];
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
  auto perpendicular=[&](int id)->Point3d<T> {
    const Point3d<T>&a=points[ch_f[id].vs[0]],&b=points[ch_f[id].vs[1]],&c=points[ch_f[id].vs[2]];
    return cross(b-a,c-a);
  };
  {
    std::vector<bool>vis(ch_f.size(),false);
    std::vector<int>link(n);
    auto sign=[](T x)->int {return x>0?1:(x<0?-1:0);};
    for(int i=0;i<ch_f.size();i++)if(!vis[i]){
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
          return Point<T4>(vp.z-up.z,up.y-vp.y)*sig;
        };
        do{
          int v=link[u];
          Point<T4>uv=aux_2d(u,v);
          std::cerr<<"AAA "<<uv.convert()<<' '<<u<<' '<<v<<std::endl;
          std::cerr<<points[u]<<' '<<points[v]<<std::endl;
          diryz.push_back(uv);
          if(sig<0)ch.emplace_back(u,v,n);
          else ch.emplace_back(v,u,n+1);
          u=v;
        }while(u!=start);
      }
      else{
        Point<T4>now(d.y,d.z);
        // for(int j=0;j<3;j++){
        //   int ni=ch_f[i].es[j];
        //   Point3d<T4>d2=perpendicular(ni).template convert<T4>();
        //   if(d2.y==0&&d2.z==0)continue;
        //   Point<T4>adj(d2.y,d2.z);
        // }
        diryz.emplace_back(now);
        int a=ch_f[i].vs[0],b=ch_f[i].vs[1],c=ch_f[i].vs[2];
        // if(a>b)std::swap(a,b);
        // if(b>c)std::swap(b,c);
        // if(a>b)std::swap(a,b);
        // if(points[a].x==points[b].x){
        //   if(d.z>0||(d.y<0&&d.z==0))std::swap(a,b);
        // }
        // else if(points[b].x==points[c].x){
        //   if(d.z>0||(d.y<0&&d.z==0))std::swap(b,c);
        // }
        ch.emplace_back(a,b,c);
      }
    }
  }
  std::vector<Point<T4>>diryz_q(q+3);
  for(int i=0;i<q;i++)diryz_q[i]=Point<T4>(query[i].y,query[i].z);
  diryz_q[q]=Point<T4>(0,-1);
  diryz_q[q+1]=Point<T4>(0,1);
  diryz_q[q+2]=Point<T4>(1,0);
  query.emplace_back(0,0,-1);
  query.emplace_back(0,0,1);
  query.emplace_back(0,1,0);
  std::vector<int>init_v{n};
  for(int i=0;i<n;i++){
    const Point3d<T>&c=points[i];
    while((int)init_v.size()>2){
      const Point3d<T>&a=points[init_v[init_v.size()-2]],&b=points[init_v[init_v.size()-1]];
      Point<T>db(b.x-a.x,b.y-a.y);
      Point<T>dc(c.x-a.x,c.y-a.y);
      if(cross(db,dc)<=0)init_v.pop_back();
      else break;
    }
    init_v.push_back(i);
  }
  while((int)init_v.size()>2){
    int a=init_v[init_v.size()-2],b=init_v[init_v.size()-1];
    if(points[a].x==points[b].x)init_v.pop_back();
    else break;
  }
  init_v.push_back(n+1);
  std::vector<node>nds(n+2);
  for(int i=0;i<n+2;i++)nds[i].idx=i;
  std::vector<bool>use(n+2);
  node *root=&nds[init_v[0]];
  for(int i=0;i<(int)init_v.size()-1;i++){
    node*lnd=&nds[init_v[i]];
    node*rnd=&nds[init_v[i+1]];
    lnd->right=rnd;
    lnd->nxt=rnd;
    rnd->pre=lnd;
    rnd->par=lnd;
  }
  auto eval=[&](int i,int j)->T {
    if(i==n||i==n+1)return minf;
    return dot(points[i],query[j]);
  };
  for(int i=0;i<(int)init_v.size();i++)use[init_v[i]]=true;
  std::vector<T>res(q,minf);
  std::vector<int>ord_p=arg_sort(diryz);
  std::vector<int>ord_q=arg_sort(diryz_q);
  int ptr=0;
  std::cerr<<"DIRYZ\n";
  for(int i=0;i<diryz.size();i++)std::cerr<<diryz[i]<<std::endl;
  std::cerr<<"DIRYZ\n";
  for(int i:ord_q){
    const Point3d<T>&p=query[i];
    auto get_middle=[&](int a,int b,int c)->std::pair<int,bool> {
      std::array<Point<T4>,3>arr;
      arr[0]=Point<T4>(points[a].x,points[a].y*p.y+points[a].z*p.z);
      arr[1]=Point<T4>(points[b].x,points[b].y*p.y+points[b].z*p.z);
      if(c<n)arr[2]=Point<T4>(points[c].x,points[c].y*p.y+points[c].z*p.z);
      else if(c==n)arr[2]=Point<T4>(minf,0);
      else arr[2]=Point<T4>(inf,0);
      std::array<int,3>ord;
      std::iota(ord.begin(),ord.end(),0);
      std::sort(ord.begin(),ord.end(),[&](int lhs,int rhs){return arr[lhs]<arr[rhs];});
      Point<T4>A=arr[ord[0]],B=arr[ord[1]],C=arr[ord[2]];
      bool s;
      if(cross(B-A,C-A)>=0)s=false;
      else s=true;
      switch(ord[1]){
        case 0:return {a,s};
        case 1:return {b,s};
        case 2:return {c,s};
      }
      return {-1,0};
    };
    while(ptr<(int)ord_p.size()){
      int j=ord_p[ptr];
      if(arg_comp(diryz[j],diryz_q[i])){
        ptr++;
        auto [a,b,c]=ch[j];
        auto [bs,s]=get_middle(a,b,c);
        std::cerr<<a<<' '<<b<<' '<<c<<' '<<bs<<' '<<s<<std::endl;
        b=bs;
        if(!s){
          if(use[b]){
            splay(&nds[b]);
            root=&nds[b];
            assert(nds[b].pre);
            assert(nds[b].nxt);
            assert(nds[b].left);
            assert(nds[b].right);
            nds[b].pre->nxt=nds[b].nxt;
            nds[b].nxt->pre=nds[b].pre;
            nds[b].left->par=nds[b].right->par=nullptr;
            root=merge(nds[b].left,nds[b].right);
            use[b]=false;
            nds[b].left=nds[b].right=nds[b].par=nds[b].pre=nds[b].nxt=nullptr;
          }
        }
        else{
          if(!use[b]){
            Point3d<T>target=points[b];
            while(true){
              if(root->idx==n+1||(root->idx!=n&&b<root->idx)){
                if(!root->left)break;
                root=root->left;
              }
              else{
                if(!root->right)break;
                root=root->right;
              }
            }
            // std::cerr<<"root idx:"<<root->idx<<std::endl;
            if(root->idx==n+1||(root->idx!=n&&b<root->idx)){
              root->left=&nds[b];
              nds[b].par=root;
              nds[b].pre=root->pre;
              nds[b].nxt=root;
              root->pre=&nds[b];
              nds[b].pre->nxt=&nds[b];
              nds[b].nxt->pre=&nds[b];
              root=&nds[b];
            }
            else{
              root->right=&nds[b];
              nds[b].par=root;
              nds[b].nxt=root->nxt;
              nds[b].pre=root;
              root->nxt=&nds[b];
              nds[b].pre->nxt=&nds[b];
              nds[b].nxt->pre=&nds[b];
              root=&nds[b];
            }
            splay(root);
            use[b]=true;
          }
        }
      }
      else break;
    }
    while(true){
      T now=eval(root->idx,i);
      if(root->pre){
        T pre=eval(root->pre->idx,i);
        if(pre>now){
          if(!root->left)break;
          root=root->left;
          continue;
        }
      }
      if(root->nxt){
        T nxt=eval(root->nxt->idx,i);
        if(now<nxt){
          if(!root->right)break;
          root=root->right;
          continue;
        }
      }
      break;
    }
    splay(root);
    std::cerr<<"q index:"<<i<<std::endl;
    for(int i=0;i<n+2;i++)if(use[i])std::cerr<<i<<' ';std::cerr<<std::endl;
    if(i<q)res[i]=eval(root->idx,i);
  }
  return res;
}