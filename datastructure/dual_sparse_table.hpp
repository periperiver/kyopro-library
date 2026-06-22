#pragma once
#include<vector>
#include "math/util.hpp"
template<typename M,int L=4>
std::vector<typename M::S>dual_sparse_table(int n,const std::vector<std::tuple<int,int,typename M::F>>&query){
  using S=typename M::S;
  using F=typename M::F;
  int n2=(n+(1<<L)-1)&~((1<<L)-1);
  std::vector<S>res(n,M::e());
  std::vector<F>prefix(n2,M::id()),suffix(n2,M::id());
  int large_size=n2>>L;
  int d=large_size==1?1:32-__builtin_clz(large_size-1);
  std::vector<F>sp(d*large_size,M::id());
  for(const auto&[l,r2,f]:query)if(l<r2){
    int r=r2-1;
    int lid=l>>L,rid=r>>L;
    if(lid==rid){
      for(int i=l;i<=r;i++)res[i]=M::mapping(f,res[i],1);
    }
    else{
      lid++,rid--;
      if(lid==rid)sp[lid]=M::composition(f,sp[lid]);
      else if(lid<rid){
        int s=msb(lid^rid)*large_size;
        sp[s+lid]=M::composition(f,sp[s+lid]);
        sp[s+rid]=M::composition(f,sp[s+rid]);
      }
      suffix[l]=M::composition(f,suffix[l]);
      prefix[r]=M::composition(f,prefix[r]);
    }
  }
  for(int i=1;i<d;i++){
    int w=1<<i;
    int base=i*large_size;
    for(int j=w;j<=large_size;j+=w*2){
      for(int k=j-w;k<j-1;k++)sp[base+k+1]=M::composition(sp[base+k],sp[base+k+1]);
      int r=std::min<int>(large_size,j+w);
      for(int k=r-1;k>=j+1;k--)sp[base+k-1]=M::composition(sp[base+k],sp[base+k-1]);
    }
    for(int j=0;j<large_size;j++)sp[j]=M::composition(sp[j],sp[base+j]);
  }
  for(int i=0;i<large_size;i++){
    for(int j=(1<<L)-1;j>=1;j--)prefix[(i<<L)+j-1]=M::composition(prefix[(i<<L)+j-1],prefix[(i<<L)+j]);
    for(int j=1;j<(1<<L);j++)suffix[(i<<L)+j]=M::composition(suffix[(i<<L)+j-1],suffix[(i<<L)+j]);
  }
  for(int i=0;i<n;i++){
    F lazy=M::composition(prefix[i],M::composition(suffix[i],sp[i>>L]));
    res[i]=M::mapping(lazy,res[i],1);
  }
  return res;
}