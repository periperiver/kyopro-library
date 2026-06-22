#pragma once
#include "template.hpp"
template<typename I,typename S,auto op,auto e>
struct DynamicSegmentTree2d{
private:
  struct node{
    I key;
    S v,sum;
    node *par,*left,*right;
    node(I key_,S v_):key(key_),v(v_),sum(v_),par(nullptr),left(nullptr),right(nullptr){}
    inline void update(){
      sum=v;
      if(left)sum=op(left->sum,sum);
      if(right)sum=op(sum,right->sum);
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
  bool lower_bound(node*&nd,I key){
    while(nd){
      if(nd->key==key){
        nd->splay();
        return true;
      }
      else if(nd->key>key){
        if(nd->left)nd=nd->left;
        else{
          nd->splay();
          return true;
        }
      }
      else{
        if(nd->right)nd=nd->right;
        else{
          nd->splay();
          if(nd->right){
            nd=nd->right;
            while(nd->left)nd=nd->left;
            nd->splay();
            return true;
          }
          return false;
        }
      }
    }
    return false;
  }
  void iset(node*&root,I key,S val){
    if(!root)root=new node(key,val);
    else{
      while(true){
        if(root->key==key){
          root->splay();
          root->v=val;
          root->update();
          break;
        }
        else if(root->key>key){
          if(root->left)root=root->left;
          else{
            root->splay();
            node *nd=new node(key,val);
            nd->left=root->left;
            if(nd->left)nd->left->par=nd;
            nd->right=root;
            root->par=nd;
            root->left=nullptr;
            root->update();
            nd->update();
            root=nd;
            break;
          }
        }
        else{
          if(root->right)root=root->right;
          else{
            root->splay();
            node *nd=new node(key,val);
            nd->right=root->right;
            if(nd->right)nd->right->par=nd;
            nd->left=root;
            root->par=nd;
            root->right=nullptr;
            root->update();
            nd->update();
            root=nd;
            break;
          }
        }
      }
    }
  }
  S iget(node*&root,I key){
    if(!root)return e();
    else{
      while(true){
        if(root->key==key){
          root->splay();
          return root->v;
        }
        else if(root->key>key){
          if(root->left)root=root->left;
          else break;
        }
        else{
          if(root->right)root=root->right;
          else break;
        }
      }
      root->splay();
      return e();
    }
  }
  S iprod(node*&root,I l,I r){
    if(!root)return e();
    if(!lower_bound(root,l))return e();
    node *nd=root->left;
    root->left=nullptr;
    root->update();
    node *pre=root;
    if(!lower_bound(root,r)){
      S ret=root->sum;
      root=pre;
      root->splay();
      root->left=nd;
      if(nd)nd->par=root;
      root->update();
      return ret;
    }
    S ret=root->left?root->left->sum:e();
    root=pre;
    root->splay();
    root->left=nd;
    if(nd)nd->par=root;
    root->update();
    return ret;
  }
  vector<node*>dat;
public:
  DynamicSegmentTree2d(int n){
    int z=1;
    while(z<n)z<<=1;
    dat.resize(z*2,nullptr);
  }
  void set(int x,I y,S val){
    x+=dat.size()>>1;
    iset(dat[x],y,val);
    while(x>=2){
      val=op(val,iget(dat[x^1],y));
      x>>=1;
      iset(dat[x],y,val);
    }
  }
  S get(int x,I y){
    x+=dat.size()>>1;
    return iget(dat[x],y);
  }
  S prod(int lx,int rx,I ly,I ry){
    lx+=dat.size()>>1,rx+=dat.size()>>1;
    S ret=e();
    while(lx<rx){
      if(lx&1)ret=op(ret,iprod(dat[lx++],ly,ry));
      if(rx&1)ret=op(ret,iprod(dat[--rx],ly,ry));
      lx>>=1,rx>>=1;
    }
    return ret;
  }
};