#ifndef ast_optimizer_h__
#define ast_optimizer_h__

/* GLOBAL INCLUDES */
#include <memory>

/* LOCAL INCLUDES */
#include "generator_framework.h"
#include "syntx/parser.h"
/* INCLUDES END */

class AstOptimizer
{
public:
  static std::shared_ptr<base_rule::node> optimizeAst(std::shared_ptr<base_rule::node> &node);
private:
  static std::shared_ptr<base_rule::node> rearrangeOperators(std::shared_ptr<base_rule::node> node);
  static std::shared_ptr<base_rule::node> rearrangeOneOperandOperators(std::shared_ptr<base_rule::node> node);
  static std::shared_ptr<base_rule::node> removeLparRpar(std::shared_ptr<base_rule::node> node);
  static std::shared_ptr<base_rule::node> removeCharacterLeafs(std::shared_ptr<base_rule::node> node);
  static std::shared_ptr<base_rule::node> removeAlternations(std::shared_ptr<base_rule::node> node);
  static std::shared_ptr<base_rule::node> removeOneChildrenRoots(std::shared_ptr<base_rule::node> node);
  static void removeNodesMarkedForDeletion(std::shared_ptr<base_rule::node> node);
};

#endif // ast_optimizer_h__