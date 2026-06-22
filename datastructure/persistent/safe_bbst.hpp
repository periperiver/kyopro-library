#pragma once
#include<vector>
#include<memory>
template<typename M>
struct PersistentRedBlackTree{
private:
  using S=typename M::S;
  using F=typename M::F;
  struct node;
  using np=std::shared_ptr<node>;
  struct node{
    S v;
    bool isblack,rev;
    F lazy;
    np left,right;
    int sz,rank;
    template<int need=7>
    static np push(np nd){
      if(!nd->left)return nd;
      np le,ri;
      if constexpr(need&1){
        if(nd->left->left)le=create_node(nd->left->left,nd->left->right,nd->left->isblack,nd->left->v,nd->left->lazy,nd->left->rev);
        else le=create_node(nd->left->v);
        le->propagate(nd->lazy);
        if(nd->rev)le->reverse();
      }
      else le=nd->left;
      if constexpr(need&2){
        if(nd->right->left)ri=create_node(nd->right->left,nd->right->right,nd->right->isblack,nd->right->v,nd->right->lazy,nd->right->rev);
        else ri=create_node(nd->right->v);
        ri->propagate(nd->lazy);
        if(nd->rev)ri->reverse();
      }
      else ri=nd->right;
      if constexpr(need&4)return create_node(le,ri,nd->isblack);
      else{
        nd->lazy=M::id();
        nd->rev=false;
        nd->left=le;
        nd->right=ri;
        return nd;
      }
    }
    void propagate(F lz){
      v=M::mapping(lz,v,sz);
      lazy=M::composition(lz,lazy);
    }
    void reverse(){
      std::swap(left,right);
      M::revS(v);
      rev^=1;
    }
    static np create_node(S vv){
      np nd=std::make_shared<node>();
      nd->v=vv;
      nd->isblack=true;
      nd->rev=false;
      nd->lazy=M::id();
      nd->left=nd->right=nullptr;
      nd->sz=1;
      nd->rank=0;
      return nd;
    }
    static np create_node(np l,np r,bool b){
      np nd=std::make_shared<node>();
      if(l){
        nd->v=M::op(l->v,r->v);
        nd->sz=l->sz+r->sz;
        nd->rank=l->rank+l->isblack;
        nd->left=l;
        nd->right=r;
      }
      else{
        nd->v=M::e();
        nd->sz=1;
        nd->rank=0;
        nd->left=nd->right=nullptr;
      }
      nd->rev=false;
      nd->lazy=M::id();
      nd->isblack=b;
      return nd;
    }
    static np create_node(np l,np r,bool b,S val,F lz,bool rv){
      np nd=std::make_shared<node>();
      nd->v=val;
      nd->left=l;
      nd->right=r;
      nd->isblack=b;
      nd->lazy=lz;
      nd->rev=rv;
      if(l){
        nd->sz=l->sz+r->sz;
        nd->rank=l->rank+l->isblack;
      }
      else{
        nd->sz=1;
        nd->rank=0;
      }
      return nd;
    }
  };
  np root;
  np imerge(np l,np r){
    if((l->rank)<(r->rank)){
      r=node::push(r);
      np c=imerge(l,r->left);
      if(r->isblack&&!c->isblack&&!c->left->isblack){
        if(r->right->isblack){
          return node::create_node(c->left,node::create_node(c->right,r->right,false),true);
        }
        else{
          r->right=node::push(r->right);
          return node::create_node(node::create_node(c->left,c->right,true),node::create_node(r->right->left,r->right->right,true),false);
        }
      }
      else{
        return node::create_node(c,r->right,r->isblack);
      }
    }
    else if(l->rank>r->rank){
      l=node::push(l);
      np c=imerge(l->right,r);
      if(l->isblack&&!c->isblack&&!c->right->isblack){
        if(l->left->isblack){
          return node::create_node(node::create_node(l->left,c->left,false),c->right,true);
        }
        else{
          l->left=node::push(l->left);
          return node::create_node(node::create_node(l->left->left,l->left->right,true),node::create_node(c->left,c->right,true),false);
        }
      }
      else{
        return node::create_node(l->left,c,l->isblack);
      }
    }
    else{
      return node::create_node(l,r,false);
    }
    return nullptr;
  }
  np imerge2(np l,np r){
    if(!l)return r;
    if(!r)return l;
    np nd=imerge(l,r);
    nd->isblack=true;
    return nd;
  }
  template<int need=1>
  np as_root(np nd){
    if(!nd||nd->isblack)return nd;
    if constexpr(need)return node::create_node(nd->left,nd->right,true,nd->v,nd->lazy,nd->rev);
    else{
      nd->isblack=true;
      return nd;
    }
  }
  template<int need,int top=1>
  std::pair<np,np>isplit(np nd,int k){
    if(k==0)return std::make_pair(nullptr,nd);
    if(k==nd->sz)return std::make_pair(nd,nullptr);
    if(k<nd->left->sz){
      nd=node::template push<top*4+(need|1)>(nd);
      auto [l,r]=isplit<need,0>(nd->left,k);
      if constexpr(need&2)return std::make_pair(l,imerge2(r,as_root<top>(nd->right)));
      else return std::make_pair(l,nullptr);
    }
    else if(k>nd->left->sz){
      nd=node::template push<top*4+(need|2)>(nd);
      np rr=nd->right;
      auto [l,r]=isplit<need,0>(nd->right,k-nd->left->sz);
      if constexpr(need&1)return std::make_pair(imerge2(as_root<top>(nd->left),l),r);
      else return std::make_pair(nullptr,r);
    }
    else{
      nd=node::template push<top*4+need>(nd);
      np res_l,res_r;
      if constexpr(need&1)res_l=as_root<top>(nd->left);
      if constexpr(need&2)res_r=as_root<top>(nd->right);
      return std::make_pair(res_l,res_r);
    }
  }
  PersistentRedBlackTree(np r):root(r){}
public:
  PersistentRedBlackTree():root(nullptr){}
  PersistentRedBlackTree(const std::vector<S>&a){
    if(a.empty())root=nullptr;
    else{
      auto dfs=[&](auto self,int l,int r)->np {
        if(l+1==r)return node::create_node(a[l]);
        int mid=(l+r)/2;
        np left=self(self,l,mid);
        np right=self(self,mid,r);
        if(left->rank!=right->rank){
          right->isblack=false;
        }
        return node::create_node(left,right,true);
      };
      root=dfs(dfs,0,a.size());
    }
  }
  [[nodiscard]]PersistentRedBlackTree merge(PersistentRedBlackTree rhs){
    return PersistentRedBlackTree(imerge2(this->root,rhs.root));
  }
  template<int need=3>
  [[nodiscard]]std::pair<PersistentRedBlackTree,PersistentRedBlackTree>split(int k){
    auto [l,r]=isplit<need>(root,k);
    return std::make_pair(PersistentRedBlackTree(l),PersistentRedBlackTree(r));
  }
  [[nodiscard]]PersistentRedBlackTree insert(int k,S v){
    auto [l,r]=isplit<3>(root,k);
    np nd=node::create_node(v);
    return PersistentRedBlackTree(imerge2(l,imerge2(nd,r)));
  }
  [[nodiscard]]PersistentRedBlackTree erase(int k){
    auto [left,right]=isplit<3>(root,k);
    auto [middle,right2]=isplit<2>(right,1);
    return PersistentRedBlackTree(imerge2(left,right2));
  }
  S prod(int l,int r){
    if(l==r)return M::e();
    auto [left,right]=isplit<3>(root,l);
    auto [middle,right2]=isplit<3>(right,r-l);
    return middle->v;
  }
  [[nodiscard]]PersistentRedBlackTree apply(int l,int r,F f){
    if(l==r)return root;
    auto [left,right]=isplit<3>(root,l);
    auto [middle,right2]=isplit<3>(right,r-l);
    np nd=node::create_node(middle->left,middle->right,middle->isblack,middle->v,middle->lazy,middle->rev);
    nd->propagate(f);
    return PersistentRedBlackTree(imerge2(as_root(left),imerge2(as_root(nd),as_root(right2))));
  }
  [[nodiscard]]PersistentRedBlackTree reverse(int l,int r){
    if(l==r)return root;
    auto [left,right]=isplit<3>(root,l);
    auto [middle,right2]=isplit<3>(right,r-l);
    np nd=node::create_node(middle->left,middle->right,middle->isblack,middle->v,middle->lazy,middle->rev);
    nd->reverse();
    return PersistentRedBlackTree(imerge2(as_root(left),imerge2(as_root(nd),as_root(right2))));
  }
  int size()const{return root?root->sz:0;}
  std::vector<S>to_vector()const{
    auto dfs=[&](auto self,np nd,bool rev,F lazy)->std::vector<S> {
      if(nd->left){
        std::vector<S>l=self(self,nd->left,rev^nd->rev,M::composition(lazy,nd->lazy));
        std::vector<S>r=self(self,nd->right,rev^nd->rev,M::composition(lazy,nd->lazy));
        if(rev)swap(l,r);
        l.insert(l.end(),r.begin(),r.end());
        return l;
      }
      else return std::vector<S>{M::mapping(lazy,nd->v,1)};
    };
    return dfs(dfs,root,false,M::id());
  }
};