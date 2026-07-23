#pragma once
#include<limits>
#include "ntt.hpp"
template<typename T>
struct SemiRelaxedConvolution{
private:
  using mul_type=typename T::mul_type;
  std::vector<T>f,g,h;
  std::vector<std::vector<T>>f_memo,g_memo;
  int p,n,loglog2n,na;
  std::vector<mul_type>temp;
  static constexpr int b=4;
  static constexpr int d=6;
  static_assert(b<=d);
  static_assert((mul_type)(T::mod()-1)*(T::mod()-1)<=std::numeric_limits<mul_type>::max()/(1<<b));
public:
  SemiRelaxedConvolution(){}
  explicit SemiRelaxedConvolution(const std::vector<T>&f_):f(f_),p(0){
    n=ceil_pow2(f.size()+1)-1;
    f.resize(n),g.resize(n),h.resize(n);
    temp.resize(n);
    loglog2n=[&]()->int {
      int n2=n+1;
      int res=0;
      while(n2>(1<<d)){
        res++;
        n2>>=b;
      }
      return res;
    }();
    f_memo.resize(loglog2n),g_memo.resize(loglog2n);
    na=msb(n+1)-loglog2n*b;
    for(int i=0,sq=(n+1)>>(loglog2n*b);i<loglog2n;i++,sq<<=b){
      std::vector<T>&c=f_memo[i];
      c.resize((n/sq)*sq*2);
      g_memo[i].resize(c.size());
      std::vector<T>buf(sq*2);
      for(int j=0;j+sq*2-1<=n;j+=sq){
        std::copy(f.begin()+j,f.begin()+j+sq*2-1,buf.begin());
        buf[sq*2-1]=T();
        dft(buf);
        std::copy(buf.begin(),buf.end(),c.begin()+j*2);
      }
    }
  }
  T query(T v){
    g[p]=v;
    int p2=p+1;
    int w=lsb(p2);
    if(w<na){
      int lc=p2&((1<<na)-1);
      for(int i=0;i<lc;i++)h[p]+=f[i]*g[p-i];
    }
    else{
      w=(w-na)/b;
      int sq=(n+1)>>((loglog2n-w)*b);
      int lc=(p2>>(na+w*b))&((1<<b)-1);
      std::vector<T>buf(sq*2);
      std::copy(g.begin()+p2-sq,g.begin()+p2,buf.begin());
      dft(buf);
      std::copy(buf.begin(),buf.end(),g_memo[w].begin()+(p2-sq)*2);
      for(int i=0;i<sq*2;i++)temp[i]=(mul_type)buf[i].val()*f_memo[w][i].val();
      for(int i=1;i<lc;i++){
        for(int j=0;j<sq*2;j++){
          temp[j]+=(mul_type)f_memo[w][i*sq*2+j].val()*g_memo[w][(p2-sq*(i+1))*2+j].val();
        }
      }
      for(int i=0;i<sq*2;i++)buf[i]=temp[i];
      idft(buf);
      T inv=T::raw(sq*2).inv();
      for(int i=sq-1;i<sq*2-1;i++)h[p+i+1-sq]+=buf[i]*inv;
    }
    return h[p++];
  }
};