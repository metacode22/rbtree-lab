#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;

  p->root = p->nil;

  return p;
}


node_t * postOrder(rbtree *t, node_t * curr){
    if(curr->left == t->nil && curr->right == t->nil){
        return curr;
    }
    if(curr->left != t->nil){
        free(postOrder(t, curr->left));
    }
    if(curr->right != t->nil){
        free(postOrder(t, curr->right));
    }
    return curr;
}


void delete_rbtree(rbtree *t) {
    // TODO: reclaim the tree nodes's memory
    if(t->root != t->nil){
        free(postOrder(t, t->root));
    }
    free(t->nil);
    free(t);
}

void right_Rotate(rbtree *T, node_t *x){
  node_t* y = x->left;    
  x->left = y->right;       
  if (y->right != T->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;    
  if(x->parent == T->nil){
    T->root = y;
  }
  else if(x == x->parent->left){
    x->parent->left = y;
  }
  else {x->parent->right = y;}
  y->right = x;    
  x->parent = y;
}

void left_Rotate(rbtree *T, node_t *x){
  node_t* y = x->right;     // y 설정
  x->right = y->left;       // y의 왼쪽 서브 트리를 x의 오른쪽 서브 트리로 옮기기
  if (y->left != T->nil){
    y->left->parent = x;
  }
  y->parent = x->parent;    // x의 부모를 y로 연결한다.
  if(x->parent == T->nil){
    T->root = y;
  }
  else if(x == x->parent->left){
    x->parent->left = y;
  }
  else x->parent->right = y;
  y->left = x;    // x를 y의 왼쪽에 놓는다.
  x->parent = y;
}


void rbtree_insert_fixup(rbtree *t, node_t* node) {
  while(node->parent->color == RBTREE_RED){
    if (node->parent == node->parent->parent->left){
      node_t* tmp = node->parent->parent->right;
      if (tmp->color == RBTREE_RED) {   // 경우 1
        node->parent->color = RBTREE_BLACK;
        tmp->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        node = node->parent->parent;
      }
      else {
        if ( node == node->parent->right){   // 경우 2
          node = node->parent;
          left_Rotate(t,node);
      }
        node->parent->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        right_Rotate(t,node->parent->parent);  
    }
  }
  else {
      node_t* tmp = node->parent->parent->left;
      if (tmp->color == RBTREE_RED) {   // 경우 1
        node->parent->color = RBTREE_BLACK;
        tmp->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        node = node->parent->parent;
      }
      else {
        if ( node == node->parent->left){   // 경우 2
          node = node->parent;
          right_Rotate(t,node);
      }
        node->parent->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        left_Rotate(t,node->parent->parent);  
      }
    }
  }
}



node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t* new_node = (node_t *)calloc(1,sizeof(node_t));
  node_t* y = t->nil;
  node_t* x = t->root;
  
  while (x != t->nil){
    y = x;
    if (key < x->key){
      x = x->left;
    }
    else{
      x = x->right;
    }
  }
  new_node->parent = y;
  if (y == t->nil){
    t->root = new_node;
  }
  else if (key < y->key){
    y->left = new_node;
  }
  else { 
    y->right = new_node;
  }

  new_node->key = key;

  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->color = RBTREE_RED;
  
  rbtree_insert_fixup(t,new_node);
  t->root->color = RBTREE_BLACK;
  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  if (t->root == t->nil){
    return NULL;
  }
  node_t *curr = t->root;
  while(curr != t->nil){
    if(key < curr->key) {
      curr = curr->left;
    }
    else if (key > curr->key){
      curr = curr->right;
    }
    else {
      return curr;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  if (t->root == t->nil) {
    return NULL;
  }
  node_t *curr = t->root;
  while (curr->left != t->nil){
    curr = curr->left;
  }
  return curr;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if (t->root == t->nil){
    return NULL;
  }
  node_t *curr = t->root;
  while(curr->right != t->nil){
    curr = curr->right;
  }
  return curr;
}


void transplant(rbtree *t, node_t *u, node_t *v){
  if (u->parent == t->nil){
    t->root = v;
  }
  else if (u == u->parent->left){
    u->parent->left = v;
  }
  else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

void delete_fixup(rbtree *t, node_t *x) {
  while (x!= t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {
      node_t *w = x->parent->right;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_Rotate(t,x->parent);
        w = x->parent->right;
      }
      if( w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else if (w->right->color == RBTREE_BLACK) {
        w->left->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        right_Rotate(t,w);
        w = x->parent->right;
      }
      w->color = x->parent->color;
      x->parent->color = RBTREE_BLACK;
      w->right->color = RBTREE_BLACK;
      left_Rotate(t,x->parent);
      x = t->root;
    }
    else {
      node_t *w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_Rotate(t,x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{ 
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_Rotate(t,w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        left_Rotate(t,x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  if (p == NULL) {
    return 0;
  }
  node_t *y = p;
  node_t *x;
  color_t y_original = y->color;
  if (p->left == t->nil){
    x = p->left;
    transplant(t,p,p->right);
  }
  else if (p->right == t->nil) {
    x = p->left;
    transplant(t,p,p->left);
  }
  else {
    y = p->right;
    while (y->left != t->nil){
      y = y->left;
    }
    y_original = y->color;
    x = y->right;
    if (y->parent == p) {
      x->parent = y;
    }
    else {
      transplant(t,y,y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    transplant(t,p,y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if (y_original == RBTREE_BLACK) {
    delete_fixup(t,x);
  }
  free(p);
  return 0;
}

void inOrder(const rbtree *t, node_t * curr, key_t *arr, size_t *pcnt, const size_t n){
    if(curr == t->nil){
        return;
    }
    inOrder(t, curr->left, arr, pcnt, n);
    if(*pcnt < n){
        arr[(*pcnt)++] = curr->key;
    }
    else{
        return;
    }
    inOrder(t, curr->right, arr, pcnt, n);
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    // TODO: implement to_array
    if(t->root == t->nil){
        return 0;
    }
    else{
        size_t cnt = 0;
        inOrder(t, t->root, arr, &cnt, n);
    }
    return 0;
}