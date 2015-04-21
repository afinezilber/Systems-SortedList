#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sorted-list.h"

/*Creates a Linked List Node
 *
 */

Node *nodeMake(void *val)
{
    Node *listNode = malloc(sizeof(Node));	//Allocate memory for the node
    listNode->data = val;
    listNode->moveRef = 0;	//Hold the count for removed node
    listNode->counter = 0;	//Hold the reference counter 
    listNode->next = NULL;
    return listNode;
}

/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list, and a destruct function that gets rid of the objects
 * once they are no longer in the list or referred to in an iterator.
 *
 * If the function succeeds, it returns a (non-NULL) SortedListT object,
 * otherwise, it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df)
{
    SortedListPtr sl = malloc(sizeof(struct SortedList));
    sl->cf = cf;
    sl->df = df;
    sl->head = NULL;		//Create function allows for a compare and destroy funcs to be accessed
    return sl;
}

/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 *
 * You need to fill in this function as part of your implementation.
 */

void SLDestroy(SortedListPtr list)
{
    if(!list){
        return;
    }
	//As long as the list is still exsistant, iterate through destroying all node data.
    Node *ptr = NULL;	//Ptr to the node now points to null
    while(list->head != NULL){
        list->df(list->head->data);			//Use the destroy function
        ptr = list->head;
        list->head = list->head->next;		//The new head of the list is the next node
        free(ptr);						//free the memory that was allocated for the node ptr
    }
    free(list);				//Free the memory that was allocated for the list
}

int SLInsert(SortedListPtr list, void *newObj)
{
	//If there is no list or no new data to insert into the node, return successfully
    if(!list || !newObj){       
        return 0;               
    }

    Node *newNode = nodeMake(newObj);	//Create a node object to store the data to be inserted
	
    if(list->head == NULL || list->cf(list->head->data, newObj) < 0){
        newNode->next = list->head;     //If the data entered is greater than the data held at the head node
        list->head = newNode;           //Make the new node the head node and make the head the new node
        return 1;
    }

    Node *ptr = list->head;		//If the new node is not greater, iterate through the list to determine where to enter the new data
    Node *prev= NULL;		
	
    while(ptr != NULL){			//While the list is the not at the last node
        if(list->cf(ptr->data, newObj) == 0){   //if the data is a duplicate, do not add the node to the list
            return 0;
        }
        else if(list->cf(ptr->data, newObj) < 0){ //if newObj is bigger, insert
            Node *newNode = nodeMake(newObj);
            if(prev == NULL){       //if the data is bigger than that of the data held at the head node
                Node *temp = ptr;       //Make the new data the head node and point the old head to the head->next node
                list->head = newNode;   
                newNode->next = temp;   
            }
            prev->next = newNode;
            newNode->next = ptr;
            newNode->counter++;
            return 1;
        }
        else if(list->cf(ptr->data, newObj) > 0){ //As long as the data is smaller, we must continue to traverse through the list
            prev = ptr;
            ptr = ptr->next;
        }
    }
    prev->next = newNode;
    return 1;
}

int SLRemove(SortedListPtr list, void *newObj)
{
												//If there is no data in the list, or no data at the head node, 
    if(!list || list->head == NULL || !newObj){	//or no data in the new object, nothing to remove.
        return 0;
    }		

    Node *ptr = list->head;
    Node *prev = NULL;
    while(ptr != NULL){
        if(list->cf(ptr->data, newObj) == 0){ //If the Object we are looking to remove is the head node
            if(prev == NULL){   				//Remove the head node and have the node that comes after take its place
                list->head = list->head->next;
                if(list->head){
                    list->head->counter++;     //increment counter so the SortedListPtr is pointing to the new head
                }
                ptr->counter--;    
                ptr->moveRef = 1;
                if(ptr->counter <= 0){     
                    list->df(ptr->data);	//If no object is found at the current node, destroy it.
                    if(ptr->next !=NULL){   
                        ptr->next->counter--;  //If there is a ptr to the next node, decrement the reference counter
                    }
                    free(ptr);				//Free the memory at the ptr if there was no data in the node
                    return 1;
                }
            }
            else{
                prev->next = ptr->next;     //If we do not delete the head node, we must find the node we need to delete
                if(ptr->next != NULL){      //If we are at the current node and it is not what we are looking for,
                    ptr->next->counter++;	//and the node has a next, we increment reference counter and go to the next node
                    ptr->counter--;
                    ptr->moveRef = 1;
                }
                if(ptr->counter <=0){
                    list->df(ptr->data);
                    if(ptr->next != NULL){
                        ptr->next->counter--;
                    }
                    free(ptr);
                    return 1;
                }
            }
        }
        prev = ptr;
        ptr = ptr->next;
    }
    return 0;
}

SortedListIteratorPtr SLCreateIterator(SortedListPtr list)
{
    SortedListIteratorPtr sortIter = malloc(sizeof(struct SortedListIterator)); //allocate memory for the iterator ptr
    sortIter->current = list->head;			//Start by making the iterator point to the head node
    if(sortIter->current != NULL){			
        sortIter->current->counter++;	//If the data at the head node is not NULL, go the next node
    }
    return sortIter;

}

void SLDestroyIterator(SortedListIteratorPtr iter)
{
    
    if(iter->current != NULL){
        iter->current->counter--;	//The current count gets decremented, then we free the iterator
    }
    free(iter);

}
void *SLGetItem(SortedListIteratorPtr iter)
{
	if(iter->current->next == NULL)
		return 0;
	else
		return iter->current->data;
}

void *SLNextItem(SortedListIteratorPtr iter)
{
	//printf("%d\n", *(int*)(iter->list->head->data));
    if(iter->current == NULL || iter == NULL){
        return NULL;
    }

		
    while(iter->current != NULL && iter->current->moveRef ==1){   //the iterator points to the node and it is removed
        iter->current->counter--;
        iter->current = iter->current->next;				//While the iterator is pointing to a node that has been removed,
        iter->current->counter++;							//We decrement the counter, go the node after that, and increment the counter
    }
    if(iter->current != NULL && iter->current->moveRef ==0 && iter->current->next != NULL){     //If it does not point to a removed node, it points to a node that has data
        //void * temp = iter->current->data;
        iter->current->counter--;     
        //printf("%d\n", *(int*)(iter->current->data));
        iter->current = iter->current->next;  
       // printf("%d\n", *(int*)(iter->current->data));
        return iter->current->data;					//Decrement, return the current node and continue onto the next item in the linked list.
    }
    return NULL;
}

