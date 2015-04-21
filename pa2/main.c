/*
 * sorted-list.c
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sorted-list.h"

int compareInts(void *p1, void *p2)
{
	int i1 = *(int*)p1;
	int i2 = *(int*)p2;

	return i1 - i2;
}

int compareDoubles(void *p1, void *p2)
{
	double d1 = *(double*)p1;
	double d2 = *(double*)p2;

	return (d1 < d2) ? -1 : ((d1 > d2) ? 1 : 0);
}

int compareStrings(void *p1, void *p2)
{
	char *s1 = p1;
	char *s2 = p2;

	return strcmp(s1, s2);
}

//Destructor functions
void destroyBasicTypeAlloc(void *p){
	//For pointers to basic data types (int*,char*,double*,...)
	//Use for allocated memory (malloc,calloc,etc.)
	free(p);
}

void destroyBasicTypeNoAlloc(void *p) {
	//For pointers to basic data types (int*,char*,double*,...)
	//Use for memory that has not been allocated (e.g., "int x = 5;SLInsert(mylist,&x);SLRemove(mylist,&x);")
	return;
}

int main()
{
    SortedListPtr intList = SLCreate(compareInts, destroyBasicTypeNoAlloc);
    SortedListPtr lastList = SLCreate(compareInts, destroyBasicTypeNoAlloc);
    SortedListPtr stringList = SLCreate(compareStrings,destroyBasicTypeNoAlloc);


    //Int List
    int a = 13;
    int b = 4;
    int c = 24;
    int d = 64;
    int f = 122;
    int g = 53;

    //Int Values for the final list
    int h = 21;
    int i = 32;
    int j = 43;
    int k = 54;
    int l = 65;
    int m = 76;
    int n = 87;
    int o = 98;
    int p = 102;
    int q = 113;
    int r = 124;
    int s = 136;
 
    //String values that can be in the list
    char *string1 = "panda";
    char *string2 = "tiger";
    char *string3 = "sloth";
    char *string4 = "dog";
    char *string5 = "hamster";
    char *string6 = "kangaroo";


    //Insert ints into the list
    SLInsert(intList, &a);
    SLInsert(intList, &b);
    SLInsert(intList, &c);
    SLInsert(intList, &d);
    SLInsert(intList, &g);
    SLInsert(intList, &f);

    //Insert ints into the final list
    SLInsert(lastList, &g);
    SLInsert(lastList, &h);
    SLInsert(lastList, &i);
    SLInsert(lastList, &j);
    SLInsert(lastList, &k);
    SLInsert(lastList, &l);
    SLInsert(lastList, &m);
    SLInsert(lastList, &n);
    SLInsert(lastList, &o);
    SLInsert(lastList, &p);
    SLInsert(lastList, &q);
    SLInsert(lastList, &r);
    SLInsert(lastList, &s);
   
    //Test cases to test insert and remove string functions
    SLInsert(stringList, string1);
    SLRemove(stringList, string4);
    SLInsert(stringList, string1);
    SLInsert(stringList, string1);
    SLInsert(stringList, string2);
    SLInsert(stringList, string3);
    SLRemove(stringList, string3);
    SLInsert(stringList, string4);
    SLRemove(stringList, string4);
    SLInsert(stringList, string5);
    SLInsert(stringList, string6);
    SLRemove(stringList, string2);
    SLInsert(stringList, string4);
    SLRemove(stringList, string2);
    SLInsert(stringList, string2);
    SLRemove(stringList, string1);
    

    //Initialize the iterators for the string, int and final lists
    SortedListIteratorPtr SLString = SLCreateIterator(stringList);
    SortedListIteratorPtr SlInt = SLCreateIterator(intList);
    SortedListIteratorPtr SlFinal = SLCreateIterator(lastList);

    //Print the Int list
    void *item = SLGetItem(SlInt);
    while(1){
        if (item == NULL){
            break;
        }
        else
         printf("%d\n", *((int*)item));

	item = SLNextItem(SlInt);
    }
    printf("\n");
	//Print the String list
    void *item2 = SLGetItem(SLString);
    while(1){
        if (item2 == NULL){
            break;
        }
        else
         printf("%s\n", (char*)item2);

	item2 = SLNextItem(SLString);
    }

    //Destroy all of the iterators, free memory
    SLDestroyIterator(SlFinal);
    SLDestroyIterator(SlInt);
    SLDestroyIterator(SLString);

    //Destory all of the lists, free memory
    SLDestroy(stringList);
    SLDestroy(intList);
    SLDestroy(lastList);

	return 0;
}

