#include <stdio.h>
#include <stdlib.h>
void push();
void pop();
void display();

struct node *head;

struct node
{
  int val;
  struct node *next;
};

void push()
{
  struct node *ptr = (struct node *)malloc(sizeof(struct node));
  if (ptr == NULL)
  {
    printf("not able to push the element");
  }
  else
  {
   
    if (head == NULL)
    {
      ptr->val = val;
      ptr->next = NULL;
      head = ptr;
    }
    else
    {
      ptr->val = val;
      ptr->next = head;
      head = ptr;
    }
    printf("Item pushed");
  }
}

void pop()
{
  int item;
  struct node *ptr;
  if (head == NULL)
  {
    printf("Underflow");
  }
  else
  {
    ptr = head;
    head = head->next;
    free(ptr);
    printf("Item popped");
  }
}
void display()
{
  int i;
  struct node *ptr;
  ptr = head;
  if (ptr == NULL)
  {
    printf("Stack is empty\n");
  }
  else
  {
    printf("Printing Stack elements \n");
    while (ptr != NULL)
    {
      printf("%d\n", ptr->val);
      ptr = ptr->next;
    }
  }
}