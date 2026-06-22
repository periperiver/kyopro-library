#pragma once
#include "lazy_segmenttree.hpp"
#include "monoid/rangeadd_rangemin.hpp"
struct PermutationTree{
  enum node_type{
    inc,dec,prime,leaf
  };
  struct node{
    int par;
    int l,r;
    int x;
    std::vector<int>child;
    node_type type;
  };
  int root=-1;
  std::vector<node>nd;
  PermutationTree(const std::vector<int>p){
    std::vector<int>mn,mx,st;
    LazySegmentTree<RangeAddRangeMin<int>>seg(std::vector<int>(p.size()));
    auto add=[&](int par,int ch)->void {
      nd[par].child.push_back(ch);
      nd[ch].par=par;
      nd[par].l=std::min(nd[par].l,nd[ch].l);
      nd[par].r=std::max(nd[par].r,nd[ch].r);
      nd[par].x=std::min(nd[par].x,nd[ch].x);
    };
    for(int i=0;i<(int)p.size();i++){
      while(!mn.empty()){
        int j=mn.back();
        if(p[j]>p[i]){
          mn.pop_back();
          seg.apply(mn.empty()?0:mn.back()+1,j+1,p[j]-p[i]);
        }
        else break;
      }
      mn.push_back(i);
      while(!mx.empty()){
        int j=mx.back();
        if(p[j]<p[i]){
          mx.pop_back();
          seg.apply(mx.empty()?0:mx.back()+1,j+1,p[i]-p[j]);
        }
        else break;
      }
      mx.push_back(i);
      int now=nd.size();
      {
        node &n=nd.emplace_back();
        n.l=i,n.r=i+1,n.type=node_type::leaf,n.x=p[i];
      }
      while(true){
        node_type t=node_type::leaf;
        if(!st.empty()&&nd[st.back()].x+nd[st.back()].r-nd[st.back()].l==nd[now].x)t=node_type::inc;
        if(!st.empty()&&nd[st.back()].x==nd[now].x+nd[now].r-nd[now].l)t=node_type::dec;
        if(t!=node_type::leaf){
          node&v=nd[st.back()];
          if(v.type==t){
            add(st.back(),now);
            now=st.back();
            st.pop_back();
          }
          else{
            int j=st.back();
            node&n=nd.emplace_back();
            n.type=t,n.l=nd[j].l,n.r=nd[j].r,n.x=nd[j].x;
            n.child.push_back(j);
            nd[j].par=nd.size()-1;
            add(nd.size()-1,now);
            now=nd.size()-1;
            st.pop_back();
          }
        }
        else if(seg.prod(0,i-(nd[now].r-nd[now].l)+1)==0){
          node&n=nd.emplace_back();
          n.l=nd[now].l,n.r=nd[now].r,n.x=nd[now].x;
          n.child.push_back(now);
          nd[now].par=nd.size()-1;
          n.type=node_type::prime;
          now=nd.size()-1;
          int y=n.x+n.r-n.l;
          do{
            add(now,st.back());
            y=std::max(y,nd[st.back()].x+nd[st.back()].r-nd[st.back()].l);
            st.pop_back();
          }while(y-nd[now].x!=nd[now].r-nd[now].l);
          std::reverse(nd[now].child.begin(),nd[now].child.end());
        }
        else break;
      }
      st.push_back(now);
      seg.apply(0,i+1,-1);
    }
    root=st[0];
    nd[root].par=-1;
  }
  node& operator[](int i){return nd[i];}
  const node& operator[](int i)const{return nd[i];}
  int size()const{return nd.size();}
};