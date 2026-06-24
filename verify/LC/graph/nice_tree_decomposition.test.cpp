#define PROBLEM "https://judge.yosupo.jp/problem/tree_decomposition_width_2"
#include "graph/nice_tree_decomposition.hpp"
#include "datastructure/unionfind.hpp"
void check_nice(const std::vector<std::pair<int,int>>&child,const std::vector<std::array<int,3>>&vs){
  int n=child.size();
  for(int i=0;i<n;i++){
    if(child[i].first==-1){
      assert(child[i].second==-1);
      assert(std::count(vs[i].begin(),vs[i].end(),-1)==2);
    }
    else if(child[i].second==-1){
      const std::array<int,3>&c=vs[child[i].first];
      const std::array<int,3>&p=vs[i];
      int cnt=0;
      for(int j=0;j<3;j++){
        cnt+=c[j]!=p[j];
        if(c[j]!=p[j]){
          assert(c[j]==-1||p[j]==-1);
        }
      }
      assert(cnt==1);
    }
    else{
      assert(vs[i]==vs[child[i].first]);
      assert(vs[i]==vs[child[i].second]);
    }
  }
}
int main(){
  std::string gomi;
  std::cin>>gomi>>gomi;
  int n,m;
  std::cin>>n>>m;
  Graph<>g(n,false);
  g.read<0,1>(m);
  UnionFind uf(n);
  for(auto e:g)uf.merge(e.from,e.to);
  for(int i=0;i<n;i++)if(uf.merge(i,0))g.add_edge(i,0);
  g.build();
  auto [child,vs]=nice_tree_decomposition(g);
  if(child.empty())std::cout<<"-1\n";
  else{
    std::cout<<"s td "<<child.size()<<" 2 "<<n<<'\n';
    for(int i=0;i<std::ssize(child);i++){
      std::cout<<"b "<<i+1;
      for(int v:vs[i])if(v!=-1)std::cout<<' '<<v+1;
      std::cout<<'\n';
    }
    for(int i=0;i<std::ssize(child);i++){
      if(child[i].first!=-1)std::cout<<i+1<<' '<<child[i].first+1<<'\n';
      if(child[i].second!=-1)std::cout<<i+1<<' '<<child[i].second+1<<'\n';
    }
    check_nice(child,vs);
  }
}
