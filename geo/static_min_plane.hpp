#pragma once
#include<cassert>
#include "static_convex_hull3d.hpp"
#include "../datastructure/dynamic/top_down_splay.hpp"
namespace static_min_plane_impl{
struct node{
  node *left=nullptr,*right=nullptr,*pre=nullptr,*nxt=nullptr;
  int idx=-1;
};
template<typename T>
inline T iceil(T a,T b){
  if(a>=0)return (a+b-1)/b;
  else return a/b;
}
template<typename T,typename T3>
std::vector<T>static_min_plane(std::vector<std::tuple<T,T,T>>points,std::vector<std::pair<T,T>>query){
  static constexpr T inf=std::numeric_limits<T>::max();
  assert(!points.empty());
  std::sort(points.begin(),points.end()),points.erase(std::unique(points.begin(),points.end()),points.end());
  int n=points.size(),q=query.size();
  std::vector<Point3d<T>>pts(n);
  for(int i=0;i<n;i++){
    auto [x,y,z]=points[i];
    pts[i]=Point3d<T>(x,y,z);
  }
  std::vector<std::tuple<int,int,int>>cht=static_convex_hull3d<T,T3>(pts);
  if(cht.empty()){
    std::vector<T>res(q,inf);
    for(int i=0;i<q;i++){
      for(const auto&[a,b,c]:{points[0],points[n-1]}){
        T now=a*query[i].first+b*query[i].second+c;
        if(res[i]>now)res[i]=now;
      }
    }
    return res;
  }
  std::erase_if(cht,[&](std::tuple<int,int,int>ijk){
    auto [i,j,k]=ijk;
    Point3d<T>u=pts[j]-pts[i],v=pts[k]-pts[i];
    return u.x*v.y-u.y*v.x>=0;
  });
  std::vector<bool>use(n,false);
  for(const auto&[i,j,k]:cht)use[i]=use[j]=use[k]=true;
  auto slope=[&](const std::tuple<int,int,int>&ijk)->T {
    const auto&[i,j,k]=ijk;
    Point3d<T>c=cross(pts[j]-pts[i],pts[k]-pts[i]);
    T num=-c.y,den=c.z;
    if(den<0){
      num=-num;
      den=-den;
    }
    return iceil(num,den);
  };
  std::sort(cht.begin(),cht.end(),[&](const std::tuple<int,int,int>&ijkl,const std::tuple<int,int,int>&ijkr){
    return slope(ijkl)<slope(ijkr);
  });
  std::vector<int>ord(q);
  std::iota(ord.begin(),ord.end(),0);
  std::sort(ord.begin(),ord.end(),[&](int lhs,int rhs){
    return query[lhs].second>query[rhs].second;
  });
  std::vector<T>res(q);
  int ptr=0;
  std::vector<node>nds(n);
  node*root=nullptr;
  for(int i=0;i<n;i++)if(use[i]){
    nds[i].idx=i;
    nds[i].left=nds[i].pre=root;
    if(root)root->nxt=&nds[i];
    root=&nds[i];
    if(root->left){
      while(root->left->left){
        Point3d<T>a=pts[root->left->idx]-pts[root->left->left->idx],b=pts[root->idx]-pts[root->left->left->idx];
        if(a.x*b.y-a.y*b.x<=0){
          root->left->idx=-1;
          node*lft=root->left->left;
          lft->nxt=root;
          root->left=root->pre=lft;
        }
        else break;
      }
    }
  }
  for(int qid:ord){
    while(ptr<(int)cht.size()){
      if(slope(cht[ptr])>-query[qid].second)break;
      int i,j,k;
      std::tie(i,j,k)=cht[ptr];
      if(i>j)std::swap(i,j);
      if(j>k)std::swap(j,k);
      if(i>j)std::swap(i,j);
      root=top_down_splay(root,[&](node*nd)->int {
        if(nd->idx==j)return 0;
        else if(j<nd->idx)return nd->left?-1:0;
        else return nd->right?1:0;
      });
      if(root->idx==j){
        root->pre->nxt=root->nxt;
        root->nxt->pre=root->pre;
        root->right=left_most<node,false>(root->right);
        root->right->left=root->left;
        root=root->right;
      }
      else{
        nds[j].idx=j;
        if(root->idx<j){
          nds[j].pre=root;
          nds[j].left=nullptr;
          nds[j].right=root->right;
          nds[j].nxt=root->nxt;
          root->nxt=&nds[j];
          nds[j].nxt->pre=&nds[j];
          root->right=&nds[j];
        }
        else{
          nds[j].nxt=root;
          nds[j].right=nullptr;
          nds[j].left=root->left;
          nds[j].pre=root->pre;
          root->pre=&nds[j];
          nds[j].pre->nxt=&nds[j];
          root->left=&nds[j];
        }
      }
      ptr++;
    }
    T x,y;
    std::tie(x,y)=query[qid];
    root=top_down_splay(root,[&](node*nd)->int {
      auto [a,b,c]=points[nd->idx];
      T now=a*x+b*y+c;
      if(nd->pre){
        auto [la,lb,lc]=points[nd->pre->idx];
        if(now>la*x+lb*y+lc)return -1;
      }
      if(nd->nxt){
        auto [ra,rb,rc]=points[nd->nxt->idx];
        if(now>ra*x+rb*y+rc)return 1;
      }
      return 0;
    });
    auto [a,b,c]=points[root->idx];
    res[qid]=a*x+b*y+c;
  }
  return res;
}
}
using static_min_plane_impl::static_min_plane;