#pragma once
#include "io128.hpp"
#include<queue>
#include<cassert>
#include "static_convex_hull3d.hpp"
#include "argsort.hpp"
#include "../datastructure/dynamic/splay.hpp"
#include "../math/util.hpp"
#include "../math/cyclic_ternary_search.hpp"
namespace linear_maximization_impl{
struct node{
  node *left,*right,*par,*pre,*nxt;
  int idx;
  node():left(nullptr),right(nullptr),par(nullptr),pre(nullptr),nxt(nullptr),idx(-1){}
};
template<typename T>
struct Event{
  std::array<int,3>vs;
  Point<T>dir;
  bool operator<(const Event&rhs)const{return arg_comp(dir,rhs.dir);}
};
template<typename T>
std::vector<int>yz_lower_hull(const std::vector<Point3d<T>>&points){
  std::vector<int>res;
  for(auto [i,c]:points|std::views::enumerate){
    while((int)res.size()>=2){
      const Point3d<T>&a=points[res[res.size()-2]],&b=points[res[res.size()-1]];
      Point<T>A(b.y-a.y,b.z-a.z),B(c.y-a.y,c.z-a.z);
      if(cross(A,B)<=0)res.pop_back();
      else break;
    }
    res.push_back(i);
  }
  return res;
}
template<typename T>
std::vector<int>yz_convex_hull(std::vector<Point3d<T>>points){
  if((int)points.size()==1)return {0};
  std::vector<int>lower_hull=yz_lower_hull(points);
  std::reverse(points.begin(),points.end());
  for(Point3d<T>&p:points)p*=-1;
  std::vector<int>upper_hull=yz_lower_hull(points);
  for(int&x:upper_hull)x=points.size()-x-1;
  lower_hull.insert(lower_hull.end(),upper_hull.begin()+1,upper_hull.end()-1);
  return lower_hull;
}
template<typename T,typename T4>
std::vector<T>linear_maximization(std::vector<Point3d<T>>points,std::vector<Point3d<T>>query){
  static constexpr T minf=std::numeric_limits<T>::min();
  static constexpr T inf=std::numeric_limits<T>::max();
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
  std::vector<Event<T4>>events;
  for(auto f:ch_f){
    const Point3d<T>&a=points[f.vs[0]],&b=points[f.vs[1]],&c=points[f.vs[2]];
    Point3d<T>d=cross(b-a,c-a);
    if(d.y==0&&d.z==0)continue;
    for(int&x:f.vs)x++;
    events.push_back({f.vs,Point<T4>(d.y,d.z)});
  }
  auto boundary_hull=[&](std::vector<int>idx,int aux)->void {
    if((int)idx.size()==1)return;
    for(int i=0;i<(int)idx.size();i++){
      Event<T4>e;
      e.vs[0]=idx[i]+1;
      e.vs[1]=idx[(i+1)%idx.size()]+1;
      e.vs[2]=aux;
      const Point3d<T>&a=points[e.vs[0]-1],&b=points[e.vs[1]-1];
      e.dir=Point<T4>(b.z-a.z,a.y-b.y);
      events.push_back(std::move(e));
    }
  };
  boundary_hull(yz_convex_hull(std::vector<Point3d<T>>(points.begin(),std::partition_point(points.begin(),points.end(),[&](const Point3d<T>&p){return p.x==points[0].x;}))),0);
  boundary_hull([&](){
    auto itr=std::partition_point(points.begin(),points.end(),[&](const Point3d<T>&p){return p.x!=points.back().x;});
    std::vector<int>res=yz_convex_hull(std::vector<Point3d<T>>(itr,points.end()));
    for(int&x:res)x+=itr-points.begin();
    return res;
  }(),n+1);
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
  std::cerr<<"init v"<<std::endl;
  for(int i=0;i<(int)init_v.size();i++)std::cerr<<init_v[i]<<" \n"[i+1==(int)init_v.size()];
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
    std::cerr<<"insert"<<id<<std::endl;
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
    // std::cerr<<"insert"<<id<<std::endl;
  };
  auto erase=[&](int id)->void {
    if(!use[id])return;
    std::cerr<<"erase"<<id<<std::endl;
    use[id]=false;
    splay(&nds[id]);
    nds[id].pre->nxt=nds[id].nxt;
    nds[id].nxt->pre=nds[id].pre;
    nds[id].left->par=nds[id].right->par=nullptr;
    root=merge(nds[id].left,nds[id].right);
    // std::cerr<<"erase"<<id<<std::endl;
  };
  auto print_bst=[&]()->void {
    std::cerr<<"print bst---------------------------"<<std::endl;
    std::cerr<<"root:"<<root->idx<<std::endl;
    std::vector<bool>seen(n+2);
    auto dfs=[&](auto self,node *nd)->void {
      if(!nd)return;
      assert(!seen[nd-&nds[0]]);
      seen[nd-&nds[0]]=true;
      std::cerr<<"index:"<<nd->idx<<std::endl;
      std::cerr<<"\tleft:"<<(nd->left?nd->left->idx:-1)<<std::endl;
      std::cerr<<"\tright:"<<(nd->right?nd->right->idx:-1)<<std::endl;
      std::cerr<<"\tpar:"<<(nd->par?nd->par->idx:-1)<<std::endl;
      std::cerr<<"\tpre:"<<(nd->pre?nd->pre->idx:-1)<<std::endl;
      std::cerr<<"\tnxt:"<<(nd->nxt?nd->nxt->idx:-1)<<std::endl;
      self(self,nd->left);
      self(self,nd->right);
    };
    dfs(dfs,root);
    std::cerr<<"print bst---------------------------"<<std::endl;
  };
  for(int i:arg_sort(q_dir)){
    while(ptr<(int)events.size()){
      Event e=events[ptr];
      if(arg_comp(e.dir,q_dir[i])){
        ptr++;
        Point<T4>dir90=e.dir.rot90();
        for(int j=0;j<3;j++){
          if(e.vs[j]==0||e.vs[j]==n+1)continue;
          const Point3d<T>&p=points[e.vs[j]-1];
          buf[e.vs[j]]=Point<T4>(p.x,p.y*dir90.x+p.z*dir90.y);
        }
        std::sort(e.vs.begin(),e.vs.end(),[&](int lhs,int rhs){return buf[lhs]<buf[rhs];});
        if(e.vs[0]==0){
          erase(e.vs[1]);
          insert(e.vs[2]);
        }
        else if(e.vs[2]==n+1){
          erase(e.vs[0]);
          insert(e.vs[1]);
        }
        else{
          std::cerr<<"vs"<<std::endl;
          for(int j=0;j<3;j++)std::cerr<<e.vs[j]<<" \n"[j+1==3];
          std::cerr<<"point"<<std::endl;
          for(int j=0;j<3;j++)std::cerr<<buf[e.vs[j]]<<" \n"[j+1==3];
          std::cerr<<"cross"<<std::endl;
          std::cerr<<cross(buf[e.vs[1]]-buf[e.vs[0]],buf[e.vs[2]]-buf[e.vs[0]])<<std::endl;
          if(cross(buf[e.vs[1]]-buf[e.vs[0]],buf[e.vs[2]]-buf[e.vs[0]])>=0)erase(e.vs[1]);
          else insert(e.vs[1]);
        }
      }
      else break;
    }
    for(int j=0;j<n+2;j++)if(use[j]){
      std::cerr<<j<<' '<<eval(j,i)<<std::endl;
    }
    print_bst();
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