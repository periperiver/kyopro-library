#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include "graph/erased_shortest_path.hpp"
#include "graph/dijkstra.hpp"
#include "random/generator.hpp"
std::vector<long long>naive(Graph<long long>g,int s,int t){
  std::vector<long long>res(g.edge_size());
  for(int i=0;i<(int)res.size();i++){
    Graph<long long>g2(g.size(),false);
    for(int j=0;j<(int)res.size();j++)if(i!=j)g2.add_edge(g.get_edge(j).from,g.get_edge(j).to,g.get_edge(j).weight);
    g2.build();
    res[i]=dijkstra(g2,s).first[t];
  }
  return res;
}
void test(Graph<long long>g,int s,int t){
  assert(erased_shortest_pah(g,s,t)==naive(g,s,t));
}
int main(){
  for(int n=1;n<=20;n++){
    for(int m=0;m<=50;m++){
      for(int i=0;i<10;i++){
        int s=Random::range(n);
        int t=Random::range(n);
        Graph<long long>g(n,false);
        for(int j=0;j<m;j++){
          g.add_edge(Random::range(n),Random::range(n),Random::range(5));
        }
        g.build();
        test(g,s,t);
      }
      for(int i=0;i<10;i++){
        int s=Random::range(n);
        int t=Random::range(n);
        Graph<long long>g(n,false);
        for(int j=0;j<m;j++){
          g.add_edge(Random::range(n),Random::range(n),Random::range(1000000000));
        }
        g.build();
        test(g,s,t);
      }
    }
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}