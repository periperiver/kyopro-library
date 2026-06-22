#pragma once
#include "../math/quotients.hpp"
#include "../math/kth_root.hpp"
#include<cassert>
#include<algorithm>
template<typename T>
std::vector<T>dirichlet_convolution(const std::vector<T>&f,const std::vector<T>&g,const Quotients&quo){
  assert(f.size()==quo.size());
  assert(g.size()==quo.size());
  std::vector<T>fsmall(f.begin(),f.begin()+quo.s);
  std::vector<T>gsmall(g.begin(),g.begin()+quo.s);
  for(int i=quo.s-1;i>=1;i--)fsmall[i]-=fsmall[i-1],gsmall[i]-=gsmall[i-1];
  std::vector<T>h(quo.size()+1);
  for(long long x=1;x*x*x<=quo.n;x++){
    for(long long y=x;x*y*y<=quo.n;y++){
      T val=fsmall[x-1]*gsmall[y-1];
      h[quo.get_key(x*y)]+=val;
      h[quo.size()-y+1]-=val;
    }
  }
  for(long long y=1;y*y*y<=quo.n;y++){
    for(long long x=y+1;y*x*x<=quo.n;x++){
      T val=fsmall[x-1]*gsmall[y-1];
      h[quo.get_key(x*y)]+=val;
      h[quo.size()-x+1]-=val;
    }
  }
  for(int i=1;i<quo.size();i++)h[i]+=h[i-1];
  for(long long x=1;x*x*x<=quo.n;x++){
    for(long long z=x;x*z*z<=quo.n;z++){
      h[quo.size()-z]+=fsmall[x-1]*(g[quo.get_key(quo.n/(x*z))]-g[z-1]);
    }
  }
  for(long long z=1;z*z*z<=quo.n;z++){
    for(long long x=z+1;z*x*x<=quo.n;x++){
      h[quo.size()-z]+=fsmall[x-1]*(g[quo.get_key(quo.n/(x*z))]-g[x-2]);
    }
  }
  for(long long y=1;y*y*y<=quo.n;y++){
    for(long long z=y;y*z*z<=quo.n;z++){
      h[quo.size()-z]+=gsmall[y-1]*(f[quo.get_key(quo.n/(y*z))]-f[z-1]);
    }
  }
  for(long long z=1;z*z*z<=quo.n;z++){
    for(long long y=z+1;z*y*y<=quo.n;y++){
      h[quo.size()-z]+=gsmall[y-1]*(f[quo.get_key(quo.n/(y*z))]-f[y-1]);
    }
  }
  h.pop_back();
  return h;
}