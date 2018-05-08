#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include <stdbool.h>

#include "bst.h" 
#define STRSIZE 20
/* What to do on error */
#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)
#define CSEC (double)(CLOCKS_PER_SEC)

int mystrcmp(const void* a, const void* b);
int mychrcmp(const void* a, const void* b);
int myintcmp(const void* a, const void* b);
char* myprintstr(const void* v);
char* myprintchr(const void* v);
char* myprintint(const void* v);

int main(int argc, char *argv[])
{
   FILE *fp1;
   FILE *fp2;
   char *filename1, *filename2;
   char *str;
   bst *dictionary, *misspelt;
   clock_t start, end;

   start = clock();
   printf("Beginning SPL Test ...\n\n");

   dictionary = bst_init(STRSIZE, mystrcmp, myprintstr);
   misspelt = bst_init(STRSIZE, mystrcmp, myprintstr);
   str = (char*)calloc(dictionary->elsz, sizeof(char));

   filename1 = "";
   filename2 = "";
   if(argc == 3) {
      filename1 = argv[1];
      filename2 = argv[2];
      printf("%s\n", filename1);
      printf("%s\n\n", filename2);
   }

   if((fp1 = fopen(filename1, "r")) == NULL) {
      ON_ERROR("Cannot open file 1\n");
   }
   if((fp2 = fopen(filename2, "r")) == NULL) {
      ON_ERROR("Cannot open file 2\n");
   }

   while(!feof(fp1)) {
      if(fgets(str, STRSIZE, fp1) != NULL) {
         bst_insert(dictionary, str);
      }
   }
   while(!feof(fp2)) {
      if(fgets(str, STRSIZE, fp2) != NULL) {
         if(bst_isin(dictionary, str) == false && bst_isin(misspelt, str) == false) {
            printf("%s", misspelt->prntnode((str)));
            bst_insert(misspelt, str);
         }
      }
   }
   bst_free(&dictionary);
   assert(dictionary == NULL);
   bst_free(&misspelt);
   assert(misspelt == NULL);
   free(str);
   str = NULL;

   fclose(fp1);
   fclose(fp2);
   printf("\nFinished SPL Test ...\n");
   end = clock();
   printf("Time(s): %f seconds\n\n", (double)(end-start)/CSEC);

   return  0;
}

char* myprintstr(const void* v)
{
   return (char*)v;
}

char* myprintchr(const void* v)
{
   static char str[100];
   sprintf(str, "%c", *(char*)v);
   return str;
}

char* myprintint(const void* v)
{
   static char str[100];
   sprintf(str, "%d", *(int*)v);
   return str;
}

int mystrcmp(const void* a, const void* b)
{
   return strcmp(a, b);
}

int mychrcmp(const void* a, const void* b)
{
   return *(char*)a - *(char*)b;
}

int myintcmp(const void* a, const void* b)
{
   return *(int*)a - *(int*)b;
}

