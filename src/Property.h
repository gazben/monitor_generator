#ifndef Property_h__
#define Property_h__

#include <limits.h>
#include <stdlib.h>

#define EVENT_A 0x1
#define EVENT_B 0x2
#define EVENT_C 0x4
#define EVENT_D 0x8

typedef enum OutputState{
  TRUE, FALSE, UNKNOWN
}OutputState;

/* 3 state logic functions */
OutputState AND_3(OutputState a, OutputState b){
  return (a == FALSE | b == FALSE) ? FALSE : (a == UNKNOWN | b == UNKNOWN) ? UNKNOWN : TRUE;
}
OutputState NAND_3(OutputState a, OutputState b){
  return (a == FALSE | b == FALSE) ? TRUE : (a == UNKNOWN | b == UNKNOWN) ? UNKNOWN : FALSE;
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

/* FUNCTION TYPE DEFINITIONS */
typedef struct Property;
typedef OutputState(*PROP_evalFunctionType)(unsigned long long int);
typedef void(*PROP_constructDescendantNodeType)(Property*);

/* TEST EVAL FUNCTIONS */
OutputState EVAL_Impl_U(unsigned long long int event_code){
  return UNKNOWN;
}
OutputState EVAL_Impl_T(unsigned long long int event_code){
  return TRUE;
}
OutputState EVAL_Impl_F(unsigned long long int event_code){
  return FALSE;
}

OutputState EVAL_s0(int StateRegisterCopy, OutputState x1, OutputState x2){
  return AND_3(NAND_3(getEvent(StateRegisterCopy, EVENT_A),
    AND_3(NOT_3(getEvent(StateRegisterCopy, EVENT_B)),
    NAND_3(getEvent(StateRegisterCopy, EVENT_C), x1))), NAND_3(TRUE, x2));
}

OutputState EVAL_s1a(int StateRegisterCopy, OutputState x1, OutputState x2){
  return NAND_3(NOT_3(getEvent(StateRegisterCopy, EVENT_B)),
    NAND_3(getEvent(StateRegisterCopy, EVENT_C), x1));
}

/* CONSTRUCT FUNCTIONS */
void PROP_constructDescImplicateBlock( Property* _this ){
  _this->descendantNode = PROP_createEmptyProperty();
  _this->descendantNode->evalFunction

}


typedef struct Property{
  struct Property* rootNode;
  struct Property* descendantNode;

  StateRegisterState* stateRegisterPtr;
  PROP_evalFunctionType evalFunction;
  PROP_constructDescendantNodeType constructDescendantNode;
}Property;

Property* PROP_createEmptyProperty(){
  Property* newProperty = (Property*)malloc(sizeof(Property));
  newProperty->descendantNode = NULL;
  newProperty->rootNode = NULL;
  newProperty->evalFunction = NULL;
  newProperty->constructDescendantNode = NULL;
  newProperty->stateRegisterPtr = NULL;

  return newProperty;
}

Property* PROP_addNewPropertyToRoot(unsigned long long int stateRegisterCopy, Property* rootproperty, PROP_evalFunctionType evalFunction){
  Property* tempPropertyPtr = PROP_createEmptyProperty();
  tempPropertyPtr->rootNode = rootproperty;

  tempPropertyPtr->stateRegisterPtr = stateRegisterCopy;
  tempPropertyPtr->evalFunction = evalFunction;

  if (rootproperty == NULL){
    rootproperty = tempPropertyPtr;
    tempPropertyPtr->rootNode = NULL;
  }
  else if (rootproperty->descendantNode == NULL){
    rootproperty->descendantNode = tempPropertyPtr;
  }

  return rootproperty;
}


OutputState PROP_evaluateProperty(Property* root){
  Property* temp;

  for (temp = root;;temp = temp->descendantNode){
    if (root->evalFunction(root->stateRegisterPtr->stateRegisterState) != UNKNOWN)
      break;
      //THIS IS NOT WORKING
  }

}

void PROP_freePropertyTree(Property* root) {
  if (root == NULL)
    return;

  PROP_freePropertyTree(root->descendantNode);
  free(root);
}
#endif // Property_h__
