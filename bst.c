#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "bst.h" 
#define PARENTHESES 2

bstnode *createNode(void *v, int size);
int getSize(bstnode* node);	
int subDepth(bstnode *p);
char* printNode(bst* b, bstnode* node, char* strTmp);
int find_Leaf(bst* b, bstnode* node, void *v, int i);
void freeNode(bstnode* node);
void rebalance(bst* b, void* arr, int start, int end);

/* bst_init(STRSIZE, mystrcmp, myprintstr) */
bst* bst_init(int sz, int(*comp)(const void* a, const void* b), char*(*prnt)(const void* a)  )
{
   bst* initBst;

   initBst = (bst*)calloc(1, sizeof(bst));
   initBst->top = NULL;
   initBst->elsz = sz;
   initBst->compare = comp;
   initBst->prntnode = prnt;
   return initBst;
}

/* Insert 1 item into the tree */
void bst_insert(bst* b, void* v)
{
   int tmp;
   bstnode* tmpNode = b->top;
   bstnode* parentNode = NULL;

   if(b->top == NULL) {
      b->top = createNode(v, b->elsz);
      return;
   }
   while(tmpNode != NULL) {
      parentNode = tmpNode;
      tmp = b->compare(tmpNode->data, v);
      if(tmp == 0) {
         return;
      }
      else if(tmp > 0) {
         tmpNode = tmpNode->left;
      }
      else if(tmp < 0) {
         tmpNode = tmpNode->right;
      }
   }
   tmpNode = createNode(v, b->elsz);
   if(tmp > 0) {
      parentNode->left = tmpNode;
   }
   else {
      parentNode->right = tmpNode;
   }
}

/* Number of nodes in tree */
int bst_size(bst* b)
{
   if(b->top == NULL) {
      return 0;
   }
   return 1 + getSize(b->top->left) + getSize(b->top->right);
}

/* Longest path from root to any leaf */
int bst_maxdepth(bst* b)
{
   int Maxdepth;
   
   if(b == NULL) {
      return 0;
   }
   else {
      Maxdepth = subDepth(b->top);
      return Maxdepth;
   }
}

/* Whether the data in v, is stored in the tree */
bool bst_isin(bst* b, void* v)
{
   bstnode* tmpNode = b->top;

   while(tmpNode != NULL) {
      if(b->compare(tmpNode->data, v) > 0) {
         tmpNode = tmpNode->left;
      }
      else if (b->compare(tmpNode->data, v) < 0) {
         tmpNode = tmpNode->right;
      }
      else {
         return true;
      }
   }
   return false;
}
    
/* Bulk insert n items from an array v into an initialised tree */
void bst_insertarray(bst* b, void* v, int n)            /* v=words1[][], n=WORDS */
{
   int i = 0;

   for(i=0;i<n;i++) {
      bst_insert(b, (char*)v + (b->elsz)*i);
   }
}     

/* Clear all memory associated with tree, & set pointer to NULL */
void bst_free(bst** p)
{
   bst *a = *p;
   
   if(p == NULL) {
      return;
   }
   freeNode(a->top->left);
   freeNode(a->top->right);
   free(a->top->data);
   a->top->data = NULL;
   free(a->top);
   a->top = NULL;
   free(a);
   *p = NULL;
}

/***************************/
/* Advanced functionality  */
/***************************/

/* Return a string displaying the tree in a textual form (head(left)(right)) recursively */
char* bst_print(bst* b)
{
   char* strTmp = (char*)calloc(bst_size(b), (b->elsz+PARENTHESES)+1);            /*sizeof(char)*/
   char* strTmp2 = (char*)calloc(bst_size(b), (b->elsz+PARENTHESES)+1);
   
   sprintf(strTmp2, "(%s)", printNode(b, b->top, strTmp));
   free(strTmp);
   printf("%s\n", strTmp2);
   return strTmp2;   
}

/* Fill an array with a copy of the sorted tree data */
void bst_getordered(bst* b, void* v)
{
   if(b == NULL) {
      return;
   }
   find_Leaf(b, b->top, v, 0);          /* Making In-order traversal */
}

/* Rebalance the tree, recursively using the median of the sorted keys */
bst* bst_rebalance(bst* b)              /* Using unitializing declaration */
{  
   int start = 0, end = bst_size(b);  
   void *arr = calloc(end, b->elsz);
   bst* newTree = bst_init(sizeof(int), b->compare, b->prntnode);
   
   bst_getordered(b, arr);
   rebalance(newTree, arr, start, end);
   free(arr);
   arr = NULL;
   return newTree;
}

/***************************/
/*   Additional Functions  */
/***************************/

bstnode *createNode(void *v, int size) 
{
   bstnode *node = (bstnode*)calloc(1, sizeof(bstnode));
   node->data = malloc(size);               /* Still need to check using malloc or calloc below */
   memset(node->data, 0, size);
   memcpy(node->data, v, size);
   node->right = NULL;
   node->left = NULL;
   return node; 
}

int getSize(bstnode* node)                  /* node=b->top->left or p=b->top->right */
{
   if(node == NULL) {
      return 0;
   }
   return 1 + getSize(node->left) + getSize(node->right);
}

int subDepth(bstnode *p)
{
   if(p == NULL) {
      return 0;
   }
   else {
      int lDepth = subDepth(p->left);       /* Compute the depth of each subtree */
      int rDepth = subDepth(p->right);
 
      if(lDepth > rDepth) {                 /* Use the larger one */
         return (lDepth + 1);
      }
      else {
         return (rDepth + 1);
      }      
   }
}

char* printNode(bst* b, bstnode* node, char* strTmp)
{
   char* strLeft = (char*)calloc(bst_size(b), (b->elsz+PARENTHESES)+1);
   char* strRight = (char*)calloc(bst_size(b), (b->elsz+PARENTHESES)+1);

   if(node->left != NULL) {
      sprintf(strLeft, "(%s)", printNode(b, node->left, strTmp));
   }
   if(node->right != NULL) {
      sprintf(strRight, "(%s)", printNode(b, node->right, strTmp));
   }
   sprintf(strTmp, "%s%s%s", (char*)node->data, strLeft, strRight);
   free(strLeft);
   free(strRight);
   return strTmp;
}

/* Fill an array with a copy of the sorted tree */
int find_Leaf(bst* b, bstnode* node, void *v, int i)
{
   if(node == NULL) {
      return i;
   }
   if(node->left != NULL) {
      i = find_Leaf(b, node->left, v, i);
   }
   memcpy((char*)v+(b->elsz*i), node->data, b->elsz);
   i++;
   if(node->right != NULL) {
      i = find_Leaf(b, node->right, v, i);
   }
   return i;
}

void freeNode(bstnode* node)
{
   if(node == NULL) {
      return;
   }
   freeNode(node->left);
   freeNode(node->right);
   free(node->data);
   node->data = NULL;
   free(node);
   node = NULL;
}

void rebalance(bst* b, void* arr, int start, int end)
{
   int mid;

   if(start > end) {
      return;
   }
   mid = start + (end - start)/2;
   bst_insert(b, (char*)arr+(b->elsz)*mid);
   rebalance(b, arr, start, mid-1);
   rebalance(b, arr, mid+1, end);
}







