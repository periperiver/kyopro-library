#pragma once
#include "tree_base.hpp"
#include<map>
#include<algorithm>
template<typename T>
std::vector<int>tree_isomorphism_classification(const Tree<T>&t){
  assert(t.is_directed());
  std::vector<int>res(t.size());
  std::vector<int>bfs=t.bfs_order();
  std::reverse(bfs.begin(),bfs.end());
  std::map<std::vector<int>,int>mp;
  for(int x:bfs){
    std::vector<int>now;
    now.reserve(t[x].size());
    for(const Edge<T>&e:t[x])now.push_back(res[e.to]);
    std::sort(now.begin(),now.end());
    if(mp.contains(now))res[x]=mp[now];
    else{
      res[x]=mp.size();
      mp[now]=res[x];
    }
  }
  return res;
}