//Binary search tree insertion code written in the class
#include <stdio.h>
#include<stdlib.h>

typedef struct tree_node {
  int data;
  struct tree_node *left;
  struct tree_node *right;
}tree_node;

int c = 0;
tree_node *create_node(int val);
void inorder(tree_node *current_ptr);
tree_node* insert(tree_node *root, tree_node *element);


int main(void) {
   tree_node *my_root=NULL, *temp_node;

   int element;

   printf("Enter a number to insert in the tree: ");
   scanf("%d", &element);

   while(element!=0)
   {
      temp_node = create_node(element); // Create the node.
      // Insert the value.
      my_root = insert(my_root, temp_node);
      
      printf("In-order traversal: ");
      inorder(my_root);

      printf("\nEnter a number to insert in the tree: ");
      scanf("%d", &element);
   }

   
  return 0;
}

//take the root of the tree and insert the node element in the tree. Returns the root of the tree

tree_node* insert(tree_node* root, tree_node* element)
{
  if(root==NULL)
    return element;
  else
  {
    if(element->data > root->data)
    {
      if(root->right != NULL)
        root->right = insert(root->right, element);
      else
        root->right = element;      

    }
    else
    {
      if(root->left != NULL)
        root->left  = insert(root->left, element);
      else
        root->left = element;
    }

    return root;


  }


}

void inorder(tree_node *current_ptr)
{
  if(current_ptr != NULL)
  {
    inorder(current_ptr->left);
    printf("%d ", current_ptr->data);
    inorder(current_ptr->right);
  }
}

//take an integer and return a new node filling up with the integer with left and right pointers as NULL
tree_node* create_node(int val) {

  // Allocate space for the node, set the fields.
  tree_node* temp;
  temp = (tree_node*)malloc(sizeof(tree_node));
  temp->data = val;
  temp->left = NULL;
  temp->right = NULL;

  return temp; // Return a pointer to the created node.
}

int find (struct tree_node *current_ptr, int val)
{
  if (current_ptr == NULL)
  {
    return 0;
  }
  if (current_ptr->data == val) {
    return 1;
  }
  else if (val < current_ptr->data) {
    return find(current_ptr->left, val);
  }
  else {
    return find(current_ptr->right, val);
  }
}

int add(struct tree_node *current_ptr) {
  if (current_ptr==NULL)
  {
    return 0;
  }
  return current_ptr->data + add(current_ptr->left) + add(current_ptr->right);
  
}

int count(struct tree_node *current_ptr) {
  if (current_ptr==NULL)
  {
    return 0;
  }
  return 1 + count(current_ptr->left) + count(current_ptr->right);
}
  
int numLeaves(struct tree_node *p) {
  if (p == NULL)
    return 0;
  if (p->left == NULL && p->right == NULL)
    return 1;
  else
    return numLeaves(p->left) + numLeaves(p->right);
}
void printEven(struct tree_node *current_ptr) { 
  if (current_ptr==NULL)
    return;
  if (current_ptr->data % 2 == 0)
    printf("%d ",current_ptr->data);
  printEven(current_ptr->left);
  printEven(current_ptr->right);
}

void printOdd(struct tree_node *current_ptr) { 
  if (current_ptr==NULL)
    return;
  printOdd(current_ptr->left);
  if (current_ptr->data % 2 != 0)
    printf("%d ",current_ptr->data);
  printOdd(current_ptr->right);
}

int height(struct tree_node* root) {
  if (root==NULL)
    return -1;
  int leftHeight = height(root->left);
  int rightHeight = height(root->right);

  if (leftHeight > rightHeight)
    return leftHeight+1;
  else
    return rightHeight+1;
}
struct node* largest (struct tree_node(*B)) {
  if (B == NULL)
    return NULL;
  if (B->right == NULL)
    return B;
  else
    return largest(B->right);
}

int one (struct tree_node *p){
  if (p==NULL)
  {
    return 0;
  }
  if (p->left != NULL && p->right == NULL)
    return one(p->left) + 1;
  else if (p->left == NULL && p->right != NULL)
    return one(p->right) + 1;
  return one(p->left) + one(p->right); 
  
  
}