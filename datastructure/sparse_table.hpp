#pragma once
#include<vector>
#include "math/util.hpp"
template<typename M,int L=5>
struct SparseTable{
  using S=typename M::S;
private:
  std::vector<S>dat,prefix,suffix;
  std::vector<std::vector<S>>sp;
public:
  SparseTable(){}
  SparseTable(std::vector<S>a):dat(a){
    int n=a.size();
    n=(n+(1<<L)-1)&~((1<<L)-1);
    a.resize(n,M::e());
    prefix=suffix=a;
    std::vector<S>d2(n>>L,M::e());
    for(int i=0;i<(int)d2.size();i++){
      for(int j=0;j<(1<<L);j++)d2[i]=M::op(d2[i],a[(i<<L)+j]);
    }
    for(int i=0;i<(n>>L);i++){
      for(int j=1;j<(1<<L);j++)prefix[(i<<L)+j]=M::op(prefix[(i<<L)+j-1],prefix[(i<<L)+j]);
    }
    for(int i=(n>>L)-1;i>=0;i--){
      for(int j=(1<<L)-1;j>=1;j--)suffix[(i<<L)+j-1]=M::op(suffix[(i<<L)+j-1],suffix[(i<<L)+j]);
    }
    int d=(d2.size()==1?1:32-__builtin_clz(d2.size()-1));
    sp.resize(d,d2);
    for(int i=1;i<d;i++){
      int w=1<<i;
      for(int j=w;j<=(int)d2.size();j+=w*2){
        for(int k=j-2;k>=j-w;k--)sp[i][k]=M::op(d2[k],sp[i][k+1]);
        int r=std::min<int>(d2.size(),j+w);
        for(int k=j+1;k<r;k++)sp[i][k]=M::op(sp[i][k-1],d2[k]);
      }
    }
  }
  S prod(int l,int r)const{
    if(l==r)return M::e();
    r--;
    int lid=l>>L,rid=r>>L;
    if(lid==rid){
      S ret=M::e();
      for(int i=l;i<=r;i++)ret=M::op(ret,dat[i]);
      return ret;
    }
    else{
      lid++;
      rid--;
      S mid=M::e();
      if(lid==rid)mid=sp[0][lid];
      else if(lid<rid){
        int s=msb(lid^rid);
        mid=M::op(sp[s][lid],sp[s][rid]);
      }
      return M::op(suffix[l],M::op(mid,prefix[r]));
    }
  }
};