#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));    // rbtree 1개 만큼의 동적 메모리를 할당시키고 void 형태로 반환되기 때문에 (rbtree *)로 rbtree 크기만큼의 주소로 바꾼다.
                                                      // 만들어진 포인터 p는 생성된 rbtree 1개의 시작 주소값을 가지게 된다.(rbtree 1개의 시작 주소를 가르키게 된다.)
  // rbtree의 nil에 node_t만큼의 메모리를 동적으로 할당시킨다. 
  p->nil = (node_t *)calloc(1, sizeof(node_t));       // 이 때 nil의 color를 제외하곤 나머지 필드들, 즉 key, parent, left, right은 사용할 일이 없으므로 따로 값을 넣지 않아도 된다.
  p->root = p->nil;                                   // 처음 root의 값은 NULL이다. 하지만 NULL 대신 nil을 쓰기로 하였고, nil을 사용하면 color를 편하게 지정할 수 있으므로 nil을 사용하겠다.(nil->color = RBTREE_BLACK)
  p->nil->color = RBTREE_BLACK;                       // 레드 블랙 트리의 특징에 따라서 nil의 color는 black이다.
  
  // TODO: initialize struct if needed
  return p;
}

// rotation은 β, parent, x <-> y 순서로 주소 교환이 이루어진다.
void left_rotate(rbtree *t, node_t *x) {            // rbtree라는 하나의 구조체를 가르키는 포인터 t, 하나의 노드를 가르키는 포인터 x
  node_t *y = x->right;
  // 1. 힌 단계 올라가는 y의 왼쪽 자식 β를, 한 단계 내려가는 x의 오른쪽 자식으로 옮기기
  x->right = y->left;                                 // 위로 올리고자 하는 y의 왼쪽 자식 혹은 서브 트리 β를, 한 단계 내려가는 x의 오른쪽 자식으로 붙여야 한다.
                                                      // NIL이라면 NIL이 그냥 x의 right로 붙는다.  
                                              
  // 2. 옮기고자 하는 β가 NIL이 아니라면 부모 설정해주기
  if (y->left != t->nil) {                            // NIL이라면 β의 부모를 설정할 필요가 없지만, NIL이 아니라면 부모를 설정해줘야 한다.
    y->left->parent = x;                              // y->left가 β이고, 이 β의 부모를 x로 바꿔주면 된다.
  }
  
  // 3. y의 부모가 원래는 x였다. y의 부모를 x의 부모(parent)로 바꾼다.
  y->parent = x->parent;                
  
  // 4. x의 부모의 왼쪽 혹은 오른쪽 자식에 y가 오도록 한다.
  if (x->parent == t->nil) {                          // x의 부모가 만약 NIL이라면, 즉 없었다면 x 자체가 root 노드였다는 뜻이다. 따라서 rbtree의 root를 y로 바꿔준다.
    t->root = y;
  } else if (x == x->parent->left) {                  // (if문에서 걸러지지 않았다면 parent가 있다는 것) x가 부모의 왼쪽 자식이었다면, 부모의 왼쪽 자식을 y로 바꾼다.
    x->parent->left = y;
  } else {                                            // (x == x->parent->right)를 만족한다면, 즉 x가 부모의 오른쪽 자식이라면, 부모의 오른쪽 자식을 y로 바꾼다.
    x->parent->right = y;
  }
  
  // 5. 한 단계 올라간 y의 왼쪽 자식이 한 단계 내려간 x가 되도록 한다.
  y->left = x;                          
  
  // 6. 5번과 연관지어, 한 단계 내려간 x의 부모가 한 단계 올라간 y가 되도록 한다.
  x->parent = y;
}

// left_rotation을 반대로 바꾸면 된다.
void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;
  
  x->left = y->right;
  
  if (y->right != t->nil) {
    y->right->parent = x;
  }
  
  y->parent = x->parent;
  
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x->parent->right == x) {
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }
  
  y->right = x;
  x->parent = y;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

// 일단 새로 삽입할 노드 z가 삽입되고 나서 fixup이 시작된다.
// 새로운 노드 z 삽입 시 특성 2, root는 black이다. 혹은 특성 4, red가 연속해서 나오면 안된다를 위반할 수 있으니 이를 보완해줘야 한다.
void rbtree_insert_fixup(rbtree *t, node_t *z) {
  // <특성 4를 위반할 경우>
  while (z->parent->color == RBTREE_RED) {            // z의 parent가 red가 아닐 때까지 반복해줘야 특성 4를 벗어날 수 있다.
  
    // a. z의 부모가 조부모의 왼쪽 자식이라면
    if (z->parent == z->parent->parent->left) {       
      node_t *uncle = z->parent->parent->right;       // z의 부모의 부모, 즉 조부모의 오른쪽 자식이 uncle일 것이다.
      
      // 1. 경우 1에 해당하는 것으로, uncle이 red라면 색깔 조정을 하는 것만으로 특성 4 위반을 보완할 수도 있다.
      if (uncle->color == RBTREE_RED) {               
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent = RBTREE_RED;
        z = z->parent->parent;                        // 해당 구역은, 즉 해당 z를 root로 하는 트리는 처리했으니 위로 올라가서 다시 부모를 보고, 정확히 말하자면 while 조건을 따져보고 red라면 또 특성 4를 위반하게 되니 다시 보완해야 한다
        
      // 2. 경우 2, 3에 해당하는 것으로, uncle이 black이라면 z, parent, parent의 parent가 linear인지 혹은 triangle인지 따져보고 회전 및 색깔 조정을 해줘야 한다.
      } else {
        
        // triangle이라면 linear로 만들어줘야 한다.
        if (z == z->parent->right) {                  // 만약 z가 부모의 오른쪽 자식이었다면, 부모가 grand parent의 왼쪽 자식이었으므로 triangle에 해당한다.
          z = z->parent;                              // z의 부모를 축으로 삼아
          left_rotate(t, z);                          // 왼쪽으로 회전해야 한다.
        }
        
        // 바로 위의 if문을 거치고 밑의 코드를 거치게 되면 경우 3에 해당하는 것이다.
        // linear라면 색깔 조정 후 회전하면 된다. 
        // 색깔 조정은 해당 트리의 root가 될 z의 parent를 black으로, 사촌이 될 grand parent를 red로 해주면 된다.
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
      
    // b. z의 부모가 조부모의 오른쪽 자식이라면(z->parent == z->parent->parent->right)
    } else {                                          
      node_t *uncle = z->parent->parent->left;
      
        if (uncle->color == RBTREE_RED) {
          z->parent->color = RBTREE_BLACK;
          uncle->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          z = z->parent->parent;
        } else {
          if (z == z->parent->left) {
            z = z->parent;
            right_rotate(t, z);
          }
          
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          left_rotate(t, z->parent->parent);
        }
    }
  }
  
  // <특성 2를 위반할 경우>
  // while문을 한번도 거치지 않았다는 것은, 새로 삽입할 노드 z의 parent가 red가 아니었음을 말한다.
  // z는 이미 rbtree_insert 함수에서 root 노드가 되어 있을 것이다. 따라서 밑의 구문을 만나 black으로 바뀌게 된다.
  // 이 경우는 red인 z를 그냥 붙이기만 하면 돼서 특성 4를 위반하지 않는다.
  t->root->color = RBTREE_BLACK;
}

// 기존 Binary Search Tree의 Insert에서 변형
// 하지만 삽입할 자리를 찾는 과정은 동일하다. NULL이 nil 노드로 바뀐다는 점과, 새로 삽입할 노드는 항상 단말 노드가 되기 때문에 새로 삽입할 노드 z의 left, right가 NIL이 된다는 점이 다르다.
// 또한 새로 삽입할 노드 z가 레드 블랙 트리의 색깔 특성을 위반시킬 수 있으므로 따로 함수(rbtree_insert_fixup)를 호출하여 보완해줄 필요가 있다.
node_t *rbtree_insert(rbtree *t, const key_t key) {   // rbtree만큼의 크기를 가르키는 포인터 t와, 새로운 삽입할 노드의 키 값을 매개 변수로 받는다.
  node_t *x = t->root;                                // 삽입할 곳을 탐색해 나가는 포인터로, nil(NULL)을 찾으면 x를 삽입한다.
  node_t *y = t->nil;                                 // parent를 저장해두는 포인터이다. 우리는 이미 parent라는 변수를 사용하고 있으므로, 해당 포인터의 변수명을 y로 정했다.
  
  // 중복되는 값도 넣을 수 있어야 한다. 여기서는 오른쪽에 넣는 것으로 동작한다.
  while (x != t->nil) {                               // nil이 아니라면 계속 찾고, nil이라면 root부터 비어있다는 뜻이다. 그리고 while문을 한 번도 돌지 않기 때문에 x는 주소값으로 t->nil을 가지고 있게 된다.
    y = x;                                            // y에는 미리 x의 값(주소값)을 저장해놓는다. parent를 저장해놓기 위해서이다.
    if (x->key > key) {                               // 삽입하려는 값이 현재 x가 가르키는 노드의 키 값보다 작다면 왼쪽으로 탐색한다.
      x = x->left;
    } else {                                          // 삽입하려는 값이 현재 x가 가르키는 노드의 키 값보다 크다면 오른쪽으로 탐색한다.
      x = x->right;                                   // 추가로, 만약 (x->key == key)를 만족하여 중복되는 값이 있다면 else문을 만나 오른쪽에 넣도록 동작하게 된다.
    }
  }
  
  // while 문을 다 돌게 되면 x는 nil을 가르키게 되었다는 뜻으로, parent인 y에 새로운 노드인 z를 삽입하면 된다.
  // 새로운 노드 z 생성(할당)
  node_t *z = (node_t *)calloc(1, sizeof(node_t));    // node_t만큼 메모리에 동적으로 할당시키고 이 공간의 시작 주소를 포인터 z에 대입한다.                       
  z->key = key;
  z->parent = y;                                      // 삽입하려는 시점에는 x가 nil을, y는 nil의 parent를 가르키고 있다.(주소값을 가지고 있다)
  
  if (y == t->nil) {                                  // while문을 1번도 돌지 않았다면 y, 즉 parent는 nil일 것이다.
    t->root = z;                                      // while문을 1번도 돌지 않았다는 것은 애초에 트리가 비어있다는 뜻으로, 새로 생성한 노드 z가 rbtree의 root로 된다.
  } else if (z->key < y->key) {                       // 만약 새로 삽입할 z의 키 값이 parent의 키 값보다 작다면 parent(y)의 왼쪽에 넣어야 한다.
    y->left = z;
  } else {                                            // 만약 새로 삽입할 z의 키 값이 parent의 키 값보다 크다면 parent(y)의 오른쪽에 넣어야 한다.
    y->right = z;
  }
  
  // 새로 삽입할 노드 z는 단말 노드(리프 노드, leaf node)이기 때문에 항상 left, right는 nil이다.
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;                              // 새로 삽입할 노드의 색은 처음에 적색이다.
  
  rbtree_insert_fixup(t, z);
  
  // TODO: implement insert
  return t->root;                                     // 왜 t->root를 반환할까? z가 될 수도 있지 않을까?
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
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

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}


