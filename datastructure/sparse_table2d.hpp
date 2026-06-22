#pragma once
#include "sparse_table.hpp"
template<typename M,int L1=3,int L2=4>
struct SparseTable2d{
private:
  using S=typename M::S;
  using Sp=SparseTable<M,L2>;
  std::vector<Sp>prefix,suffix;
  std::vector<std::vector<Sp>>sp;
  std::vector<Sp>dat;
public:
  SparseTable2d(std::vector<std::vector<S>>a){
    dat.resize(a.size());
    for(int i=0;i<(int)a.size();i++)dat[i]=Sp(a[i]);
    int h=a.size();
    h=(h+(1<<L1)-1)&~((1<<L1)-1);
    a.resize(h,std::vector<S>(a[0].size(),M::e()));
    auto p(a),s(a);
    for(int i=0;i<h;i++)for(int j=0;j<(int)a[i].size();j++)if(i&((1<<L1)-1)){
      p[i][j]=M::op(p[i][j],p[i-1][j]);
    }
    for(int i=h-1;i>=0;i--)for(int j=0;j<(int)a[i].size();j++)if(i&((1<<L1)-1)){
      s[i-1][j]=M::op(s[i-1][j],s[i][j]);
    }
    prefix.resize(h);
    suffix.resize(h);
    for(int i=0;i<h;i++){
      prefix[i]=Sp(p[i]);
      suffix[i]=Sp(s[i]);
    }
    std::vector<std::vector<S>>d2(h>>L1,std::vector<S>(a[0].size(),M::e()));
    for(int i=0;i<(int)d2.size();i++){
      for(int j=0;j<(1<<L1);j++){
        for(int k=0;k<(int)a[0].size();k++)d2[i][k]=M::op(d2[i][k],a[(i<<L1)+j][k]);
      }
    }
    int d=(d2.size()==1?1:32-__builtin_clz(d2.size()-1));
    sp.resize(d,std::vector<Sp>(d2.size()));
    for(int i=0;i<(int)d2.size();i++)sp[0][i]=Sp(d2[i]);
    for(int i=1;i<d;i++){
      int w=1<<i;
      std::vector<std::vector<S>>now(d2);
      for(int j=w;j<=(int)d2.size();j+=w*2){
        for(int k=j-2;k>=j-w;k--)for(int l=0;l<(int)d2[0].size();l++)now[k][l]=M::op(now[k][l],now[k+1][l]);
        int r=std::min<int>(d2.size(),j+w);
        for(int k=j+1;k<r;k++)for(int l=0;l<(int)d2[0].size();l++)now[k][l]=M::op(now[k-1][l],now[k][l]);
      }
      for(int j=0;j<(int)d2.size();j++)sp[i][j]=Sp(now[j]);
    }
  }
  S prod(int lx,int rx,int ly,int ry)const{
    rx--;
    int lid=lx>>L1,rid=rx>>L1;
    if(lid==rid){
      S ret=M::e();
      for(int i=lx;i<=rx;i++)ret=M::op(ret,dat[i].prod(ly,ry));
      return ret;
    }
    else{
      lid++,rid--;
      S mid=M::e();
      if(lid==rid)mid=sp[0][lid].prod(ly,ry);
      else if(lid<rid){
        int s=31-__builtin_clz(lid^rid);
        mid=M::op(sp[s][lid].prod(ly,ry),sp[s][rid].prod(ly,ry));
      }
      return M::op(suffix[lx].prod(ly,ry),M::op(mid,prefix[rx].prod(ly,ry)));
    }
  }
};