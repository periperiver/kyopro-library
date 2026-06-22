#define PROBLEM "https://judge.yosupo.jp/problem/associative_array"
#include "fastio.hpp"
#include "datastructure/hash_map.hpp"
int main(){
  int q;
  rd(q);
  HashMap<long long,long long>mp(q);
  while(q--){
    int t;
    rd(t);
    if(t==0){
      long long k,v;
      rd(k),rd(v);
      mp[k]=v;
    }
    else{
      long long k;
      rd(k);
      wt(mp.get(k)),wt('\n');
    }
  }
}