#include <stdio.h> // 테스트용
#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil = nil;
  p->nil->color = RBTREE_BLACK;
  p->root = nil;
  
  return p;  
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  // free(t->nil);
  remove_subtree(t, t->root);
  free(t->nil);
  free(t);
}

void remove_subtree(rbtree *t, node_t *node)
{
  if (node != t->nil)
  {
    remove_subtree(t, node->left);
    remove_subtree(t, node->right);
    free(node);
  }
}

void left_rotate(rbtree *t, node_t * x){
  node_t *y = x ->right;
  x ->right = y->left;

  if (y->left != t->nil){
    y->left->parent = x;
  }
  
  y->parent = x->parent;

  if (x->parent == t->nil){
    t->root = y;
  }else if (x == x->parent->left){
    x->parent->left = y;
  }else{
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x){
  node_t *y = x ->left;
  x ->left = y->right;

  if (y->right != t->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil){
    t->root = y;
  }else if (x == x->parent->right){
    x->parent->right = y;
  }else{
    x->parent->left = y;
  }
  
  y->right = x;
  x->parent = y;
}

void rb_insert_fixup(rbtree *t, node_t * z){
  node_t * y = NULL;
  while(z->parent->color == RBTREE_RED){
    if (z->parent == z->parent->parent->left){
      y = z->parent->parent->right;

      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }else{
        //삽입하는 노드가 부모의 오른쪽 자식일 때
        if(z == z->parent->right){
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }else{
      y = z->parent->parent->left;
      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }else{
        //삽입하는 노드가 부모의 왼쪽 자식일 때
        if(z == z->parent->left){
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }

  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t * x = t->root;
  node_t * y = t->nil;
  // 삽입 위치 탐색
  while (x != t->nil){
    y = x;
    x = (x->key > key) ? x->left :  x->right;
  }

  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;
  z->parent = y;

  if (y == t->nil){
    t->root = z;
  }else if (z->key < y->key){
    y->left = z;
  }else{
    y->right = z;
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  rb_insert_fixup(t, z);
  return t->root;
}

node_t *search_node(rbtree *t, node_t* root, const key_t key){
  if(root == t->nil){
    return NULL;
  }else if(root->key == key){
    return root;
  }else if (root->key >key){
    return search_node(t, root->left, key);
  }else{
    return search_node(t, root->right, key);
  }
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return search_node(t, t->root, key);
  // return t->root;
}

// 특정 노드에서의 최소값
node_t *tree_min(rbtree *t, node_t *x) {
  // TODO: implement find
  while (x->left != t->nil){
    x= x->left;
  }
  return x;
}
// 특정 노드의 최대값
node_t *tree_max(rbtree *t, node_t *x) {
  // TODO: implement find
  while (x->right != t->nil){
    x= x->right;
  }
  return x;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *min_node = t->root;
  
  while (min_node->left != t->nil){
    min_node = min_node->left;
  }
  return min_node; 
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *max_node = t->root;
  
  while (max_node->right != t->nil){
    max_node = max_node->right;
  }
  return max_node; 
}

void rb_translant(rbtree *t, node_t *u, node_t *v){
  // 이게 머하는 거지???
  if (u->parent == t->nil){
    t->root = v;
  }else if (u == u->parent->left){
    u->parent->left = v;
  }else u->parent->right = v;
    v->parent = u->parent;
}

void rb_delete_fixup(rbtree *t, node_t* x){
  node_t *w = NULL;
  while(x != t->root && x->color == RBTREE_BLACK){
    if(x == x->parent->left){
      w = x->parent->right;

      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        
        if (w->right->color == RBTREE_BLACK){
          w->left->color == RBTREE_BLACK;
          w->color == RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }else{
      w = x->parent->left;
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if (w->left->color == RBTREE_BLACK){
          w->right->color == RBTREE_BLACK;
          w->color == RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  if (z == NULL){
    return 0;
  }
  node_t *x = NULL;

  node_t *y = z;
  color_t y_color = y->color;
  if (z->left == t->nil){
    x = z->right;
    rb_translant(t, z, z->right);
  }else if (z->right == t->nil){
    x = z->left;
    rb_translant(t, z, z->left);
  }else{
    y = tree_min(t, z->right);
    // y = tree_max(t, z->left);
    y_color = y->color;
    x = y->right;

    if (y->parent == z){
      x->parent = y;
    }else{
      rb_translant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }

    rb_translant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  
  if (y_color == RBTREE_BLACK) 
    rb_delete_fixup(t, x);
  
  free(z);
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int idx = 0;
  int *pidx = &idx;
  inorder_to_array(t, t->root, pidx, arr);
  return 0;
}

void inorder_to_array(rbtree *t, node_t *root, int *pidx, key_t *arr){
  if (root != t->nil){
    inorder_to_array(t, root->left, pidx,arr);
    arr[(*pidx)++] = root->key;
    inorder_to_array(t, root->right, pidx,arr);
  }
}