#pragma once
#include<vector>
#include<cassert>
template<typename T>
std::pair<std::vector<T>,std::vector<std::vector<T>>>matrix_linear_equation(std::vector<std::vector<T>>mat,std::vector<T>c){
  int n=mat.size(),m=mat[0].size();
  assert(n==c.size());
  int y=0;
  std::vector<std::pair<int,int>>detv;
  std::vector<int>rankv;
  for(int d=0;d<m;d++){
    int id=-1;
    for(int i=y;i<n;i++)if(mat[i][d].val()){
      id=i;
      break;
    }
    if(id==-1){
      rankv.push_back(d);
      continue;
    }
    for(int j=d;j<n;j++)std::swap(mat[y][j],mat[id][j]);
    std::swap(c[y],c[id]);
    T inv=mat[y][d].inv();
    for(int j=d;j<m;j++)mat[y][j]*=inv;
    c[y]*=inv;
    for(int i=0;i<n;i++)if(i!=y){
      c[i]-=mat[i][d]*c[y];
      for(int j=m-1;j>=d;j--)mat[i][j]-=mat[i][d]*mat[y][j];
    }
    detv.push_back({d,y++});
  }
  for(int i=y;i<n;i++)if(c[i].val())return std::make_pair(std::vector<T>{},std::vector<std::vector<T>>{});
  std::vector<T>ret1(m);
  std::vector<std::vector<T>>ret2(rankv.size(),std::vector<T>(m));
  for(auto [x,i]:detv)ret1[x]=c[i];
  for(int d=0;d<rankv.size();d++){
    ret2[d][rankv[d]]=-1;
    for(auto [x,i]:detv)ret2[d][x]=mat[i][rankv[d]];
  }
  return std::make_pair(ret1,ret2);
}