#ifndef StateRegister_h__
#define StateRegister_h__

/* GLOBAL INCLUDES */
#include <stdlib.h>

/* LOCAL INCLUDES */

/* INCLUDES END */

unsigned long long int stateRegister;
unsigned long long int SR_getStateRegister(){
  return stateRegister;
}

typedef struct StateRegisterState{
  unsigned long long int stateRegisterState;
  struct StateRegisterState* rightNode;
  struct StateRegisterState* leftNode;
}StateRegisterState;
StateRegisterState* SR_rootPtr;

StateRegisterState* SR_initStateRegisterState(unsigned long long int StateRegisterCopy){
  StateRegisterState* temp = (StateRegisterState*)malloc(sizeof(StateRegisterState));
  temp->leftNode = NULL;
  temp->rightNode = NULL;
  temp->stateRegisterState = StateRegisterCopy;

  return temp;
}

StateRegisterState* SR_insertState(StateRegisterState* root, unsigned long long int StateRegisterCopy){
  if (root == NULL) {
    StateRegisterState *temp = (StateRegisterState*)malloc(sizeof(StateRegisterState));
    temp->leftNode = temp->rightNode = NULL;
    temp->stateRegisterState = StateRegisterCopy;
    return temp;
  }

  if (StateRegisterCopy < root->stateRegisterState)
    root->leftNode = SR_insertState(root->leftNode, StateRegisterCopy);
  else if (StateRegisterCopy > root->stateRegisterState)
    root->rightNode = SR_insertState(root->rightNode, StateRegisterCopy);
  else
    ;

  return root;
}

/*If the state is not found it will be inserted to the tree. The new node pointer will be returned.*/
StateRegisterState* SR_getStatePointer(unsigned long long int StateRegisterCopy){
  StateRegisterState* temp = SR_rootPtr;

  while (temp != NULL && temp->stateRegisterState != StateRegisterCopy) {
    if (StateRegisterCopy < temp->stateRegisterState){
      if (temp->leftNode == NULL){
        temp->leftNode = SR_initStateRegisterState(StateRegisterCopy);
        return temp->leftNode;
      }
      temp = temp->leftNode;
    }
    else{
      if (temp->rightNode == NULL){
        temp->rightNode = SR_initStateRegisterState(StateRegisterCopy);
        return temp->rightNode;
      }
      temp = temp->rightNode;
    }
  }

  return temp;
}

#endif // StateRegister_h__
