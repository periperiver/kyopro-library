#pragma once
#include "point.hpp"
#include<vector>
#include<algorithm>
#include<cassert>
template<typename T>
std::pair<int,int> closest_pair_points(const std::vector<Point<T>>&points){
  int n=points.size();
  assert(n>=2);
  T dist=dot(points[0]-points[1],points[0]-points[1]);
  std::pair<int,int>ans{0,1};
  std::vector<int>ord(n);
  std::iota(ord.begin(),ord.end(),0);
  std::vector<int>near,buf;
  buf.reserve(n);
  near.reserve(n);
  auto dfs=[&](auto self,int l,int r)->void {
    if(l+1==r)return;
    int mid=(l+r)/2;
    std::nth_element(ord.begin()+l,ord.begin()+mid,ord.begin()+r,[&](int i,int j){return points[i].x<points[j].x;});
    T mx=points[ord[mid]].x;
    self(self,l,mid);
    self(self,mid,r);
    for(int i=l;i<mid;i++){
      T dx=mx-points[ord[i]].x;
      if(dx*dx<dist)near.push_back(ord[i]);
    }
    for(int i=mid,j=0;i<r;i++){
      T dx=points[ord[i]].x-mx;
      if(dx*dx>=dist)continue;
      while(j<(int)near.size()){
        T dy=points[ord[i]].y-points[near[j]].y;
        if(dy>0&&dy*dy>=dist)j++;
        else break;
      }
      if(j==(int)near.size())break;
      for(int k=j;k<(int)near.size();k++){
        T dy=points[near[k]].y-points[ord[i]].y;
        if(dy*dy>=dist)break;
        T dx=points[near[k]].x-points[ord[i]].x;
        T ndist=dx*dx+dy*dy;
        if(dist>ndist){
          dist=ndist;
          ans=std::make_pair(ord[i],near[k]);
        }
      }
    }
    int p=l,q=mid;
    while(p<mid||q<r){
      if(p==mid)buf.push_back(ord[q++]);
      else if(q==r)buf.push_back(ord[p++]);
      else if(points[ord[p]].y<points[ord[q]].y)buf.push_back(ord[p++]);
      else buf.push_back(ord[q++]);
    }
    std::copy(buf.begin(),buf.end(),ord.begin()+l);
    buf.clear();
    near.clear();
  };
  dfs(dfs,0,n);
  return ans;
}