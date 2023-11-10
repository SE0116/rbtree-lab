#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  
  // nil 노드 초기화
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  p -> nil = p -> root = nil;

  nil -> color = RBTREE_BLACK;  // 루트 노드는 검은색이어야 한다

  return p;
}

void left_rotate(rbtree *t, node_t *node) { // node : 오른쪽 자식이 nil이 아닌 모든 노드
  node_t *y = node -> right;
  node -> right = y -> left;  // y의 왼쪽 자식을 node의 오른쪽 자식으로

  if (y -> left != t -> nil) {  // y의 왼쪽 자식이 nil이 아닐 때 node를 [y의 왼쪽 자식]의 부모로
    y -> left -> parent = node;
  }
  y -> parent = node -> parent; // node의 부모를 y의 부모로

  if (node -> parent == t -> nil) { // 1. node가 루트노드면 y를 루트노드로 ( ∵ 루트노드는 부모노드가 없음 )
    t -> root = y;
  }

  else if ( node == node -> parent -> left) { // 2. node가 부모의 왼쪽 자식이면 y를 node의 자리로
    node -> parent -> left = y;
  }

  else {  // 3. node가 부모의 오른쪽 자식이면 y를 node의 자리로
    node -> parent -> right = y;
  }

  y -> left = node; // node를 y의 왼쪽 자식으로
  node -> parent = y;

}

// left_rotate에서 방향만 반대인 함수
void right_rotate(rbtree *t, node_t *node) {
  node_t *y = node -> left;
  node -> left = y -> right;

  if (y -> right != t -> nil) {
    y -> right -> parent = node;
  }

  y -> parent = node -> parent;

  if (node -> parent == t -> nil) {
    t -> root = y;
  }

  else if ( node == node -> parent -> right) {
    node -> parent -> right = y;
  }

  else {
    node -> parent -> left = y;
  }

  y -> right = node;
  node -> parent = y;

}

// insert 함수에서 키값을 추가하고 RB트리에 위반되는 항목 조정
node_t *rbtree_insert_fixup(rbtree *t, node_t *node) {
  node_t *uncle;  // 노드의 색을 비교할 삼촌 노드
  while ( node -> parent -> color == RBTREE_RED ) { // 부모의 노드 색이 빨강일 때
    if (node -> parent == node -> parent -> parent -> left) { // 부모가 왼쪽 자식노드면
      uncle = node -> parent -> parent -> right;  // 삼촌은 오른쪽 자식노드

      if ( uncle -> color == RBTREE_RED ) { // 삼촌의 노드 색이 빨강일 때
        node -> parent -> color = RBTREE_BLACK; //  부모의 노드 색을 검정
        uncle -> color = RBTREE_BLACK;  // 삼촌의 노드 색도 검정
        node -> parent -> parent -> color = RBTREE_RED; // 조부모 노드 색을 빨강
        node = node -> parent -> parent;  // 자식을 조부모 노드로
      }

      else {  // 부모의 노드 색이 검정일 때
        if ( node == node -> parent -> right) {  // 오른쪽 자식이면
          node = node -> parent;  // 자식과 부모 노드 위치를 바꾸고 왼쪽으로 회전
          left_rotate(t, node);
        }
        // left_rotate로 인해 node는 왼쪽 자식이 됨
        node -> parent -> color = RBTREE_BLACK; // 부모는 검정으로
        node -> parent -> parent -> color = RBTREE_RED; // 조부모는 빨강으로
        right_rotate(t, node -> parent -> parent);  // 회전은 오른쪽으로
      }
    }

    else {  // 부모가 오른쪽 자식노드면 위 if문의 반대로 진행
      uncle = node -> parent -> parent -> left;
      if  ( uncle -> color == RBTREE_RED ) {
        node -> parent -> color = RBTREE_BLACK;
        uncle -> color = RBTREE_BLACK;
        node -> parent -> parent -> color = RBTREE_RED;
        node = node -> parent -> parent;
      }

      else {
        if ( node == node -> parent -> left ) {
          node = node -> parent;
          right_rotate(t, node);
        }

        node -> parent -> color = RBTREE_BLACK;
        node -> parent -> parent -> color = RBTREE_RED;
        left_rotate(t, node -> parent -> parent);
      }
    }
  }
  t -> root -> color = RBTREE_BLACK;

  return t -> root;
}

// 매개변수로 받은 키값을 노드에 추가
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *node = (node_t *)calloc(1, sizeof(node_t)); // 추가할 노드 생성

  node_t *y = t -> nil;
  node_t *x = t -> root;
  node -> key = key;  // 인자로 받은 키값 저장
  node -> left = node -> right = t -> nil;
  
  while ( x != t -> nil ) { // 루트부터 시작해서 nil이 나올 때 까지 탐색 ( log n )
    y = x;
    if ( node -> key < x -> key ) {
      x = x -> left;
    }
    
    else {
      x = x -> right;
    }
  }

  node -> parent = y; // 반복문을 돌리면서 부모가 될 값을 y에 저장했기 때문에 새로 추가되는 값의 부모를 y로

  if ( y == t -> nil ) {  // y가 nil이면 부모노드가 nil인 경우이기 때문에 node는 루트 노드
    t -> root = node;
  }

  else if ( node -> key < y -> key ) {  // 왼쪽 자식은 부모 노드보다 작고
    y -> left = node;
  }

  else {  // 오른쪽 자식은 부모 노드보다 큼
    y -> right = node;
  }
  node -> color = RBTREE_RED; // 새로 추가하는 노드의 색은 RED로 고정
  
  rbtree_insert_fixup(t, node); // RB트리 규칙을 위반하는 항목이 있는지 확인

  return t -> root;
}

// 재귀를 사용해 트리의 리프까지 탐색 후 반환
void delete_recursive(rbtree *t, node_t *node) {
    if (node -> left != t -> nil ) {
      delete_recursive(t, node -> left);
    }
  
    if (node -> right != t -> nil ) {
      delete_recursive(t, node -> right);
    }
    free(node);
}

// 트리가 사용한 메모리 전체 반환
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t -> root != t -> nil) {
    delete_recursive(t, t -> root);
  }

  free(t -> nil);
  free(t);
}

// 트리에 원하는 키값을 가진 노드가 있는지 탐색 ( 이진 탐색 )
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *f = t -> root;  // 루트 노드를 탐색 시작 노드로
  while (f != t -> nil) {
    if (f -> key < key) {
      f = f -> right;
    }

    else if (f -> key > key) {
      f = f -> left;
    }

    else {
      return f;
    }
  }

  return NULL;  // 찾는 키값이 트리에 없으면 NULL 반환
}


// 트리의 가장 왼쪽에 있는 노드까지 탐색
node_t *rbtree_min(const rbtree *t) {
  node_t *minT = t -> root;
  if (minT == t -> nil) {
    return minT;
  }
  
  while ( minT -> left != t -> nil ) {
      minT = minT -> left;
  }
  return minT;
}

// 트리의 가장 오른쪽에 있는 노드까지 탐색
node_t *rbtree_max(const rbtree *t) {
  node_t *maxT = t -> root;
 
  if (maxT == t -> nil) {
    return maxT;
  }
 
  while ( maxT -> right != t -> nil ) {
    maxT = maxT -> right;
  }

  return maxT;
}

// 서브트리 u를 서브트리 v로 교체
void rbtree_transplant(rbtree *t, node_t *u, node_t *v) { 
  if ( u -> parent == t -> nil ) {  // u가 루트 노드일 때
    t -> root = v;
  }
  
  else if ( u == u -> parent -> left ) {  // u가 왼쪽 자식일 때
    u -> parent -> left = v;
  }
  
  else {  // u가 오른쪽 자식일 때
    u -> parent -> right = v;
  }
  
  v -> parent = u -> parent;  // 원래 u의 부모를 v의 부모로
}

// erase에서 발생하는 RB트리 규칙 위반 조정
void rbtree_erase_fixup(rbtree *t, node_t *x) {
  node_t *node; // 노드의 색을 비교하기 위한 형제 노드
  while ( (x != t -> root) && (x -> color == RBTREE_BLACK) ) {
    if ( x == x -> parent -> left ) { // x가 왼쪽 자식일 때
      node = x -> parent -> right;

      if ( node -> color == RBTREE_RED ) {  // 1. 형제 노드의 색이 빨강이면
        node -> color = RBTREE_BLACK; // 형제 노드와 부모 노드의 색을 바꾸고
        x -> parent -> color = RBTREE_RED;
        left_rotate(t, x -> parent);  // 왼쪽 회전
        node = x -> parent -> right;
      }

      if ( node -> left -> color == RBTREE_BLACK && node -> right -> color == RBTREE_BLACK ) {  // 2. 형제 노드와 그 자식이 둘 다 검정이면
        node -> color = RBTREE_RED; // 형제 노드를 빨강으로
        x = x -> parent;
      }

      else {  // 3. 형제 노드는 검정이고
        if ( node -> right -> color == RBTREE_BLACK ) { // 형제의 오른쪽 자식이 검정이면
          node -> left -> color = RBTREE_BLACK; // 형제 노드와 그 왼쪽 자식의 색 변경
          node -> color = RBTREE_RED;
          right_rotate(t, node);  // 하고 오른쪽 회전
          node = x -> parent -> right;
        }

        node -> color = x -> parent -> color; // 4. 형제의 오른쪽 자식이 빨강이면
        x -> parent -> color = RBTREE_BLACK;  // 부모와 형제의 색 변경
        node -> right -> color = RBTREE_BLACK;  // 바꾼김에 오른쪽 자식도 같이
        left_rotate(t, x -> parent);  // 하고 왼쪽 회전
        x = t -> root;
        }
    }
    
    else {  // x가 오른쪽 자식일 때 위와 반대 방향으로 진행
      node = x -> parent -> left;
      
      if ( node -> color == RBTREE_RED ) {
        node -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        right_rotate(t, x -> parent);
        node = x -> parent -> left;
      }

      if ( node -> right -> color == RBTREE_BLACK && node -> left -> color == RBTREE_BLACK ) {
        node -> color = RBTREE_RED;
        x = x -> parent;
      }
      else {
        if ( node -> left -> color == RBTREE_BLACK ) {
            node -> right -> color = RBTREE_BLACK;
            node -> color = RBTREE_RED;
            left_rotate(t, node);
            node = x -> parent -> left;
        }

        node -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        node -> left -> color = RBTREE_BLACK;
        right_rotate(t, x -> parent);
        x = t -> root;
      }
    }
  }
  x -> color = RBTREE_BLACK;
}

// 포인터로 지정된 노드 삭제
int rbtree_erase(rbtree *t, node_t *p) {
  node_t *x;  // y의 원래 위치로 갈 노드
  node_t *y = p;  // y : 삭제하려는 노드
  color_t y_original_color = y -> color;  // 삭제될 노드의 색

  if ( p -> left == t -> nil ) {  // 1. p가 왼쪽 자식이 없을 때
    x = p -> right; // p의 오른쪽 자식을 p자리로 올리기
    rbtree_transplant(t, p, p -> right);
  }

  else if ( p -> right == t -> nil) { // 2. p가 오른쪽 자식이 없을 때
    x = p -> left;  // p의 왼쪽 자식을 p자리로 올리기
    rbtree_transplant(t, p, p -> left);
  }

  else {  // 3. p의 자식이 두개일 때
    y = p -> right; // y를 p의 오른쪽 자식으로 두고

    while ( y -> left != t -> nil ) { // y가 루트인 서브트리에서의 최솟값으로 변경 ( 흔히 말하는 successor )
      y = y -> left;
    }

    y_original_color = y -> color;  // 바뀐 y의 색으로 변경
    x = y -> right; // x를 y의 오른쪽 자식으로

    if ( y -> parent == p ) { // 삭제하려는 p가 y의 부모 노드면
      x -> parent = y;  // x가 nil인 경우에 x의 부모를 호출하면 Segmentation Fault 가 생기기 때문에 x의 부모를 y로 설정)
    }
  
    else {  // 부모 노드가 아니면 y의 부모 노드와 y의 오른쪽 자식을 연결
      rbtree_transplant(t, y, y -> right);  // y의 자리에 y의 오른쪽 자식 놓기
      y -> right = p -> right;
      y -> right -> parent = y;
    }

    rbtree_transplant(t, p, y); // 삭제하려는 노드의 자리에 y 놓기 ( successor )
    y -> left = p -> left;
    y -> left -> parent = y;
    y -> color = p -> color;
  }

  if ( y_original_color == RBTREE_BLACK ) { // 삭제되는 노드의 색이 검정이면 fixup 함수로 조정
    rbtree_erase_fixup(t, x);
  }

  free(p);  // 삭제한 메모리에 대한 할당 해제

  return 0;
}

// 키값을 오름차순으로 확인하기 위해 중위순회로 진행
int rbtree_inorder(node_t *current, key_t *res, const rbtree *t, int i, const size_t n) {
  if ( current == t -> nil || i >= n) { 
    return i;
  }
  
  i = rbtree_inorder(current -> left, res, t, i, n);

  res[i] = current -> key;
  i += 1;

  i = rbtree_inorder(current -> right, res, t, i, n);

  return i;
}

// RB tree의 내용을 중위 순회로 정리한 값을 배열에 저장
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if ( t -> root == t -> nil ) {
    return -1;
  }

  rbtree_inorder(t -> root, arr, t, 0, n);
  return 0;
}