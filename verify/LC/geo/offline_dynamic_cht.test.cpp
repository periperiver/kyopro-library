#define PROBLEM "https://judge.yosupo.jp/problem/line_add_get_min"
#include "fastio.hpp"
#include "geo/offline_dynamic_cht.hpp"
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<RangeLine<long long>>lines(n);
  for(int i=0;i<n;i++){
    long long a,b;
    rd(a),rd(b);
    lines[i]={0,q,a,b};
  }
  std::vector<bool>ask(q);
  std::vector<long long>query(q);
  for(int i=0;i<q;i++){
    int t;
    rd(t);
    if(t==0){
      long long a,b;
      rd(a),rd(b);
      lines.push_back({i,q,a,b});
    }
    else{
      rd(query[i]);
      ask[i]=true;
    }
  }
  auto ans=offline_dynamic_cht<long long,true,__int128_t>(lines,query);
  for(int i=0;i<q;i++)if(ask[i])wt(ans[i]),wt('\n');
}