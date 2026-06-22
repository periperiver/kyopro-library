#pragma once
#include "fps.hpp"
#include "power_projection.hpp"
template<typename T>
std::vector<T>compositional_inverse(std::vector<T>f){
  assert(f.empty()||f[0].val()==0);
  if(f.size()<=1)return f;
  int n=f.size()-1;
  assert(f[1].val()!=0);
  T inv=f[1].inv();
  for(int i=1;i<=n;i++)f[i]*=inv;
  std::vector<T>g(n+1);
  g[0]=1;
  g=power_projection(f,g,n+1);
  std::vector<T>h(n);
  for(int i=1;i<=n;i++)h[n-i]=g[i]*n/i;
  h=fps_log(h);
  T c=T(-n).inv();
  for(int i=0;i<n;i++)h[i]*=c;
  h=fps_exp(h);
  h.insert(h.begin(),0);
  T pr=1;
  for(int i=0;i<=n;i++)h[i]*=pr,pr*=inv;
  return h;
}