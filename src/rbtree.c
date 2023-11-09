#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  // nil 노드 초기화
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  p -> nil = p -> root = nil;
  nil -> color = RBTREE_BLACK;

  return p;
}

void left_rotate(rbtree *t, node_t *node) {
  node_t *y = node -> right;
  node -> right = y -> left;

  if (y -> left != t -> nil) {
    y -> left -> parent = node;
  }
  y -> parent = node -> parent;

  if (node -> parent == t -> nil) {
    t -> root = y;
  }

  else if ( node == node -> parent -> left) {
    node -> parent -> left = y;
  }

  else {
    node -> parent -> right = y;
  }

  y -> left = node;
  node -> parent = y;

}

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

node_t *rbtree_insert_fixup(rbtree *t, node_t *node) {
  node_t *uncle;
  while ( node -> parent -> color == RBTREE_RED ) {
    if (node -> parent == node -> parent -> parent -> left) {
      uncle = node -> parent -> parent -> right;

      if ( uncle -> color == RBTREE_RED ) {
        node -> parent -> color = RBTREE_BLACK;
        uncle -> color = RBTREE_BLACK;
        node -> parent -> parent -> color = RBTREE_RED;
        node = node -> parent -> parent;
      }

      else {
        if ( node == node -> parent -> right) {
          node = node -> parent;
          left_rotate(t, node);
        }

        node -> parent -> color = RBTREE_BLACK;
        node -> parent -> parent -> color = RBTREE_RED;
        right_rotate(t, node -> parent -> parent);
      }
    }

    else {
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

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *node = (node_t *)calloc(1, sizeof(node_t));

  node_t *y = t -> nil;
  node_t *x = t -> root;
  node -> key = key;
  node -> left = node -> right = t -> nil;
  
  while ( x != t -> nil ) {
    y = x;
    if ( node -> key < x -> key ) {
      x = x -> left;
    }
    
    else {
      x = x -> right;
    }
  }

  node -> parent = y;

  if ( y == t -> nil ) {
    t -> root = node;
    node -> color = RBTREE_BLACK;
  }

  else if ( node -> key < y -> key ) {
    y -> left = node;
    node -> color = RBTREE_RED;
  }

  else {
    y -> right = node;
    node -> color = RBTREE_RED;
  }
  
  rbtree_insert_fixup(t, node);

  return t -> root;
}

void delete_recursive(rbtree *t, node_t *node) {
    if (node -> left != t -> nil ) {
      delete_recursive(t, node -> left);
    }
  
    if (node -> right != t -> nil ) {
      delete_recursive(t, node -> right);
    }
    free(node);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t -> root != t -> nil) {
    delete_recursive(t, t -> root);
  }

  free(t -> nil);
  free(t);
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *f = t -> root;
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

  return NULL;
}


node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *minT = t -> root;
  if (minT == t -> nil) {
    return minT;
  }
  
  while ( minT -> left != t -> nil ) {
      minT = minT -> left;
  }
  return minT;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *maxT = t -> root;
 
  if (maxT == t -> nil) {
    return maxT;
  }
 
  while ( maxT -> right != t -> nil ) {
    maxT = maxT -> right;
  }

  return maxT;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
  if ( u -> parent == t -> nil ) {
    t -> root = v;
  }
  
  else if ( u == u -> parent -> left ) {
    u -> parent -> left = v;
  }
  
  else {
    u -> parent -> right = v;
  }
  
  v -> parent = u -> parent;
}

void rbtree_erase_fixup(rbtree *t, node_t *x) {
  node_t *node;
  while ( (x != t -> root) && (x -> color == RBTREE_BLACK) ) {
    if ( x == x -> parent -> left ) {
      node = x -> parent -> right;

      if ( node -> color == RBTREE_RED ) {
        node -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        left_rotate(t, x -> parent);
        node = x -> parent -> right;
      }

      if ( node -> left -> color == RBTREE_BLACK && node -> right -> color == RBTREE_BLACK ) {
        node -> color = RBTREE_RED;
        x = x -> parent;
      }

      else {
        if ( node -> right -> color == RBTREE_BLACK ) {
          node -> left -> color = RBTREE_BLACK;
          node -> color = RBTREE_RED;
          right_rotate(t, node);
          node = x -> parent -> right;
        }

        node -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        node -> right -> color = RBTREE_BLACK;
        left_rotate(t, x -> parent);
        x = t -> root;
        }
    }
    
    else {
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

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *x;
  node_t *y = p;
  color_t y_original_color = y -> color;

  if ( p -> left == t -> nil ) {
    x = p -> right;
    rbtree_transplant(t, p, p -> right);
  }

  else if ( p -> right == t -> nil) {
    x = p -> left;
    rbtree_transplant(t, p, p -> left);
  }

  else {
    y = p -> right;

    while ( y -> left != t -> nil ) {
      y = y -> left;
    }

    y_original_color = y -> color;
    x = y -> right;

    if ( y -> parent == p ) {
      x -> parent = y;
    }
    
    else {
      rbtree_transplant(t, y, y -> right);
      y -> right = p -> right;
      y -> right -> parent = y;
    }

    rbtree_transplant(t, p, y);
    y -> left = p -> left;  
    y -> left -> parent = y;
    y -> color = p -> color;
  }

  if ( y_original_color == RBTREE_BLACK ) {
    rbtree_erase_fixup(t, x);
  }

  free(p);

  return 0;
}

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

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if ( t -> root == t -> nil ) {
    return -1;
  }

  rbtree_inorder(t -> root, arr, t, 0, n);
  return 0;
}