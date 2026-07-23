#pragma once
#include "ntt.hpp"
template<typename T>
struct RelaxedConvolution{
private:
using mul_type=typename T::mul_type;
  int p;
  std::vector<T>f,g,h;
  std::vector<std::vector<T>>f_memo,g_memo;
  static constexpr int b=5;
public:
  RelaxedConvolution():p(0){}
  T query(T fv,T gv){
    f.emplace_back(fv),g.emplace_back(gv);
    int w=lsb(p+1);
    if(w<b){
      if(p<(1<<b)){
        h.emplace_back();
        for(int i=0;i<=p;i++)h[p]+=f[i]*g[p-i];
      }
      else{
        int sq=(p&((1<<b)-1))+1;
        for(int i=0;i<sq;i++)h[p]+=f[i]*g[p-i]+f[p-i]*g[i];
      }
    }
    else if(p+1==(1<<w)){
      std::vector<T>f2(f),g2(g);
      f2.resize(2<<w),g2.resize(2<<w);
      dft(f2),dft(g2);
      if(w!=b)f_memo.emplace_back(f2.begin(),f2.begin()+(1<<w)),g_memo.emplace_back(g2.begin(),g2.begin()+(1<<w));
      for(int i=0;i<(2<<w);i++)f2[i]*=g2[i];
      idft(f2);
      T inv=T::raw(2<<w).inv();
      h.resize(p*2+1);
      for(int i=p;i<=p*2;i++)h[i]+=f2[i]*inv;
    }
    else{
      std::vector<T>f2(f.end()-(1<<w),f.end()),g2(g.end()-(1<<w),g.end());
      f2.resize(2<<w),g2.resize(2<<w);
      dft(f2),dft(g2);
      for(int i=0;i<(2<<w);i++)f2[i]=(mul_type)f2[i].val()*g_memo[w-b][i].val()+(mul_type)g2[i].val()*f_memo[w-b][i].val();
      idft(f2);
      T inv=T::raw(2<<w).inv();
      for(int i=0;i<(1<<w);i++)h[p+i]+=f2[i+(1<<w)-1]*inv;
    }
    return h[p++];
  }
};