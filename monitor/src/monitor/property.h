#ifndef Property_h__
#define Property_h__

/* GLOBAL INCLUDES */
#include <functional>
#include <vector>
#include <iostream>
#include <ros/ros.h>
/* LOCAL INCLUDES */
#include "output_state.h"
#include "state_register.h"
/* INCLUDES END */

#define DEBUG_NO_ROS

#ifdef DEBUG_NO_ROS
  #include <iostream>
  #define ROS_INFO_STREAM(args) std::cout<<(args)<<std::endl
#endif

/* FUNCTION TYPE DEFINITIONS */
using namespace std;

class Property{
protected:
  static Property*currentBlock;
  StateRegister * stateRegisterPtr;
  static unsigned int currentMaxID;
  static unsigned int level;
  unsigned int ID;
  static const unsigned int maxDepth;
public:
  Property();
  ~Property();

  vector<trilean> inputStates;
  vector<trilean> outputStates;
  Property* rootNode;
  Property* childrenNode;

  std::function < class Property*(class Property*) > constructChildrenNodeFunc;
  std::vector <std::function < trilean(class Property*) >> evalFunctions;
  Property* constructChildrenBlock();
  trilean isEventFired(SR_regtype eventCode);
  trilean Evaluate();
  void freeChildrenNode();

  void printBlock(Property*);
};

//Declaration
//--DECLARATIONS--

#endif // Property_h__
