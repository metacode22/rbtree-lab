#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); // 메모리 먼저 할당하고
  
  p->nil = (node_t *)calloc(1, sizeof(node_t)); //t->nil에 메모리 할당
  // 어차피 nil은 다 통일! 바닥에 있으니까 이건 new tree 초기화할 때 nil 만들어둬야지.
  p->root = p->nil;
  p->nil->color = RBTREE_BLACK;
  // p->nil->key = p->nil->left = p->nil->right = NULL;
  // TODO: initialize struct if needed
  return p;
}

// 삭제: 이따가 진행
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t->root != t->nil){
    free(_delete(t, t->root));
  }
  free(t->nil);
  free(t);
}

node_t *_delete(rbtree *t, node_t * curr){
  if (curr->left == t->nil && curr->right == t->nil) {
    return curr; // 양쪽 자식이 하나도 없으면 자기 자신을 삭제
  }

  if (curr->left !=t->nil) {
    free(_delete(t, curr->left)); //왼쪽에 자식이 있으면 재귀 타고 간다
  }

  if (curr->right != t->nil){
    free(_delete(t, curr->right)); //오른쪽도 재귀
  }
  return curr;
}

// 삽입

//insert에 사용할 보조 함수 void left_rotate(rbtree *t, const key_t key)

void left_rotation(rbtree *t, node_t *x) {
    node_t *y = x->right;
    x->right = y->left; // x 오른쪽 자식에 y->left 달아
    if (y->left != t->nil){
      y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent ==t->nil) {// x가 루트 노드이면 (루트 노드의 부모는 t->nil)
      t->root = y; // 이제 y가 루트 노드가 되어야 함
    }
    else if (x == x->parent->left){ // x가 왼쪽 자식 노드이면
      x->parent->left = y;
    }
    else {
      x->parent->right = y; // x가 오른쪽 자식 노드
    }
    y->left = x;
    x->parent = y;
}


void right_rotation(rbtree *t, node_t *x) {
    node_t *y = x->left;
    x->left = y->right; // x 오른쪽 자식에 y->left 달아
    //
    if (y->right != t->nil){
      y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent ==t->nil) {// x가 루트 노드이면 (루트 노드의 부모는 t->nil)
      t->root = y; // 이제 y가 루트 노드가 되어야 함
    }
    else if (x == x->parent->left){ // y가 왼쪽 자식 노드이면
      x->parent->left = y;
    }
    else {
      x->parent->right = y; // x가 오른쪽 자식 노드
    }
    y->right = x;
    x->parent = y;
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *y = t->nil;
  node_t *x = t->root;

  while (x != t->nil){
    y = x;
    if (x->key == key) {
      printf("같은 키가 있습니다.\n");
      return x;
    }
    else if (key < x->key){
      x = x->left;
    } 
    else {
      x = x->right;
    }
  }
  //x가 null을 가리키면 그때가 z를 삽입할 때!

  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;
  z->parent = y; //x가 NIL을 가리키고 y는 그 위 노드 가리킴 
  if (y == t->nil) //애초에 트리가 비어있다면 y는 t->nil 그대로
    t->root = z; // Case 1 (Case 2는 고려할 필요 X => 알아서 pass)
  else if (z->key < y->key) // y 왼쪽 자식으로 와야지.
    y->left = z;
  else
    y->right = z;
  
  z->left = t->nil; 
  z->right = t->nil;
  z->color = RBTREE_RED;
  rb_insert_fixup(t, z);
  return t->root;
}

void rb_insert_fixup(rbtree* t, node_t *z){
  while (z->parent->color ==RBTREE_RED) {
    if (z->parent == z->parent->parent->left){ //z의 parent가 왼쪽 자식 노드이면
      node_t *uncle = z->parent->parent->right;
      if (uncle->color == RBTREE_RED) { // 경우 1: z->uncle의 색이 red
        z->parent->color = RBTREE_BLACK; // 색 바꿔주고
        uncle->color = RBTREE_BLACK; // 색 바꿔주고
        z->parent->parent->color = RBTREE_RED; // 여기도 색 바꿔주고 & 여기서는 회전 없음.
        z= z->parent->parent; // ??? 왜 할부지 가리킴? => 할부지 밑으로 업뎃 끝났으니 그 위에서 다시 while문 돌아야 하니!
      }
      else {// uncle color가 블랙
        if (z == z->parent->right) { // 경우 2, 3: uncle color가 black일 때
          z = z->parent;
          left_rotation(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotation(t, z->parent->parent);
      }
    }
    // left => right로 방향 바꿈
    else {
      node_t *uncle = z->parent->parent->left;
      if (uncle->color == RBTREE_RED) { // 경우 1: z->uncle의 색이 red
        z->parent->color = RBTREE_BLACK; // 색 바꿔주고
        uncle->color = RBTREE_BLACK; // 색 바꿔주고
        z->parent->parent->color = RBTREE_RED; // 여기도 색 바꿔주고 & 여기서는 회전 없음.
        z= z->parent->parent; // ??? 왜 할부지 가리킴? => 할부지 밑으로 업뎃 끝났으니 그 위에서 다시 while문 돌아야 하니!
      }
      else {
        if (z == z->parent->left) { // 경우 2, 3: uncle color가 black일 때
          z = z->parent;
          right_rotation(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotation(t, z->parent->parent);
      }
    }
      
  }
  t->root->color = RBTREE_BLACK;

}





node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *p = t;
  while (p!= NULL){
  }
  
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *y = z;
  node_t *x = y;
  color_t *y_original_color = y->color;
  if (z->left == t->nil) {
    x = z->right;
    _transplant(t, z, z->right);
  }

  else if (z->right == t->nil) {
    x = z->left;
    _transplant(t, z, z->left);
  }

  else {
    y = tree_minimum(t, z);
    y_original_color = y->color;
    x = y->right;
    if (y->parent == z) {
      x->parent = y;
    }
    else {
      _transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    _transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if (y_original_color == RBTREE_BLACK) {
    _delete_fixup(t, x);
  }

  return 0;
}

node_t *tree_minimum(rbtree *t, node_t *z) {
  node_t *p = z->right;
  node_t *parent = z;
  int min = z->key+1;
  while (p!= t->nil){
    parent = p;
    if (p->key > min) {
      p = p->left;
    }
    else if (p->key == min) {
      return p;
    }
  }
  return parent;
}

void _transplant(rbtree *t, node_t *u, node_t *v){
  if (u->parent == t->nil) { // u가 루트 노드면
    t->root = v; // t->root 는 v
  }
  else if (u == u->parent->left) { // u가 왼쪽 자식 노드이면
    u->parent->left = v; // v를 왼쪽 자식으로 붙여
  }
  else {
    u->parent->right = v; // v->오른쪽
  }
  v->parent = u->parent; // u를 v 자리로 이동
}

void _delete_fixup(rbtree *t, node_t *x){
  while (x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {
      
    }
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}