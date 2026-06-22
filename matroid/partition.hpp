#pragma once
#include<vector>
struct PartitionMatroid{
private:
  int n;
  std::vector<std::vector<int>>s;
  std::vector<int>belong;
  std::vector<int>c,c2;
  std::vector<std::vector<int>>circuits;
public:
  PartitionMatroid(int n_,const std::vector<std::vector<int>>&s_,const std::vector<int>&c_):n(n_),s(s_),belong(n,-1),c2(c_){
    for(int i=0;i<(int)s.size();i++){
      for(int j:s[i])belong[j]=i;
    }
    for(int i=0;i<n;i++)if(belong[i]==-1){
      belong[i]=s.size();
      s.push_back({i});
      c2.push_back(1);
    }
    circuits.resize(s.size());
  }
  int size()const{return n;}
  void set(const std::vector<bool>&I){
    c=c2;
    for(int i=0;i<n;i++)if(I[i])c[belong[i]]--;
    for(int i=0;i<(int)circuits.size();i++)circuits[i].clear();
    for(int i=0;i<n;i++)if(I[i]&&c[belong[i]]==0)circuits[belong[i]].push_back(i);
  }
  std::vector<int>circuit(int x)const{
    if(c[belong[x]]==0){
      std::vector<int>res=circuits[belong[x]];
      res.push_back(x);
      return res;
    }
    return {};
  }
};