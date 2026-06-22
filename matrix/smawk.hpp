#pragma once
#include<vector>
#include<numeric>
template<typename Func>
std::vector<int>smawk(int n,int m,const Func&f){
  auto rec=[&](auto self,int lg,std::vector<int>col)->std::vector<int> {
    if(1<<lg>n)return {};
    std::vector<int>col2_idx;
    for(int i=0;i<(int)col.size();i++){
      while(!col2_idx.empty()&&f(((col2_idx.size()+1)<<lg)-1,col[col2_idx.back()],col[i]))col2_idx.pop_back();
      if((int)col2_idx.size()<n>>lg)col2_idx.push_back(i);
    }
    std::vector<int>col2(col2_idx.size());
    for(int i=0;i<(int)col2.size();i++)col2[i]=col[col2_idx[i]];
    std::vector<int>amin=self(self,lg+1,std::move(col2));
    std::vector<int>res(n>>lg);
    for(int i=0;i<n>>lg;i+=2){
      int l=i==0?0:col2_idx[amin[i/2-1]];
      int r=i+1==(n>>lg)?(int)col.size()-1:col2_idx[amin[i/2]];
      res[i]=l;
      for(int j=l+1;j<=r;j++)if(f(((i+1)<<lg)-1,col[res[i]],col[j]))res[i]=j;
      if(i+1!=(n>>lg))res[i+1]=col2_idx[amin[i/2]];
    }
    return res;
  };
  std::vector<int>col(m);
  std::iota(col.begin(),col.end(),0);
  return rec(rec,0,std::move(col));
}