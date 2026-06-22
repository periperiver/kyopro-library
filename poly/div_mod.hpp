#pragma once
#include "fps_inv.hpp"
#include<algorithm>
template<typename T>
std::pair<std::vector<T>,std::vector<T>>poly_div_mod(std::vector<T>f,std::vector<T>g){
  if(f.size()<g.size())return std::make_pair(std::vector<T>{},f);
  std::reverse(f.begin(),f.end()),std::reverse(g.begin(),g.end());
  std::vector<T>q=ntt_convolution(f,fps_inv(g,f.size()));
  q.resize(std::max(0,(int)f.size()-(int)g.size()+1));
  std::vector<T>p=ntt_convolution(q,g);
  std::reverse(p.begin(),p.end());
  std::reverse(f.begin(),f.end());
  for(int i=0;i<(int)f.size();i++)f[i]-=p[i];
  std::reverse(q.begin(),q.end());
  f.resize(g.size()-1);
  return std::make_pair(q,f);
}