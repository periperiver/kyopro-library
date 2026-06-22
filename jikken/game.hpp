#pragma once
#include<iostream>
#include<set>
#include<concepts>
#include<algorithm>
#include<queue>
#include<cassert>
#include<functional>
#include "../datastructure/csr_array.hpp"
#include "../random/generator.hpp"
template<typename T>
concept game_state=requires(T x,T y){
  x<y;
  {x.first}->std::convertible_to<bool>;
  x.next(std::declval<std::function<void(T)>>());
};
template<game_state T>
struct Game{
  std::vector<T>state;
  std::set<T>vis;
  csr_array<int>g,grev;
  std::vector<int>dp;
  std::vector<int>to_win;
  void dfs(T x){
    if(vis.contains(x))return;
    vis.insert(x);
    x.next([&](T y){dfs(y);});
  }
  void set_col(int r=255,int g=255,int b=255){
    std::cerr<<"\x1b[38;2;"<<r<<";"<<g<<";"<<b<<"m";
  }
  inline int get_key(T x){return std::lower_bound(state.begin(),state.end(),x)-state.begin();}
  int solve(T start){
    vis.clear();
    dfs(start);
    state=std::vector<T>(vis.begin(),vis.end());
    if(std::all_of(state.begin(),state.end(),[&](T x){return x.first==start.first;})){
      set_col(255,255,0);
      std::cerr<<"warning all player is same\n";
      set_col();
    }
    std::vector<std::pair<int,int>>init,revinit;
    int n=state.size();
    std::vector<int>deg(n);
    std::vector<bool>seen(n);
    for(T x:state){
      int keyx=get_key(x);
      x.next([&](T y){
        int keyy=get_key(y);
        if(seen[keyy])return;
        seen[keyy]=true;
        deg[keyx]++;
        init.emplace_back(keyx,keyy);
        revinit.emplace_back(keyy,keyx);
      });
      x.next([&](T y){
        int keyy=get_key(y);
        seen[keyy]=false;
      });
    }
    g=csr_array<int>(n,init);
    grev=csr_array<int>(n,revinit);
    dp.assign(n,0);
    to_win.assign(n,-1);
    std::queue<int>que;
    for(int i=0;i<n;i++)if(g[i].size()==0){
      dp[i]=-1;
      que.push(i);
    }
    while(!que.empty()){
      int x=que.front();que.pop();
      for(int y:grev[x])if(dp[y]==0){
        if((state[y].first!=state[x].first)==(dp[x]==-1)){
          dp[y]=1;
          to_win[y]=x;
          que.push(y);
          continue;
        }
        if(!--deg[y]){
          dp[y]=-1;
          que.push(y);
        }
      }
    }
    return dp[get_key(start)];
  }
  void play(T start,bool is_first){
    static_assert(requires(T x){std::cout<<x;});
    int idx=get_key(start);
    assert(!(state[idx]<start&&start<state[idx]));
    std::vector<T>history;
    auto write=[&]()->void {
      set_col(0,255,0);
      std::cerr<<"===================================state===================================\n";
      set_col();
      for(int i=0;i<(int)history.size();i++){
        set_col(0,0,255);
        std::cerr<<"-----------------------------------"<<i<<"-----------------------------------\n";
        set_col();
        std::cerr<<history[i]<<'\n';
        set_col(0,0,255);
        std::cerr<<"-----------------------------------"<<i<<"-----------------------------------\n";
        if(i+1<(int)history.size()){
          set_col(255,255,100);
          std::cerr<<"                                   |                                   \n";
          std::cerr<<"                                   v                                   \n";
          if(history[i].first==is_first)std::cerr<<"                                  you\n";
          else std::cerr<<"                                 solver\n";
          std::cerr<<"                                   |                                   \n";
          std::cerr<<"                                   v                                   \n";
        }
        set_col();
      }
      set_col(0,255,0);
      std::cerr<<"===================================state===================================\n";
      set_col();
    };
    while(true){
      history.push_back(state[idx]);
      if(state[idx].first==is_first){
        if(g[idx].size()==0){
          write();
          std::cerr<<"you lose\n";
          break;
        }
        write();
        std::cerr<<"choose next step\n";
        for(int i=0;i<g[idx].size();i++){
          set_col(0,0,255);
          std::cerr<<i<<'\n';
          set_col();
          std::cerr<<state[g[idx][i]]<<'\n';
        }
        int nidx=-1;
        while(true){
          std::cerr<<"input:";
          std::string s;
          std::cin>>s;
          if(std::all_of(s.begin(),s.end(),[](char c){return std::isdigit(c);})&&s.size()<=9&&std::stoi(s)<g[idx].size()){
            nidx=g[idx][std::stoi(s)];
            break;
          }
          else{
            set_col(255,0,0);
            std::cerr<<"invalid\n";
            set_col();
          }
        }
        idx=nidx;
      }
      else{
        if(g[idx].size()==0){
          write();
          std::cerr<<"you win\n";
          break;
        }
        if(to_win[idx]==-1)idx=g[idx][Random::range(g[idx].size())];
        else idx=to_win[idx];
      }
    }
  }
};