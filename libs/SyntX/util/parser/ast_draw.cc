#include "ast_draw.h"

size_t ast_draw::count_depth(std::shared_ptr<base_rule::node> const &node, size_t depth /*= 0*/)
{
  static size_t max_depth = 0;

  if (depth > max_depth)
    max_depth = depth;

  for (auto &a_node : node->children) {
    count_depth(a_node, depth + 1);
  }

  return max_depth;
}

void ast_draw::create_svg(std::shared_ptr<base_rule::node> const &node, size_t depth, size_t level_pos /*= 1*/, vector parentLinePoint /*= vector()*/)
{
  if (node) {
    //TEXT
    std::string text;
    switch (node->the_type) {
    case base_rule::node::type::value:
      text = node->the_value;
      break;
    case base_rule::node::type::alternation:
      text = "alternation";
      break;
    case base_rule::node::type::concatenation:
      text = "concatenation";
      break;
    case base_rule::node::type::option:
      text = "option";
      break;
    case base_rule::node::type::repetition:
      text = "repetition";
      break;
    case base_rule::node::type::repetition_or_epsilon:
      text = "repetition_or_epsilon";
      break;
    case base_rule::node::type::named_rule:
      text = "named_rule: " + node->the_value;
      break;
    }
    //POSITION
    size_t x_pos;
    size_t y_pos;

    size_t offset = level_pos;
    if (offset % 2 == 0)
      offset += 1;

    x_pos = ((svg_width / (std::pow(2, depth + 1))) * offset);
    y_pos = depth * 120 + 70;

    //SYMBOL
    double svg_ellipse_rx = 50 + text.length() * 1.6;
    double svg_ellipse_ry = 35;

    svg_file_content +=
      "<ellipse cx=\"" +
      std::to_string(x_pos + (text.length() * 3.4)) +
      "\" cy=\"" +
      std::to_string(y_pos) +
      "\" rx=\"" + std::to_string(svg_ellipse_rx) +
      "\" ry=\"" + std::to_string(svg_ellipse_ry) +
      "\" style=\""
      "stroke:#ff0000;stroke-width: 2;stroke: black;fill: none;\"/>";
    svg_file_content += "<text x=\"" + std::to_string(x_pos) + "\" y=\"" + std::to_string(y_pos) + "\"" + " fill=\"black\">";
    svg_file_content += text + "</text>";
    svg_file_content += "\n";
    if (depth != 0) {
      svg_file_content +=
        "<line x1=\"" + std::to_string(parentLinePoint.x) +
        "\" y1=\"" + std::to_string(parentLinePoint.y) +
        "\" x2=\"" + std::to_string(x_pos + svg_ellipse_rx / 2.0) +
        "\" y2=\"" + std::to_string(y_pos - svg_ellipse_ry / 1.1) +
        "\" style=\"stroke: black;stroke-width:3\" />";
    }

    for (unsigned int i = 0; i < node->children.size(); i++) {
      size_t temp_level_pos = level_pos * 2;
      vector linePoint;
      if (i == 0) {
        temp_level_pos--;
        linePoint.x = x_pos;
        linePoint.y = y_pos + svg_ellipse_ry;
      }
      else {
        temp_level_pos++;
        linePoint.x = x_pos + svg_ellipse_rx;
        linePoint.y = y_pos + svg_ellipse_ry;
      }

      create_svg(node->children[i], depth + 1, temp_level_pos, linePoint);
    }
  }
}

void ast_draw::draw_to_file(std::shared_ptr<base_rule::node> const &node, std::string path /*= "ast.html"*/)
{
  using namespace std;

  double depth = count_depth(root);
  svg_width = pow(2, depth) * 70;
  svg_height = depth * 150 + 70;

  create_svg(root, 0);

  std::ofstream svg_file;
  svg_file.open(path);

  //svg_file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << endl;
  svg_file << "<html><head></head><body>" << endl;
  svg_file << "<svg width=\"" << to_string(svg_width) << "\"  " << "height=\"" << to_string(svg_height) << "\">" << std::endl;

  svg_file << svg_file_content << endl;

  svg_file << "</svg>" << endl;
  svg_file << "</body></html>";
  svg_file.close();

  std::cout << "SVG_Height: " << svg_height << endl;
  std::cout << "SVG_Width: " << svg_width << endl;
  std::cout << "AST_depth: " << depth << endl;
}

void ast_draw::draw_to_file(std::string path /*= "ast.html"*/)
{
  draw_to_file(root);
}

void ast_draw::to_formatted_string(std::shared_ptr<base_rule::node> const &node, size_t depth /*= 0*/)
{
  if (node) {
    for (size_t i = 0; i < depth; ++i) std::cout << "   ";

    /*
    std::cout << "--" << std::endl;
    for (size_t i = 0; i < depth; ++i) std::cout << "   ";
    std::cout << "Parent: ";
    if (node->parent != nullptr) {
      switch (node->parent->the_type) {
      case base_rule::node::type::value:
        std::cout << node->the_value << std::endl;
        break;

      case base_rule::node::type::alternation:
        std::cout << "alternation" << std::endl;
        break;

      case base_rule::node::type::concatenation:
        std::cout << "concatenation" << std::endl;
        break;

      case base_rule::node::type::option:
        std::cout << "option" << std::endl;
        break;

      case base_rule::node::type::repetition:
        std::cout << "repetition" << std::endl;
        break;

      case base_rule::node::type::repetition_or_epsilon:
        std::cout << "repetition_or_epsilon" << std::endl;
        break;

      case base_rule::node::type::named_rule:
        std::cout << "named_rule: " << node->the_value << std::endl;
        break;
      }
    }
    else
    {
      std::cout << "nullptr" << std::endl;
    }
    */

    switch (node->the_type) {
    case base_rule::node::type::value:
      std::cout << node->the_value << std::endl;
      break;

    case base_rule::node::type::alternation:
      std::cout << "alternation" << std::endl;
      break;

    case base_rule::node::type::concatenation:
      std::cout << "concatenation" << std::endl;
      break;

    case base_rule::node::type::option:
      std::cout << "option" << std::endl;
      break;

    case base_rule::node::type::repetition:
      std::cout << "repetition" << std::endl;
      break;

    case base_rule::node::type::repetition_or_epsilon:
      std::cout << "repetition_or_epsilon" << std::endl;
      break;

    case base_rule::node::type::named_rule:
      std::cout << "named_rule: " << node->the_value << std::endl;
      break;

    case base_rule::node::type::deleted:
      std::cout << "deleted" << node->the_value << std::endl;
      break;
    }

    for (auto &a_node : node->children)
      to_formatted_string(a_node, depth + 1);
  }
}

void ast_draw::to_formatted_string(size_t depth /*= 0*/)
{
  to_formatted_string(root);
}

std::shared_ptr<base_rule::node> ast_draw::optimize_ast(std::shared_ptr<base_rule::node> &node) {
  remove_alternations(node);
  remove_one_children_roots(node);
  remove_character_leafs(node);
  rearrange_operators(node);
  remove_nodes_marked_for_deletion(node);
  remove_one_children_roots(node);

  return root;
}

std::shared_ptr<base_rule::node> ast_draw::rearrange_operators(std::shared_ptr<base_rule::node> &node) {
  if (node == nullptr)
    return nullptr;

  if (
    node->parent != nullptr &&
    node->parent->parent != nullptr &&
    node->the_type == base_rule::node::type::named_rule) {
    
    
    auto new_place = node->parent->parent;
    new_place->the_type = node->the_type;
    new_place->the_value = node->the_value;

    for (unsigned int i = 0; i < node->parent->children.size(); i++) {
      auto entry = node->parent->children[i];

      if (entry->the_type == base_rule::node::type::named_rule) {
        if (entry->children.size() == 0) {
          node->the_type = base_rule::node::type::deleted;
          node->parent->children.erase(node->parent->children.begin() + i);
        }
      }
      else if (entry->the_type == base_rule::node::type::value)
      {
        node->parent->the_type = entry->the_type;
        node->parent->the_value = entry->the_value;

        if (entry->children.size() == 0) {
          entry->the_type = base_rule::node::type::deleted;
          //entry->parent->children.erase(node->parent->children.begin() + i);
        }
      }
    }
  }

  rearrange_operators(node->left_children());
  rearrange_operators(node->right_children());

  return node;
}

std::shared_ptr<base_rule::node> ast_draw::remove_character_leafs(std::shared_ptr<base_rule::node> &node) {
  if (node == nullptr)
    return nullptr;
  //Remove character value leafs under the named_rules
  if (node->children.size() == 1 && node->the_type == base_rule::node::type::named_rule) {
    node->children.clear();
  }

  for (auto &entry : node->children)
    remove_character_leafs(entry);

  return node;
}

std::shared_ptr<base_rule::node> ast_draw::remove_alternations(std::shared_ptr<base_rule::node> &node) {
  if (node == nullptr)
    return nullptr;
  //Remove alternations
  for (unsigned int i = 0; i < node->children.size(); i++) {
    auto& entry = node->children[i];

    if (entry->children.size() == 1 && entry->the_type == base_rule::node::type::alternation) {
      entry = entry->children.front();
      entry->parent = node;
      remove_alternations(node);
    }
  }

  
  for (auto &entry : node->children)
    remove_alternations(entry);

  return node;
}

std::shared_ptr<base_rule::node> ast_draw::remove_one_children_roots(std::shared_ptr<base_rule::node> &node) {
  if (node == nullptr)
    return nullptr;

  while ((node->children.size() == 1) && (node->children.front()->the_type != base_rule::node::type::value)) {
    auto new_children = node->children.front()->children;
    node->children.clear();
    node->children = new_children;
    for (auto entry : new_children)
      entry->parent = node;
  }

  for (auto &entry : node->children)
    remove_one_children_roots(entry);

  return node;
}

void ast_draw::remove_nodes_marked_for_deletion(std::shared_ptr<base_rule::node> &node)
{
  if (node == nullptr)
    return;
  
  remove_nodes_marked_for_deletion(node->left_children());
  remove_nodes_marked_for_deletion(node->right_children());
 
  while (node->parent != nullptr && node->parent->children.size() != 0 && node->parent->left_children().get()->the_type == base_rule::node::type::deleted) {
    node->parent->children.erase(node->parent->children.begin());
  }

}

std::shared_ptr<base_rule::node> ast_draw::optimize_ast()
{
  return optimize_ast(root);
}

ast_draw::ast_draw(std::shared_ptr < base_rule::node> _root) {
  root = _root;
}

void ast_draw::set_root(std::shared_ptr < base_rule::node> _root)
{
  root = _root;
}