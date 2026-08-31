#pragma once
#include<vector>
#include "math/util.hpp"
template<typename M,int L=4>
std::vector<typename M::S>dual_sparse_table(int n,const std::vector<std::tuple<int,int,typename M::S>>&query){
  using S=typename M::S;
  int n2=(n+(1<<L)-1)&~((1<<L)-1);
  std::vector<S>res(n,M::e());
  std::vector<S>prefix(n2,M::e()),suffix(n2,M::e());
  int large_size=n2>>L;
  int d=large_size==1?1:32-__builtin_clz(large_size-1);
  std::vector<S>sp(d*large_size,M::e());
  for(const auto&[l,r2,f]:query)if(l<r2){
    int r=r2-1;
    int lid=l>>L,rid=r>>L;
    if(lid==rid){
      for(int i=l;i<=r;i++)res[i]=M::op(f,res[i]);
    }
    else{
      lid++,rid--;
      if(lid==rid)sp[lid]=M::op(f,sp[lid]);
      else if(lid<rid){
        int s=msb(lid^rid)*large_size;
        sp[s+lid]=M::op(f,sp[s+lid]);
        sp[s+rid]=M::op(f,sp[s+rid]);
      }
      suffix[l]=M::op(f,suffix[l]);
      prefix[r]=M::op(f,prefix[r]);
    }
  }
  for(int i=1;i<d;i++){
    int w=1<<i;
    int base=i*large_size;
    for(int j=w;j<=large_size;j+=w*2){
      for(int k=j-w;k<j-1;k++)sp[base+k+1]=M::op(sp[base+k],sp[base+k+1]);
      int r=std::min<int>(large_size,j+w);
      for(int k=r-1;k>=j+1;k--)sp[base+k-1]=M::op(sp[base+k],sp[base+k-1]);
    }
    for(int j=0;j<large_size;j++)sp[j]=M::op(sp[j],sp[base+j]);
  }
  for(int i=0;i<large_size;i++){
    for(int j=(1<<L)-1;j>=1;j--)prefix[(i<<L)+j-1]=M::op(prefix[(i<<L)+j-1],prefix[(i<<L)+j]);
    for(int j=1;j<(1<<L);j++)suffix[(i<<L)+j]=M::op(suffix[(i<<L)+j-1],suffix[(i<<L)+j]);
  }
  for(int i=0;i<n;i++){
    S lazy=M::op(prefix[i],M::op(suffix[i],sp[i>>L]));
    res[i]=M::op(lazy,res[i]);
  }
  return res;
}