/*
The property is made of contraints.
*/

/*
TODO:
- build up a binary tree
-

*/
#include <limits.h>
#include <stdlib.h>

#define EVENT_A 0x1
#define EVENT_B 0x2
#define EVENT_C 0x4
#define EVENT_D 0x8

unsigned long long int stateRegister;

/* TEST FUNCTIONS */
void addEvent(unsigned long long int event_code){
  stateRegister = stateRegister | event_code;
}

void removeEvent(unsigned long long int event_code){
  stateRegister = stateRegister ^ event_code;
}

typedef enum OutputState{
  TRUE, FALSE, UNKNOWN
}OutputState;

/* 3 state logic functions */
OutputState AND_3(OutputState a, OutputState b){
  return (a == FALSE | b == FALSE) ? FALSE : (a == UNKNOWN | b == UNKNOWN) ? UNKNOWN : TRUE;
}
OutputState OR_3(OutputState a, OutputState b){
  return (a == TRUE | b == TRUE) ? TRUE : (a == UNKNOWN | b == UNKNOWN) ? UNKNOWN : FALSE;
}
OutputState XOR_3(OutputState a, OutputState b){
  return (a == UNKNOWN | b == UNKNOWN) ? UNKNOWN : (a == b) ? FALSE : TRUE;
}
OutputState NOT_3(OutputState a){
  return (a == UNKNOWN) ? UNKNOWN : (a == FALSE) ? TRUE : FALSE;
}
int isUnknown(OutputState a){
  return (a == UNKNOWN) ? 1 : 0;
}

typedef OutputState(*evalFunctionType)(int stateRegisterCopy);

/* TEST EVAL FUNCTIONS */
OutputState evalImpl_U(int stateRegisterCopy){
  return UNKNOWN;
}
OutputState evalImpl_T(int stateRegisterCopy){
  return TRUE;
}
OutputState evalImpl_F(int stateRegisterCopy){
  return FALSE;
}

typedef struct Property{
  struct Property* rootNode;
  struct Property* leftNode;
  struct Property* rightNode;

  int stateRegisterCopy;
  evalFunctionType evalFunction;
}Property;

Property* createEmptyProperty(){
  Property* newProperty = (Property*)malloc(sizeof(Property));
  newProperty->leftNode = NULL;
  newProperty->rightNode = NULL;
  newProperty->rootNode = NULL;
  newProperty->evalFunction = NULL;

  return newProperty;
}

Property* addNewPropertyToRoot(int stateRegisterCopy, Property* rootproperty, evalFunctionType evalFunction){
  Property* tempPropertyPtr = createEmptyProperty();
  tempPropertyPtr->rootNode = rootproperty;

  tempPropertyPtr->stateRegisterCopy = stateRegisterCopy;
  tempPropertyPtr->evalFunction = evalFunction;

  if (rootproperty == NULL){
    rootproperty = tempPropertyPtr;
    tempPropertyPtr->rootNode = NULL;
  }
  else if (rootproperty->leftNode == NULL){
    rootproperty->leftNode = tempPropertyPtr;
  }
  else{
    rootproperty->rightNode = tempPropertyPtr;
  }

  return rootproperty;
}

void freePropertyTree(Property* root) {
  if (root == NULL)
    return;

  freePropertyTree(root->rightNode);
  freePropertyTree(root->leftNode);
  free(root);
}