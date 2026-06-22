#pragma once
#include "template.hpp"
template<typename I,typename S>
struct SquareRootDecomposition2d{
private:
  struct Inner{
    vector<S>da,db,dc;
    vector<I>z;
    unordered_map<I,int>zinv;
    int n,b;
    Inner(){}
    Inner(const vector<pair<I,S>>&dat){
      z.resize(dat.size());
      rep(i,dat.size())z[i]=dat[i].first;
      sort(all(z)),z.erase(unique(all(z)),z.end());
      zinv.reserve(z.size());
      rep(i,z.size())zinv[z[i]]=i;
      n=z.size(),b=1;
      while(b*b*b<n)b++;
      dc.resize(n,0);
      db.resize((dc.size()+b-1)/b,0);
      da.resize((db.size()+b-1)/b,0);
      for(auto&&[i,x]:dat)add(i,x);
    }
    inline void add(I i,S x){
      int j=zinv[i];
      dc[j]+=x;
      j/=b;
      db[j]+=x;
      j/=b;
      da[j]+=x;
    }
    S sum(I l,I r)const{
      int il=std::lower_bound(all(z),l)-z.begin();
      int ir=std::lower_bound(all(z),r)-z.begin();
      int iil=(il+b-1)/b*b,iir=ir/b*b;
      S ret=0;
      if(iil>iir){
        reps(i,il,ir)ret+=dc[i];
        return ret;
      }
      reps(i,il,iil)ret+=dc[i];
      reps(i,iir,ir)ret+=dc[i];
      il=iil/b,ir=iir/b;
      iil=(il+b)/b*b,iir=ir/b*b;
      if(iil>iir){
        reps(i,il,ir)ret+=db[i];
        return ret;
      }
      reps(i,il,iil)ret+=db[i];
      reps(i,iir,ir)ret+=db[i];
      il=iil/b,ir=iir/b;
      reps(i,il,ir)ret+=da[i];
      return ret;
    }
  };
  vector<Inner>da,db,dc;
  vector<I>z;
  unordered_map<I,int>zinv;
  int n,b;
  static constexpr I inf=numeric_limits<I>::max();
public:
  SquareRootDecomposition2d(const vector<tuple<I,I,S>>&dat){
    z.resize(dat.size());
    rep(i,dat.size())z[i]=std::get<0>(dat[i]);
    sort(all(z)),z.erase(unique(all(z)),z.end());
    zinv.reserve(z.size());
    rep(i,z.size())zinv[z[i]]=i;
    n=z.size(),b=1;
    while(b*b*b<n)b++;
    dc.resize(n);
    db.resize((dc.size()+b-1)/b);
    da.resize((db.size()+b-1)/b);
    vector<vector<pair<I,S>>>inita(da.size()),initb(db.size()),initc(dc.size());
    for(const auto&[x,y,v]:dat){
      int ix=zinv[x];
      initc[ix].emplace_back(y,v);
      ix/=b;
      initb[ix].emplace_back(y,v);
      ix/=b;
      inita[ix].emplace_back(y,v);
    }
    rep(i,inita.size())da[i]=Inner(inita[i]);
    rep(i,initb.size())db[i]=Inner(initb[i]);
    rep(i,initc.size())dc[i]=Inner(initc[i]);
  }
  void add(I x,I y,S v){
    int ix=zinv[x];
    dc[ix].add(y,v);
    ix/=b;
    db[ix].add(y,v);
    ix/=b;
    da[ix].add(y,v);
  }
  S sum(I lx,I rx,I ly,I ry)const{
    int ilx=std::lower_bound(all(z),lx)-z.begin();
    int irx=std::lower_bound(all(z),rx)-z.begin();
    int iilx=(ilx+b-1)/b*b,iirx=irx/b*b;
    S ret=0;
    if(iilx>iirx){
      reps(i,ilx,irx)ret+=dc[i].sum(ly,ry);
      return ret;
    }
    reps(i,ilx,iilx)ret+=dc[i].sum(ly,ry);
    reps(i,iirx,irx)ret+=dc[i].sum(ly,ry);
    ilx=iilx/b,irx=iirx/b;
    iilx=(ilx+b-1)/b*b,iirx=irx/b*b;
    if(iilx>iirx){
      reps(i,ilx,irx)ret+=db[i].sum(ly,ry);
      return ret;
    }
    reps(i,ilx,iilx)ret+=db[i].sum(ly,ry);
    reps(i,iirx,irx)ret+=db[i].sum(ly,ry);
    ilx=iilx/b,irx=iirx/b;
    reps(i,ilx,irx)ret+=da[i].sum(ly,ry);
    return ret;
  }
  I lower_bound(I lx,I ly,I ry,S k)const{
    assert(k>0);
    int ilx=std::lower_bound(all(z),lx)-z.begin();
    int iilx=(ilx+b-1)/b*b;
    reps(i,ilx,iilx){
      if((k-=dc[i].sum(ly,ry))<=0)return z[i];
    }
    ilx=iilx/b;
    iilx=(ilx+b-1)/b*b;
    reps(i,ilx,iilx){
      S now=db[i].sum(ly,ry);
      if(k-now<=0){
        ilx=i*b;
        iilx=ilx+b;
        reps(j,ilx,iilx){
          if((k-=dc[j].sum(ly,ry))<=0)return z[j];
        }
      }
      else k-=now;
    }
    ilx=iilx/b;
    while(ilx<da.size()){
      S now=da[ilx].sum(ly,ry);
      if(k-now<=0){
        ilx*=b;
        iilx=ilx+b;
        reps(i,ilx,iilx){
          S now2=db[i].sum(ly,ry);
          if(k-now2<=0){
            ilx=i*b;
            while(true){
              if((k-=dc[ilx].sum(ly,ry))<=0)return z[ilx];
              else ilx++;
            }
          }
          else k-=now2;
        }
      }
      else k-=now,ilx++;
    }
    return inf;
  }
};