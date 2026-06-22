#pragma once
#include<vector>
#include<algorithm>
#include<map>
#include<utility>
#include<numeric>
template<typename T>
std::vector<std::pair<std::pair<T,T>,std::pair<T,T>>>rectangle_decomposition(const std::vector<std::pair<T,T>>&points){
  int n=points.size();
  if(n<4)return {};
  struct E{
    T x;
    T l,r;
    bool operator<(const E&rhs)const{return x<rhs.x;}
  };
  static constexpr T inf=std::numeric_limits<T>::max();
  static constexpr T minf=std::numeric_limits<T>::min();
  std::vector<E>edge;
  edge.reserve(n/2);
  for(int i=0;i<n-1;i++)if(points[i].first==points[i+1].first){
    if(points[i].second<points[i+1].second)edge.push_back({points[i].first,points[i].second,points[i+1].second});
    else edge.push_back({points[i].first,points[i+1].second,points[i].second});
  }
  if(points[0].first==points[n-1].first){
    if(points[0].second<points[n-1].second)edge.push_back({points[0].first,points[0].second,points[n-1].second});
    else edge.push_back({points[0].first,points[n-1].second,points[0].second});
  }
  std::sort(edge.begin(),edge.end());
  std::map<T,T>mp;
  mp[minf]=minf;
  mp[inf]=minf;
  std::vector<std::pair<std::pair<T,T>,std::pair<T,T>>>res;
  for(const auto&[x,l,r]:edge){
    mp[l]=std::prev(mp.upper_bound(l))->second;
    mp[r]=std::prev(mp.upper_bound(r))->second;
    if(mp[l]==minf){
      for(auto itr=mp.find(l);itr->first<r;itr=mp.erase(itr));
      mp[l]=x;
    }
    else{
      for(auto itr=mp.find(l);itr->first<r;itr=mp.erase(itr)){
        res.push_back(std::make_pair(std::make_pair(itr->second,itr->first),std::make_pair(x,std::next(itr)->first)));
      }
      mp[l]=minf;
    }
  }
  return res;
}