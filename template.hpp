#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll=long long;
using ull=unsigned long long;
using P=pair<ll,ll>;
template<typename T>using minque=priority_queue<T,vector<T>,greater<T>>;
template<typename T>bool chmax(T &a,const T &b){return (a<b?(a=b,true):false);}
template<typename T>bool chmin(T &a,const T &b){return (a>b?(a=b,true):false);}
template<typename T1,typename T2>istream &operator>>(istream &is,pair<T1,T2>&p){is>>p.first>>p.second;return is;}
template<typename T1,typename T2,typename T3>istream &operator>>(istream &is,tuple<T1,T2,T3>&a){is>>std::get<0>(a)>>std::get<1>(a)>>std::get<2>(a);return is;}
template<typename T,size_t n>istream &operator>>(istream &is,array<T,n>&a){for(auto&i:a)is>>i;return is;}
template<typename T>istream &operator>>(istream &is,vector<T> &a){for(auto &i:a)is>>i;return is;}
template<typename T1,typename T2>void operator++(pair<T1,T2>&a,int n){a.first++,a.second++;}
template<typename T1,typename T2>void operator--(pair<T1,T2>&a,int n){a.first--,a.second--;}
template<typename T>void operator++(vector<T>&a,int n){for(auto &i:a)i++;}
template<typename T>void operator--(vector<T>&a,int n){for(auto &i:a)i--;}
#define overload3(_1,_2,_3,name,...) name
#define rep1(i,n) for(int i=0;i<(int)(n);i++)
#define rep2(i,l,r) for(int i=(int)(l);i<(int)(r);i++)
#define rep(...) overload3(__VA_ARGS__,rep2,rep1)(__VA_ARGS__)
#define reps(i,l,r) rep2(i,l,r)
#define all(x) x.begin(),x.end()
#define pcnt(x) __builtin_popcountll(x)
#define fin(x) return cout<<(x)<<'\n',static_cast<void>(0)
#define yn(x) cout<<((x)?"Yes\n":"No\n")
#define uniq(x) sort(all(x)),x.erase(unique(all(x)),x.end())
template<typename T>
inline int fkey(vector<T>&z,T key){return lower_bound(z.begin(),z.end(),key)-z.begin();}
ll myceil(ll a,ll b){return (a+b-1)/b;}
template<typename T,size_t n,size_t id=0>
auto vec(const int (&d)[n],const T &init=T()){
  if constexpr (id<n)return vector(d[id],vec<T,n,id+1>(d,init));
  else return init;
}
#ifdef LOCAL
#include<debug.h>
#define SWITCH(a,b) (a)
#else
#define debug(...) static_cast<void>(0)
#define debugg(...) static_cast<void>(0)
#define SWITCH(a,b) (b)
template<typename T1,typename T2>ostream &operator<<(ostream &os,const pair<T1,T2>&p){os<<p.first<<' '<<p.second;return os;}
#endif
struct Timer{
  clock_t start;
  Timer(){
    start=clock();
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout<<fixed<<setprecision(16);
  }
  inline double now(){return (double)(clock()-start)/1000;}
  #ifdef LOCAL
  ~Timer(){
    cerr<<"time:";
    cerr<<now();
    cerr<<"ms\n";
  }
  #endif
}timer;