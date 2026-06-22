#pragma once
#include "wavelet_matrix.hpp"
#include<algorithm>
struct StaticRangeSet{
private:
  WaveletMatrix<> wm;
public:
  StaticRangeSet(std::vector<int>init){
    std::vector<int>z(init);
    std::sort(z.begin(),z.end()),z.erase(std::unique(z.begin(),z.end()),z.end());
    std::vector<int>pre(z.size(),-1);
    for(int i=0;i<(int)init.size();i++){
      int v=std::lower_bound(z.begin(),z.end(),init[i])-z.begin();
      init[i]=pre[v]+1;
      pre[v]=i;
    }
    wm=WaveletMatrix<>(init);
  }
  int set(int l,int r)const{
    return l==r?0:wm.count(l,r,l+1);
  }
};