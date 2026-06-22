#pragma once
#include "tree_base.hpp"
#include "math/factorial.hpp"
#include "other/type.hpp"
#include<numeric>
template<typename T,typename T2>
T count_topological_sort_on_tree(const Tree<T2>&t){
  static_assert(is_modint_v<T>);
  assert(t.is_directed());
  auto dfs=[&](auto self,int x)->std::vector<T> {
    std::vector<T>dp{1};
    for(const Edge<T2>&e:t[x]){
      std::vector<T>ch=self(self,e.to);
      if(e.weight==0){
        for(int i=(int)ch.size()-2;i>=0;i--)ch[i]+=ch[i+1];
        ch.push_back(T());
      }
      else if(e.weight==1){
        ch.insert(ch.begin(),T());
        for(int i=1;i<(int)ch.size();i++)ch[i]+=ch[i-1];
      }
      else std::abort();
      std::vector<T>ndp(dp.size()+ch.size()-1);
      for(int i=0;i<(int)dp.size();i++)for(int j=0;j<(int)ch.size();j++){
        ndp[i+j]+=dp[i]*ch[j]*F<T>::C(i+j,i)*F<T>::C(dp.size()-1-i+ch.size()-1-j,dp.size()-1-i);
      }
      dp=std::move(ndp);
    }
    return dp;
  };
  std::vector<T>ans=dfs(dfs,t.root());
  return std::reduce(ans.begin(),ans.end());
}