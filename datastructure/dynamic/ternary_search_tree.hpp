#pragma once
template<typename Key,typename Val>
struct TernarySearchTree{
private:
  using inner_key=typename Key::value_type;
  struct node{
    node *left=nullptr,*right=nullptr,*par=nullptr,*middle=nullptr;
    inner_key key;
    Val value;
    bool has_value=false;
    node():key(),value(){}
    node(const inner_key&key_,Val value_=Val()):key(key_),value(value_){}
    ~node(){
      if(left)delete(left);
      if(right)delete(right);
      if(middle)delete(middle);
    }
    inline bool is_root()const{
      if(!par)return true;
      return par->left!=this&&par->right!=this;
    }
    void splay(){
      while(!this->is_root()){
        node *p=this->par;
        if(p->is_root()){
          if(p->left==this){
            if(this->right)this->right->par=p;
            p->left=this->right;
            this->right=p;
            this->par=p->par;
            p->par=this;
          }
          else{
            if(this->left)this->left->par=p;
            p->right=this->left;
            this->left=p;
            this->par=p->par;
            p->par=this;
          }
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
                else if(pp->par->right==pp)pp->par->right=this;
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
                else if(pp->par->right==pp)pp->par->right=this;
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
                else if(pp->par->right==pp)pp->par->right=this;
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
                else if(pp->par->right==pp)pp->par->right=this;
              }
              this->par=pp->par;
              p->par=this;
              pp->par=p;
            }
          }
        }
      }
    }
  };
  struct tst_iterator{
    node *nd;
    Key key;
    tst_iterator(node *nd_):nd(nd_){}
    void step(){
      if(nd->middle){
        node *pre=nd;
        nd=nd->middle;
        while(nd->left)nd=nd->left;
        nd->splay();
        pre->middle=nd;
        nd->par=pre;
        key.push_back(nd->key);
      }
      else{
        while(true){
          nd->splay();
          if(!nd->par)break;
          nd->par->middle=nd;
          if(nd->right)break;
          nd=nd->par;
          key.pop_back();
        }
        if(!nd->par){
          nd=nullptr;
          return;
        }
        node *pre=nd->par;
        nd=nd->right;
        while(nd->left)nd=nd->left;
        nd->splay();
        pre->middle=nd;
        nd->par=pre;
        key.pop_back();
        key.push_back(nd->key);
      }
    }
    tst_iterator &operator++(){
      do{
        step();
      }while(nd&&!nd->has_value);
      return *this;
    }
    pair<const Key&,Val&> operator*()const{
      return {key,nd->value};
    }
    bool operator!=(const tst_iterator&rhs){return nd!=rhs.nd;}
  };
  static node dummy;
  template<bool search_only>
  void get_k(node*&nd,const inner_key&k){
    node *p=nd->par;
    node *lnd=&dummy,*rnd=&dummy;
    while(!(nd->key==k)){
      if(k<nd->key){
        if(!nd->left){
          if constexpr(search_only)break;
          else nd->left=new node(k);
        }
        node *ch=nd->left;
        if(ch->key==k){
          rnd->left=nd;
          nd->par=rnd;
          rnd=nd;
          nd=ch;
        }
        else if(k<ch->key){
          if(!ch->left){
            if constexpr(search_only)break;
            else ch->left=new node(k);
          }
          node *lele=ch->left;
          nd->left=ch->right;
          if(nd->left)nd->left->par=nd;
          ch->right=nd;
          nd->par=ch;
          rnd->left=ch;
          ch->par=rnd;
          rnd=ch;
          nd=lele;
        }
        else{
          if(!ch->right){
            if constexpr(search_only)break;
            else ch->right=new node(k);
          }
          node *leri=ch->right;
          lnd->right=ch;
          ch->par=lnd;
          lnd=ch;
          rnd->left=nd;
          nd->par=rnd;
          rnd=nd;
          nd=leri;
        }
      }
      else{
        if(!nd->right){
          if constexpr(search_only)break;
          else nd->right=new node(k);
        }
        node *ch=nd->right;
        if(ch->key==k){
          lnd->right=nd;
          nd->par=lnd;
          lnd=nd;
          nd=ch;
        }
        else if(k<ch->key){
          if(!ch->left){
            if constexpr(search_only)break;
            else ch->left=new node(k);
          }
          node *rile=ch->left;
          lnd->right=nd;
          nd->par=lnd;
          lnd=nd;
          rnd->left=ch;
          ch->par=rnd;
          rnd=ch;
          nd=rile;
        }
        else{
          if(!ch->right){
            if constexpr(search_only)break;
            else ch->right=new node(k);
          }
          node *riri=ch->right;
          nd->right=ch->left;
          if(nd->right)nd->right->par=nd;
          nd->par=ch;
          ch->left=nd;
          lnd->right=ch;
          ch->par=lnd;
          lnd=ch;
          nd=riri;
        }
      }
    }
    if(lnd!=&dummy){
      lnd->right=nd->left;
      if(lnd->right)lnd->right->par=lnd;
      nd->left=dummy.right;
      nd->left->par=nd;
    }
    if(rnd!=&dummy){
      rnd->left=nd->right;
      if(rnd->left)rnd->left->par=rnd;
      nd->right=dummy.left;
      nd->right->par=nd;
    }
    p->middle=nd;
    nd->par=p;
  }
public:
  node *root;
  TernarySearchTree():root(new node(inner_key())){}
  TernarySearchTree(const TernarySearchTree&a){
    auto dfs=[&](auto self,node *nd)->node* {
      if(!nd)return nullptr;
      node *nd2=new node(nd->key,nd->value);
      nd2->left=self(self,nd->left);
      nd2->right=self(self,nd->right);
      nd2->middle=self(self,nd->middle);
      if(nd2->left)nd2->left->par=nd2;
      if(nd2->right)nd2->right->par=nd2;
      if(nd2->middle)nd2->middle->par=nd2;
      nd2->has_value=nd->has_value;
      return nd2;
    };
    root=dfs(dfs,a.root);
  }
  Val &operator[](const Key&key){
    node *nd=root;
    for(const inner_key&k:key){
      if(!nd->middle){
        nd->middle=new node(k);
        nd->middle->par=nd;
        nd=nd->middle;
      }
      else{
        nd=nd->middle;
        get_k<false>(nd,k);
      }
    }
    nd->has_value=true;
    return nd->value;
  }
  bool contains(const Key&key){
    node *nd=root;
    for(const inner_key&k:key){
      if(!nd->middle)return false;
      nd=nd->middle;
      get_k<true>(nd,k);
      if(!(nd->key==k))return false;
    }
    return nd->has_value;
  }
  //削除に成功->true,失敗->false
  bool erase(const Key&key){
    node *nd=root;
    for(const inner_key&k:key){
      if(!nd->middle)return false;
      nd=nd->middle;
      get_k<true>(nd,k);
      if(!(nd->key==k))return false;
    }
    if(nd->has_value){
      nd->has_value=false;
      return true;
    }
    else return false;
  }
  tst_iterator begin(){
    tst_iterator ret(root);
    while(ret.nd&&!ret.nd->has_value)ret.step();
    return ret;
  }
  tst_iterator end(){
    return tst_iterator(nullptr);
  }
  ~TernarySearchTree(){
    dummy.left=dummy.right=nullptr;
    delete(root);
  }
};
template<typename Key,typename Val>
typename TernarySearchTree<Key,Val>::node TernarySearchTree<Key,Val>::dummy;
template<typename Key,typename Val>
void swap(TernarySearchTree<Key,Val>&lhs,TernarySearchTree<Key,Val>&rhs){
  std::swap(lhs.root,rhs.root);
}