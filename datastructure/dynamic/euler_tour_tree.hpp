#pragma once
#include<vector>
#include<unordered_map>
#include<cassert>
using namespace std;
template<typename S,auto op,auto e,typename F,auto mapping,auto composition,auto id>
struct EulerTourTree{
private:
  struct node{
    node *left,*right,*par;
    S v,sum;
    F lazy;
    int from,to;
    int sz;
    node(){}
    node(int u,S x=e()):left(nullptr),right(nullptr),par(nullptr),v(x),sum(x),lazy(id()),from(u),to(u),sz(1){}
    node(int from_,int to_,int):left(nullptr),right(nullptr),par(nullptr),v(e()),sum(e()),lazy(id()),from(from_),to(to_),sz(0){}
    inline void update(){
      sum=v;
      sz=from==to;
      if(left)sum=op(left->sum,sum),sz+=left->sz;
      if(right)sum=op(sum,right->sum),sz+=right->sz;
    }
    inline void push(){
      if(left)left->propagate(lazy);
      if(right)right->propagate(lazy);
      lazy=id();
    }
    inline void propagate(const F&f){
      v=mapping(f,v,from==to);
      sum=mapping(f,sum,sz);
      lazy=composition(f,lazy);
    }
    void splay(){
      while(this->par){
        node *p=this->par;
        if(!this->par->par){
          p->push();
          this->push();
          if(p->left==this){
            if(this->right)this->right->par=p;
            p->left=this->right;
            this->right=p;
            this->par=nullptr;
            p->par=this;
          }
          else{
            if(this->left)this->left->par=p;
            p->right=this->left;
            this->left=p;
            this->par=nullptr;
            p->par=this;
          }
          this->sum=p->sum;
          this->sz=p->sz;
          p->update();
        }
        else{
          node *pp=p->par;
          pp->push();
          p->push();
          this->push();
          if(p->left==this){
            if(pp->left==p){
              if(this->right)this->right->par=p;
              if(p->right)p->right->par=pp;
              p->left=this->right;
              this->right=p;
              pp->left=p->right;
              p->right=pp;
              if(pp->par){
                if(pp->par->left==pp)pp->par->left=this;
                else pp->par->right=this;
              }
              this->par=pp->par;
              p->par=this;
              pp->par=p;
            }
            else{
              if(this->left)this->left->par=pp;
              if(this->right)this->right->par=p;
              pp->right=this->left;
              p->left=this->right;
              if(pp->par){
                if(pp->par->left==pp)pp->par->left=this;
                else pp->par->right=this;
              }
              this->par=pp->par;
              pp->par=this;
              p->par=this;
              this->left=pp;
              this->right=p;
            }
          }
          else{
            if(pp->left==p){
              if(this->left)this->left->par=p;
              if(this->right)this->right->par=pp;
              p->right=this->left;
              pp->left=this->right;
              if(pp->par){
                if(pp->par->left==pp)pp->par->left=this;
                else pp->par->right=this;
              }
              this->par=pp->par;
              p->par=this;
              pp->par=this;
              this->left=p;
              this->right=pp;
            }
            else{
              if(this->left)this->left->par=p;
              if(p->left)p->left->par=pp;
              p->right=this->left;
              this->left=p;
              pp->right=p->left;
              p->left=pp;
              if(pp->par){
                if(pp->par->left==pp)pp->par->left=this;
                else pp->par->right=this;
              }
              this->par=pp->par;
              p->par=this;
              pp->par=p;
            }
          }
          this->sum=pp->sum;
          this->sz=pp->sz;
          pp->update();
          p->update();
        }
      }
      this->push();
    }
  };
  vector<node>dat;
  unordered_map<ll,node*>edge;
  void reroot(node*nd){
    nd->splay();
    if(!nd->left)return;
    node *le=nd->left;
    le->par=nullptr;
    nd->left=nullptr;
    nd->update();
    while(le->left){
      le->push();
      le=le->left;
    }
    le->splay();
    le->left=nd;
    nd->par=le;
    le->update();
    nd->splay();
  }
  node* merge(node *l,node *r){
    if(!l)return r;
    if(!r)return l;
    while(l->right){
      l->push();
      l=l->right;
    }
    l->splay();
    l->right=r;
    r->par=l;
    l->update();
    return l;
  }
public:
  EulerTourTree(){}
  EulerTourTree(int n):dat(n){
    for(int i=0;i<n;i++)dat[i]=node(i);
  }
  EulerTourTree(const vector<S>&init):dat(init.size()){
    for(int i=0;i<dat.size();i++)dat[i]=node(i,init[i]);
  }
  void link(int u,int v){
    node *uv=new node(u,v,0);
    node *vu=new node(v,u,0);
    edge[ll(u)*dat.size()+v]=uv;
    edge[ll(v)*dat.size()+u]=vu;
    reroot(&dat[u]),reroot(&dat[v]);
    dat[u].par=uv;
    dat[v].par=uv;
    uv->left=&dat[u];
    uv->right=&dat[v];
    vu->left=uv;
    uv->par=vu;
    uv->update();
    vu->update();
  }
  void cut(int u,int v){
    node *uv=edge[ll(u)*dat.size()+v],*vu=edge[ll(v)*dat.size()+u];
    edge.erase(ll(u)*dat.size()+v),edge.erase(ll(v)*dat.size()+u);
    uv->splay(),vu->splay();
    node *p=uv->par;
    bool ri=uv==vu->right;
    if(p!=vu){
      ri=p==vu->right;
      p->par=nullptr;
      uv->splay();
    }
    if(uv->left)uv->left->par=nullptr;
    if(uv->right)uv->right->par=nullptr;
    if(ri){
      if(vu->left)vu->left->par=nullptr;
      merge(vu->left,uv->right);
    }
    else{
      if(vu->right)vu->right->par=nullptr;
      merge(uv->left,vu->right);
    }
    delete(uv);
    delete(vu);
  }
  S get(int u)const{return dat[u].v;}
  void set(int u,S x){
    dat[u].splay();
    dat[u].v=x;
    dat[u].update();
  }
  void apply(int u,const F&f){
    dat[u].splay();
    dat[u].propagate(f);
  }
  S prod(int u){
    dat[u].splay();
    return dat[u].sum;
  }
  bool same(int u,int v){
    dat[u].splay();
    dat[v].splay();
    return dat[u].par;
  }
};