#pragma once
#include "datastructure/static_toptree.hpp"
template<typename DP>
struct DynamicTreeDP{
private:
  using Data=typename DP::Data;
  std::vector<Data>data;
  StaticTopTree stt;
  inline void update(int x){
    int l=stt.left[x],r=stt.right[x];
    if(stt.A[l]==stt.A[r])data[x]=DP::rake(data[l],data[r]);
    else data[x]=DP::compress(data[l],data[r]);
  }
public:
  DynamicTreeDP(){}
  template<typename T>
  explicit DynamicTreeDP(Tree<T>t,std::vector<Data>init){
    assert(t.size()==(int)init.size());
    int n=t.size();
    stt=StaticTopTree(std::move(t));
    data=std::move(init);
    data.resize(n*2-1);
    for(int i=n;i<n*2-1;i++)update(i);
  }
  void set(int x,Data v){
    data[x]=std::move(v);
    for(x=stt.par[x];x!=-1;x=stt.par[x])update(x);
  }
  inline Data get()const{return data.back();}
};