#pragma once
#include<vector>
#include<numeric>
#include<algorithm>
#include "point.hpp"
template<typename T>
struct CountPointsInTriangles{
private:
  std::vector<Point<T>>a;
  std::vector<std::vector<int>>in,on;
  std::vector<int>same;
  std::vector<int>pos;
public:
  CountPointsInTriangles(const std::vector<Point<T>>&a_,std::vector<Point<T>>b):a(a_),in(a.size(),std::vector<int>(a.size())),on(a.size(),std::vector<int>(a.size())),same(a.size()),pos(a.size()){
    int n=a.size(),m=b.size();
    Point<T>origin=[&]()->Point<T> {
      T mnx=0;
      for(const Point<T>&p:a){
        if(mnx>p.x)mnx=p.x;
      }
      mnx--;
      Point<T>res(mnx,0);
      while(true){
        bool ok=true;
        for(int i=0;i<n;i++)for(int j=i+1;j<n;j++)if(a[i]!=a[j]){
          if(cross(a[i]-res,a[j]-res)==0){
            ok=false;
            i=n;
            break;
          }
        }
        if(!ok){
          res.y++;
          continue;
        }
        for(int i=0;i<n;i++)for(int j=0;j<m;j++)if(a[i]!=b[j]){
          if(cross(a[i]-res,b[j]-res)==0){
            ok=false;
            i=n;
            break;
          }
        }
        if(!ok){
          res.y++;
          continue;
        }
        break;
      }
      return res;
    }();
    for(Point<T>&p:a)p-=origin;
    for(Point<T>&p:b)p-=origin;
    std::vector<int>ord(n);
    std::iota(ord.begin(),ord.end(),0);
    std::sort(ord.begin(),ord.end(),[&](int lhs,int rhs){return cross(a[lhs],a[rhs])>0;});
    a=[&](){
      std::vector<Point<T>>res(n);
      for(int i=0;i<n;i++)res[i]=a[ord[i]];
      return res;
    }();
    for(int i=0;i<n;i++)pos[ord[i]]=i;
    for(int i=0;i<n;i++)for(int j=0;j<m;j++)if(a[i]==b[j])same[i]++;
    for(int i=0;i<n;i++)for(int j=i+1;j<n;j++)if(a[i]!=a[j]){
      int &in_cnt=in[i][j],&on_cnt=on[i][j];
      for(const Point<T>&p:b)if(p!=a[i]&&p!=a[j]){
        T c=cross(a[j]-a[i],p-a[i]);
        if(c==0){
          if(dot(a[i]-p,a[j]-p)<0)on_cnt++;
        }
        else if(c>0){
          if(cross(a[i],p)>0&&cross(a[j],p)<0)in_cnt++;
        }
      }
    }
  }
  int query(int i,int j,int k)const{
    if(i==j||j==k||k==i)return 0;
    i=pos[i],j=pos[j],k=pos[k];
    if(i>j)std::swap(i,j);
    if(j>k)std::swap(j,k);
    if(i>j)std::swap(i,j);
    T c=cross(a[j]-a[i],a[k]-a[i]);
    if(c==0)return 0;
    else if(c>0){
      return in[i][j]+in[j][k]-in[i][k]-on[i][k];
    }
    else{
      return in[i][k]-in[i][j]-in[j][k]-on[i][j]-on[j][k]-same[j];
    }
  }
};