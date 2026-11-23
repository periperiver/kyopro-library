#define PROBLEM "https://judge.yosupo.jp/problem/number_of_substrings"
#include "string/suffix_tree.hpp"
#include "fastio.hpp"
int main(){
  std::string s;
  rd(s);
  for(char&c:s)c-='a';
  SuffixTree st(s);
  long long ans=0;
  for(int i=0;i<(int)st.size();i++)ans+=st[i].range.second-st[i].range.first;
  wt(ans),wt('\n');
}