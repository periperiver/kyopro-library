#pragma once
#include "template.hpp"
#include "math/fraction.hpp"
template<typename T>
struct SternBlocotTree{
  using frac=fraction<T>;
  using pff=pair<frac,frac>;
private:
  frac left={0,1},right={1,0};
  T d=0;
  vector<pair<char,T>>p;
  static constexpr char dir[2]={'L','R'};
  void indown(bool isright,T k){
    if(k==0)return;
    if(!p.empty()&&p.back().first==dir[isright])p.back().second+=k;
    else p.emplace_back(dir[isright],k);
    d+=k;
    if(isright)left.num+=right.num*k,left.den+=right.den*k;
    else right.num+=left.num*k,right.den+=left.den*k;
  }
  void inup(T k){
    assert(0<=k&&k<=d);
    while(k){
      auto [dir,k2]=p.back();
      if(k<k2){
        k2-=k;
        p.back().second-=k;
        if(dir=='L')right.num-=left.num*k,right.den-=left.den*k;
        else left.num-=right.num*k,left.den-=right.den*k;
        break;
      }
      else{
        k-=k2;
        if(dir=='L')right.num-=left.num*k2,right.den-=left.den*k2;
        else left.num-=right.num*k2,left.den-=right.den*k2;
        p.pop_back();
      }
    }
  }
public:
  SternBlocotTree(T a,T b){
    bool isright=a>b;
    if(!isright)swap(a,b);
    while(b>0){
      T q=a/b,r=a-q*b;
      indown(isright,q-!r);
      a=b,b=r;
      isright^=1;
    }
  }
  SternBlocotTree(frac f):SternBlocotTree(f.num,f.second){}
  SternBlocotTree(vector<pair<char,T>>p):p(p){
    rep(i,p.size()){
      if(p[i].first=='L')right.num+=left.num*p[i].second,right.den+=left.den*p[i].second;
      else left.num+=right.num*p[i].second,left.den+=right.den*p[i].second;
    }
  }
  SternBlocotTree(){}
  SternBlocotTree down(bool isright,T k){
    SternBlocotTree ret(*this);
    ret.indown(isright,k);
    return ret;
  }
  SternBlocotTree up(T k){
    SternBlocotTree ret(*this);
    ret.inup(k);
    return ret;
  }
  pff range()const{return make_pair(left,right);}
  frac val()const{return frac::raw(left.num+right.num,left.den+right.den);}
  T dep()const{return d;}
  vector<pair<char,T>>path()const{return p;}
  static SternBlocotTree lca(SternBlocotTree &a,SternBlocotTree b){
    auto pa=a.path(),pb=b.path();
    int sz=min(pa.size(),pb.size());
    SternBlocotTree ret;
    rep(i,sz){
      if(pa[i]==pb[i])ret.indown(pa[i].first=='R',pa[i].second);
      else{
        if(pa[i].first==pb[i].first)ret.indown(pa[i].first=='R',min(pa[i].second,pb[i].second));
        break;
      }
    }
    return ret;
  }
  static pff binary_search(T n,const auto &f){
    static_assert(is_convertible_v<decltype(f),function<bool(frac)>>);
    assert(!f(frac(0)));
    frac l=frac::raw(0,1),r=frac::raw(1,0);
    if(!f(frac(1))){
      T now=2;
      while(!f(frac(now)))now*=2;
      T ok=now,ng=now/2;
      while(ok-ng>1){
        T mid=(ok+ng)/2;
        (f(frac(mid))?ok:ng)=mid;
      }
      l=frac::raw(ng,1);
    }
    for(bool isright=false;;isright^=1){
      T ok=1,ng=(n-r.den)/l.den+1;
      while(ng-ok>1){
        T mid=(ok+ng)/2;
        (f(frac::raw(l.num*mid+r.num,l.den*mid+r.den))!=isright?ok:ng)=mid;
      }
      r.num+=l.num*ok,r.den+=l.den*ok;
      if(r.den>n){
        r.num-=l.num;
        r.den-=l.den;
        if(isright)swap(l,r);
        break;
      }
      swap(l,r);
    }
    return make_pair(l,r);
  }
};