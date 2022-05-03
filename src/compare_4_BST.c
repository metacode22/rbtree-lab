#include <stdio.h>
#include <stdlib.h>

typedef char data;
typedef struct _Node {
    char key;
    struct _Node* left;                     // Node를 가리키는 포인터
    struct _Node* right;                    // Node를 가리키는 포인터
} Node;

// 모든 과정이 포인터를 통해, 즉 주소값으로 이루어지고 있다.

// <검색>
// 찾았을 때/p.data가 찾고자 하는 x보다 작을 때는 오른쪽으로/p.data가 찾고자 하는 x보다 클 때는 왼쪽으로/p.right 혹은 p.left가 NULL이면 탐색에 실패한 것이므로 끝(실패)
// 이 함수의 반환값은 Node 포인터이다.
Node* searchBST(Node* root, char x) {       // root가 가르킬 것은 Node이므로 Node로 자료형을 선언한다.
    Node* p = root;                         // root는 struct이므로 root의 시작 주소가 포인터 p에 들어간다.
    
    // p가 NULL이 아닌 동안, 즉 실패하기 전까지 계속 검색을 진행한다.    
    while (p != NULL) {
        if (p->key == x) {
            return p;
        } else if (p->key < x) {
            p = p->right;
        } else {
            p = p->left;
        }
    }
     
    return NULL;                            // 찾지 못했을 경우(실패), 즉 p.right 혹은 p.left에서 NULL을 만났다는 뜻으로 NULL을 반환다.
}

// <삽입>
// 탐색이 실패했을 때 삽입이 가능하다. 유일한 키를 가져야 하는 이진 탐색 트리에서는 탐색 성공 시 삽입이 불가능하다.
// 탐색 실패 시, 실패한 위치에 노드를 삽입한다.

// Node* root = insertBST(NULL, 'D');
Node* insertBST(Node* root, char x) {       // root를 가르키는 포인터를 매개 변수로 받고, 그 root로부터의 트리에다가 x를 삽입하겠다는 뜻이다.
    Node* p = root;                         // root가 애초에 비어있으면, p에는 NULL이라는 주소값이 들어가게 된다.
    Node* parent = NULL;                    // 기존 탐색 함수에서 탐색을 실패하면 NULL 값의 주소가 반환된다. 그래서 이 NULL값을 우리가 쓸 수 없다. 따라서 NULL을 만나기 직전의 Node의 주소를 알아야 한다.(NULL을 가진 Node)
                                            // p를 한 발짝 움직일 때마다 움직이기 전의 p 값(주소)을 parent에 저장하면 된다. p가 NULL이 되면 parent의 child에 삽입하고자 하는 데이터를 넣으면 된다.
                                            // root가 비어 있는 경우, 즉 빈 트리에 1번째 Node가 들어갈 경우에는 while 문이 한 바퀴도 돌지 않는다. 따라서 root 자체에다가 새 Node를 넣어줘야 되기 때문에 처음에 NULL로 초기화한다.
                                            
    while (p != NULL) {
        parent = p;                         // p = p->right 혹은 p = p->left를 수행하기 전에 미리 p값을 받아둔다. parent에는 p가 옮겨지기 전의 주소를 얻을 것이다.
        if (p->key == x) {
            printf("같은 키가 있습니다.\n");
            return p;
        } else if (p->key < x) {
            p = p->right;
        } else {
            p = p->left;
        }
    }
    
    // 새 노드 할당, 이 부분부터는 탐색을 실패해서 삽입이 가능한 부분이다.
    // 실전에서는 할당에 실패했을 경우를 넣어야 한다.
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = x;
    newNode->left = newNode->right = NULL;   // 단말 노드가 될 것이기 때문에 왼쪽, 오른쪽 모두 주소값으로 NULL을 넣는다. 이러면 Node 3개의 필드를 다 채우게 된다.
    
    // parent의 자식으로 왼쪽 혹은 오른쪽에 새 노드 붙이기. parent가 NULL인 경우가 있다. 이는 p도 NULL(반환되는 값이 NULL)이고 parent도 NULL이다.
    if (parent != NULL) {
        if (parent->key < newNode->key) {
            parent->right = newNode;
        } else {
            parent->left = newNode;
        }
    }
    
    return newNode;
}

// <삭제> 루트 노드가 삭제되는 경우와 루트 노드가 아닌 노드가 삭제되는 경우로 계속 나눠줘야 한다.
// 여기서 차수란 해당 노드가 지닌 가지(간선)의 갯수이다. 
// 1. 차수 0(단말 노드) : 해당 노드 free 후, 해당 노드를 향하는 parent의 포인터만 NULL 값으로 바꿔주면 된다.
Node* deleteBST(Node* root, char x) {
    Node* p = root;
    Node* parent = NULL;
    while ((p != NULL) && (p->key != x)) {
        parent = p;
        if (p->key < x) {
            p = p->right;
        } else {
            p = p->left;
        }
    }
    
    // 찾는 Node가 없을 시
    if (p == NULL) {
        printf("찾는 노드가 없습니다.\n");
        return root;                                           // 실패했기 때문에 받은 그대로를 반환해준다.
    }
    
    // 차수가 0인 Node(단말 노드)를 삭제할 경우(p->left도 NULL, p->right도 NULL인 경우)
    if (p->left == NULL && p->right == NULL) {
    
        // 현재 Node가 루트 노드인데 차수가 0인 경우, 루트가 아닌데 차수가 0인 경우로 나눠야 한다.
        // 현재 Node가 루트 노드인데 차수가 0인 경우, while문을 돌지 않았기 때문에(찾아서 삭제하고자 했던 Node가 루트였던 경우로, p->key == x가 되어 while문을 처음부터 돌지 않는다) parent는 NULL의 주소값을 그대로 가지고 있다.
        if (parent == NULL) {
            root = NULL;
        } else {
            if (parent->left == p) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
        }
    
    // 차수가 1인 Node를 삭제할 경우
    } else if (p->left == NULL || p->right == NULL) {           // 차수가 0인 경우에서 이미 둘 다 만족하는 경우를 걸렀기 떄문에 여기에는 차수가 1인 경우만 걸리게 된다.
        Node* child = (p->left != NULL) ? p->left : p->right;   // 삭제하고자 하는 노드의 차수가 1일 경우, 그 노드를 삭제하고 나면 그 노드의 부모가 삭제된 노드의 자식을 가르킬 수 있어야 하기 때문에, 삭제되는 노드의 자식을 가리키는 포인터를 우리가 기억하고 있어야 한다.
        
        // 현재 Node(삭제하고자 하는)가 루트 노드인데 차수가 1인 경우, 자식으로 가지고 있는 노드 그 1개가 새로운 루트가 된다.
        if (parent == NULL) {
            root = child;
        } else {
            if (parent->left == p) {
                parent->left = child;
            } else {
                parent->right = child;
            }
        }
    
    // 차수가 2인 Node를 삭제할 경우, 실제로 해당 Node를 삭제하진 않는다. 후계자를 찾아서 그 데이터를 차수가 2인 Node의 데이터에 덮어씌우고 후계자를 없앤다. 이 후계자는 단말 노드 혹은 차수가 1인 노드이다. 
    // 차수가 2인 Node를 삭제할 경우, 이미 위에서 0과 1인 경우가 걸러졌기 때문에 밑의 구문에는 차수가 2인 경우만 걸리게 된다.
    // 양쪽 자식이 다 있기 때문에, 왼쪽 혹은 오른쪽 서브트리에서 후계자를 골라, 삭제하고자 하는 Node로 올리게 된다.
    // 왼쪽에서 고를 때는 가장 큰 값을, 오른쪽에서 고를 때는 가장 작은 작은 값을 골라서 위로 올리게 된다. 이 때 고른 노드는 0 차수 혹은 1 차수이다.
    } else {
        // 왼쪽 혹은 오른쪽 아무곳에서나 고르면 된다. 여기서는 오른쪽에서 고른다.
        // 후계자, successor(succ). 여기서 잊으면 안되는 점은, 우리는 위의 while문에서 이미 삭제하고자 하는 Node를 찾았다. 밑의 과정은 후계를 하고자 하는 Node를 찾는 과정이다.
        Node* succ_parent = p;
        Node* succ = p->right;                                  // 오른쪽에서 찾을 것이므로 p->right로 간다.
        
        // 오른쪽 서브트리에서 가장 작은 값을 찾는다.
        // 왼쪽 자식들을 계속 찾아가면 된다. 그러다가 NULL을 만나면 왼쪽 끝단이라는 뜻이다. 따라서 가장 작은 값이다.
        while (succ->left != NULL) {
            succ_parent = succ;
            succ = succ->left;
        }
        
        // 데이터만 옮겨준다.
        p->key = succ->key;
        if (succ_parent->left == succ) {
            succ_parent->left = succ->right;                    // succ의 왼쪽 자식은 없는 것이 확실하므로(가장 작은 값이라서) 오른쪽 자식을 주면 된다.
        } else {
            succ_parent->right = succ->right;                   // succ의 왼쪽 자식은 없는 것이 확실하므로(가장 작은 값이라서) 오른쪽 자식을 주면 된다.
        }
        p = succ;                                               // 실제로 삭제할 것은 succ이므로, 밑에서 free(p)를 위해 p에 succ(주소값)를 대입해준다.
    }
    
    free(p);                                                    // 해당 Node를 free시킴.
    return root;                                                // 바뀌었을 수도 있는 root를 반환한다.
}

// 중위 순회, inorder
void inorder(Node* root) {
    if (root == NULL) {
        return;
    }
    
    inorder(root->left);
    printf("%c", root->key);
    inorder(root->right);
}

int main() {
    Node* root = insertBST(NULL, 'D');
    insertBST(root, 'I');
    insertBST(root, 'F');
    insertBST(root, 'A');
    insertBST(root, 'G');
    insertBST(root, 'C');
    
    inorder(root);
    printf("\n");
    
    root = deleteBST(root, 'C');                                 // root가 바뀌었을 수도 있기 때문에
    
    inorder(root);
    
    return 0;
}