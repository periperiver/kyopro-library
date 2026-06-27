#pragma once
#include<vector>
#include<array>
#include<algorithm>
#include<numeric>
#include<cassert>
#include "point3d.hpp"
namespace static_convex_hull3d_impl{
struct face_p{
  std::array<int,3>vs;
  std::array<face_p*,3>es;
  int head=-2;
  inline void change(int v,face_p*f){
    for(int i=0;i<3;i++)if(vs[i]==v)es[i]=f;
  }
};
struct face{
  std::array<int,3>vs;
  std::array<int,3>es;
};
template<typename T,typename T3>
inline T3 dotT3(const Point3d<T>&a,const Point3d<T>&b){
  return T3(a.x)*T3(b.x)+T3(a.y)*T3(b.y)+T3(a.z)*T3(b.z);
}
//T3=abs(x)*abs(y)*abs(z)*24くらい
//10^6だとint128が必要
template<typename T,typename T3>
std::vector<face>static_convex_hull3d(const std::vector<Point3d<T>>&a){
  int n=a.size();
  if(n==0)return {};
  std::vector<int>ord(n);
  std::iota(ord.begin(),ord.end(),0);
  std::sort(ord.begin(),ord.end(),[&](int lhs,int rhs){
    if(a[lhs].x!=a[rhs].x)return a[lhs].x<a[rhs].x;
    if(a[lhs].y!=a[rhs].y)return a[lhs].y<a[rhs].y;
    return a[lhs].z<a[rhs].z;
  });
  if(a[ord[0]]==a[ord[n-1]])return {};
  int A=ord[0],B=ord[n-1];
  int C=-1;
  for(int i=1;i<n-1;i++){
    Point3d<T>now=cross(a[ord[i]]-a[A],a[B]-a[A]);
    if(now!=Point3d<T>()){
      if(C==-1)C=ord[i];
      else{
        Point3d<T>pre=cross(a[C]-a[A],a[B]-a[A]);
        if(std::max({pre.x,pre.y,pre.z})<std::max({now.x,now.y,now.z}))C=ord[i];
      }
    }
  }
  if(C==-1)return {};
  int D=-1;
  Point3d<T>c=cross(a[B]-a[A],a[C]-a[A]);
  T3 mxval;
  for(int i=1;i<n-1;i++){
    Point3d<T>p=a[ord[i]]-a[A];
    T3 now=dotT3<T,T3>(p,c);
    if(now!=0){
      if(D==-1||mxval<now){
        D=ord[i],mxval=now;
      }
    }
  }
  if(D==-1){
    bool xd=true,yd=true,zd=true;
    Point3d<T>b=a[B]-a[A],c=a[C]-a[A];
    int key;
    if(b.x*c.y!=b.y*c.x)key=0;
    else if(b.y*c.z!=b.z*c.y)key=1;
    else if(b.z*c.x!=b.x*c.z)key=2;
    else assert(false);
    std::sort(ord.begin(),ord.end(),[&](int lhs,int rhs){
      if(key==0){
        if(a[lhs].x!=a[rhs].x)return a[lhs].x<a[rhs].x;
        return a[lhs].y<a[rhs].y;
      }
      else if(key==1){
        if(a[lhs].y!=a[rhs].y)return a[lhs].y<a[rhs].y;
        return a[lhs].z<a[rhs].z;
      }
      else{
        if(a[lhs].z!=a[rhs].z)return a[lhs].z<a[rhs].z;
        return a[lhs].x<a[rhs].x;
      }
    });
    std::vector<int>upper,lower;
    auto cross2d=[&](int i,int j,int k)->T {
      Point3d<T>x=a[j]-a[i],y=a[k]-a[i];
      if(key==0)return x.x*y.y-x.y*y.x;
      else if(key==1)return x.y*y.z-x.z*y.y;
      else return x.z*y.x-x.x*y.z;
    };
    for(int i:ord){
      while((int)upper.size()>=2){
        if(cross2d(upper[upper.size()-2],upper[upper.size()-1],i)>=0)upper.pop_back();
        else break;
      }
      while((int)lower.size()>=2){
        if(cross2d(lower[lower.size()-2],lower[lower.size()-1],i)<=0)lower.pop_back();
        else break;
      }
      upper.push_back(i);
      lower.push_back(i);
    }
    lower.insert(lower.end(),upper.rbegin()+1,upper.rend()-1);
    assert((int)lower.size()>=3);
    int cnt=lower.size()-2;
    std::vector<face>res(cnt*2);
    for(int i=0;i<cnt;i++){
      face f,g;
      f.vs[0]=lower[0];
      f.vs[1]=lower[i+1];
      f.vs[2]=lower[i+2];
      g.vs[0]=lower[0];
      g.vs[1]=lower[i+2];
      g.vs[2]=lower[i+1];
      f.es[0]=i==0?cnt:i-1;
      f.es[1]=cnt+i;
      f.es[2]=i+1==cnt?cnt*2-1:i+1;
      g.es[0]=i+1==cnt?cnt-1:cnt+i+1;
      g.es[1]=i;
      g.es[2]=i==0?0:cnt+i-1;
      res[i]=f;
      res[cnt+i]=g;
    }
    return res;
  }
  std::vector<std::vector<face_p>>fpool;
  fpool.emplace_back(n);
  int fp=0;
  auto newface_p=[&](int p,int q,int r)->face_p* {
    if(fp==n)fpool.emplace_back(n),fp=0;
    face_p*res=&fpool.back()[fp++];
    res->vs[0]=p,res->vs[1]=q,res->vs[2]=r;
    res->es.fill(nullptr);
    res->head=-1;
    return res;
  };
  std::vector<int>link(n);
  std::iota(link.begin(),link.end(),0);
  if(dotT3<T,T3>(a[D]-a[A],c)>0)std::swap(B,C);
  face_p*f1=newface_p(A,B,C),*f2=newface_p(B,A,D),*f3=newface_p(C,B,D),*f4=newface_p(A,C,D);
  f1->es[0]=f2,f1->es[1]=f3,f1->es[2]=f4;
  f2->es[0]=f1,f2->es[1]=f4,f2->es[2]=f3;
  f3->es[0]=f1,f3->es[1]=f2,f3->es[2]=f4;
  f4->es[0]=f1,f4->es[1]=f3,f4->es[2]=f2;
  std::vector<face_p*>st,st2,nface_p(n,nullptr);
  st.push_back(f1),st.push_back(f2),st.push_back(f3),st.push_back(f4);
  for(int i=0;i<n;i++){
    for(face_p*f:st){
      if(dotT3<T,T3>(a[i]-a[f->vs[0]],cross(a[f->vs[1]]-a[f->vs[0]],a[f->vs[2]]-a[f->vs[0]]))>0){
        if(f->head!=-1)std::swap(link[f->head],link[i]);
        else f->head=i;
        break;
      }
    }
  }
  while(!st.empty()){
    face_p*f=st.back();st.pop_back();
    if(f->head<0)continue;
    int mx=f->head;
    Point3d<T>c=cross(a[f->vs[1]]-a[f->vs[0]],a[f->vs[2]]-a[f->vs[0]]);
    for(int i=link[f->head];i!=f->head;i=link[i]){
      T3 pre=dotT3<T,T3>(a[mx]-a[f->vs[0]],c);
      T3 nxt=dotT3<T,T3>(a[i]-a[f->vs[0]],c);
      if(pre<nxt)mx=i;
    }
    const Point3d<T>&p=a[mx];
    st2.clear();
    st2.push_back(f);
    int start=-1;
    int nvs=-1;
    for(int i=0;i<(int)st2.size();i++){
      face_p*g=st2[i];
      if(g->head==-2)continue;
      if(nvs==-1)nvs=g->head;
      else if(g->head!=-1)std::swap(link[nvs],link[g->head]);
      g->head=-2;
      for(int j=0;j<3;j++){
        face_p*adj=g->es[j];
        if(adj->head==-2)continue;
        T3 d=dotT3<T,T3>(p-a[adj->vs[0]],cross(a[adj->vs[1]]-a[adj->vs[0]],a[adj->vs[2]]-a[adj->vs[0]]));
        if(d>0)st2.push_back(adj);
        else{
          face_p*g2=newface_p(mx,g->vs[j],g->vs[(j+1)%3]);
          start=g->vs[j];
          adj->change(g->vs[(j+1)%3],g2);
          g2->es[1]=adj;
          nface_p[g->vs[j]]=g2;
        }
      }
    }
    assert(start!=-1);
    if(nvs!=-1){
      int u=nvs;
      do{
        int lu=link[u];
        link[u]=u;
        int v=start;
        do{
          face_p*now=nface_p[v];
          if(dotT3<T,T3>(a[u]-a[now->vs[0]],cross(a[now->vs[1]]-a[now->vs[0]],a[now->vs[2]]-a[now->vs[0]]))>0){
            if(now->head==-1)now->head=u;
            else std::swap(link[now->head],link[u]);
            break;
          }
          v=now->vs[2];
        }while(v!=start);
        u=lu;
      }while(u!=nvs);
    }
    int v=start;
    do{
      face_p*now=nface_p[v];
      face_p*nxt=nface_p[now->vs[2]];
      now->es[2]=nxt;
      nxt->es[0]=now;
      v=now->vs[2];
      st.push_back(now);
    }while(v!=start);
  }
  std::vector<face_p*>ptrs;
  for(int i=0;i<(int)fpool.size();i++)for(int j=0;j<n;j++)if(fpool[i][j].head!=-2)ptrs.push_back(&fpool[i][j]);
  std::sort(ptrs.begin(),ptrs.end());
  std::vector<face>res(ptrs.size());
  for(const std::vector<face_p>&fv:fpool){
    for(const face_p&f:fv)if(f.head!=-2){
      int id=std::lower_bound(ptrs.begin(),ptrs.end(),&f)-ptrs.begin();
      res[id].vs=f.vs;
      for(int i=0;i<3;i++){
        res[id].es[i]=std::lower_bound(ptrs.begin(),ptrs.end(),f.es[i])-ptrs.begin();
      }
    }
  }
  return res;
}
}
using static_convex_hull3d_impl::static_convex_hull3d;