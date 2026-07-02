#pragma once
#include<vector>
template<typename T>
std::vector<T>reeds_sloane(const std::vector<T>a,std::pair<int,int>pe){
  int n=a.size();
  std::vector<std::vector<T>>Q(pe.second),B(pe.second);
  std::vector<int>nb(pe.second,-1);
  std::vector<T>tb(pe.second);
  T powp=1;
  for(int j=0;j<pe.second;j++){
    Q[j]={powp};
    powp*=pe.first;
  }
  for(int i=0;i<n;i++){
    std::vector<std::pair<T,int>>tu(pe.second);
    for(int j=0;j<pe.second;j++){
      T x=0;
      for(int k=0;k<(int)Q[j].size();k++){
        assert(i-k>=0);
        x+=Q[j][k]*a[i-k];
      }
      if(x.val()==0)tu[j]=std::make_pair(1,pe.second);
      else{
        int x2=x.val();
        int ne=tu[j].second;
        while(x2%pe.first==0)x2/=pe.first,ne++;
        tu[j]=std::make_pair(x2,ne);
      }
    }
    std::vector<std::vector<T>>nQ(Q);
    for(int j=0;j<pe.second;j++){
      if(tu[j].second==pe.second)continue;
      int k=pe.second-1-tu[j].second;
      if(nb[k]==-1)nQ[j].resize(i+2);
      else{
        T c=tu[j].first/tb[k];
        if(i+B[k].size()-nb[k]>nQ[j].size())nQ[j].resize(i+B[k].size()-nb[k]);
        for(int l=0;l<(int)B[k].size();l++)nQ[j][i+l-nb[k]]-=c*B[k][l];
      }
    }
    for(int j=0;j<pe.second;j++){
      if(Q[j].size()<nQ[j].size()){
        int k=pe.second-1-tu[j].second;
        B[j]=Q[k];
        nb[j]=i;
        tb[j]=tu[k].first;
      }
    }
    Q=std::move(nQ);
  }
  return Q[0];
}