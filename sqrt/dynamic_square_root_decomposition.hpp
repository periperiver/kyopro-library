#pragma once
#include "template.hpp"
template<typename K,typename V>
struct Sqrt2d{
private:
  struct node{
    K key;
    V v,sum;
    node *par,*left,*right;
    int cnt;
    node(K k_,V v_):key(k_),v(v_),sum(v_),par(nullptr),left(nullptr),right(nullptr),cnt(0){}
    node():key(0),v(0),sum(0),par(nullptr),left(nullptr),right(nullptr),cnt(0){}
    void update(){
      sum=v;
      if(left)sum+=left->sum;
      if(right)sum+=right->sum;
    }
    void splay(){
      while(this->par){
        node *p=this->par;
        if(!this->par->par){
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
          p->update();
        }
        else{
          node *pp=p->par;
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
          pp->update();
          p->update();
        }
      }
    }
  };
  int p;
  vector<node>pool;
  vector<node*>ptr;
  vector<pair<K,V>>dat;
  vector<node*>block;
  int n,b;
  node* create_node(K k,V v){
    node *nd=ptr[p++];
    nd->key=k;
    nd->v=nd->sum=v;
    nd->par=nd->left=nd->right=nullptr;
    nd->cnt=1;
    return nd;
  }
  void return_node(node *nd){
    ptr[--p]=nd;
  }
  V suffix_sum(node*&nd,K key){
    while(true){
      if(nd->key<key){
        if(nd->right)nd=nd->right;
        else break;
      }
      else{
        if(nd->left)nd=nd->left;
        else break;
      }
    }
    nd->splay();
    if(nd->key>=key)return nd->sum-(nd->left?nd->left->sum:0);
    else return nd->right?nd->right->sum:0;
  }
  node* merge(node *l,node *r){
    if(!l)return r;
    if(!r)return l;
    while(l->right)l=l->right;
    l->splay();
    l->right=r;
    r->par=l;
    l->update();
    return l;
  }
  void erase(node*&nd,K key,V val){
    assert(nd);
    while(true){
      if(nd->key==key){
        nd->splay();
        nd->v-=val;
        nd->sum-=val;
        if(!--nd->cnt){
          if(nd->left)nd->left->par=nullptr;
          if(nd->right)nd->right->par=nullptr;
          node *ret=nd;
          nd=merge(nd->left,nd->right);
          return_node(ret);
        }
        return;
      }
      else if(nd->key>key)nd=nd->left;
      else nd=nd->right;
    }
  }
  void insert(node*&nd,K key,V val){
    if(!nd)nd=create_node(key,val);
    else{
      while(true){
        if(nd->key==key){
          nd->splay();
          nd->v+=val;
          nd->update();
          nd->cnt++;
          break;
        }
        else if(nd->key>key){
          if(nd->left)nd=nd->left;
          else{
            nd->splay();
            node *nxt=create_node(key,val);
            nxt->left=nd->left;
            if(nxt->left)nxt->left->par=nxt;
            nd->par=nxt;
            nxt->right=nd;
            nd->left=nullptr;
            nxt->sum=val+nd->sum;
            nd->update();
            nd=nxt;
            break;
          }
        }
        else{
          if(nd->right)nd=nd->right;
          else{
            nd->splay();
            node *nxt=create_node(key,val);
            nxt->right=nd->right;
            if(nxt->right)nxt->right->par=nxt;
            nd->par=nxt;
            nxt->left=nd;
            nd->right=nullptr;
            nxt->sum=val+nd->sum;
            nd->update();
            nd=nxt;
            break;
          }
        }
      }
    }
  }
public:
  Sqrt2d(vector<pair<K,V>>&a):dat(a),n(a.size()),p(0),pool(a.size()),ptr(a.size()){
    b=sqrt(n*(31-__builtin_clz(n)))*3/2;
    block.resize((n+b-1)/b,nullptr);
    rep(i,n)ptr[i]=&pool[i];
    rep(i,n)insert(block[i/b],a[i].first,a[i].second);
  }
  Sqrt2d(vector<K>&a):n(a.size()),p(0),pool(a.size()),ptr(a.size()){
    b=sqrt(n*(32-__builtin_clz(n)))*3/2;
    b=clamp(b,1,n);
    block.resize((n+b-1)/b,nullptr);
    dat.resize(n);
    rep(i,n)ptr[i]=&pool[i];
    rep(i,n){
      dat[i]=make_pair(a[i],1);
      insert(block[i/b],a[i],1);
    }
  }
  void set(int i,K k,V v=1){
    erase(block[i/b],dat[i].first,dat[i].second);
    insert(block[i/b],k,v);
    dat[i]=make_pair(k,v);
  }
  V sum(int lx,int rx,int ly,int ry){
    V ret=0;
    if(rx-lx<b){
      reps(i,lx,rx)if(ly<=dat[i].first&&dat[i].first<ry)ret+=dat[i].second;
      return ret;
    }
    int lxx=(lx+b-1)/b*b,rxx=rx/b*b;
    reps(i,lx,lxx)if(ly<=dat[i].first&&dat[i].first<ry)ret+=dat[i].second;
    reps(i,rxx,rx)if(ly<=dat[i].first&&dat[i].first<ry)ret+=dat[i].second;
    lx=lxx/b,rx=rxx/b;
    reps(i,lx,rx)ret+=suffix_sum(block[i],ly)-suffix_sum(block[i],ry);
    return ret;
  }
  void print(node *nd){
    map<node*,int>mp;
    rep(i,n)mp[&pool[i]]=1+i;
    auto dfs=[&](auto self,node *nd)->void {
      if(!nd)return;
      debug(mp[nd],mp[nd->left],mp[nd->right],mp[nd->par],nd->key,nd->v,nd->sum);
      self(self,nd->left);
      self(self,nd->right);
    };
    dfs(dfs,nd);
  }
};