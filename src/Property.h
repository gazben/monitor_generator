#ifndef Property_h__
#define Property_h__

/* GLOBAL INCLUDES */
#include <stdlib.h>

/* LOCAL INCLUDES */

/* INCLUDES END */

/* FUNCTION TYPE DEFINITIONS */
typedef struct Property;
typedef OutputState(*PROP_evalFunctionType)(unsigned long long int);
typedef void(*PROP_constructDescendantNodeType)(struct Property*);

typedef struct Property{
  struct Property* rootNode;
  struct Property* descendantNode;

  StateRegisterState* stateRegisterPtr;
  PROP_evalFunctionType evalFunction;
  PROP_constructDescendantNodeType constructDescendantNode;
}Property;
#endif // Property_h__
