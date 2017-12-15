#include "safra_node.hpp"

#include <iostream>
#include <algorithm>

namespace safra {

  bool SafraNode::is_valid() const {
    // No empty nodes
    if (label.size() == 0) {
      std::cout << "Empty nodes" << std::endl;
      return false;
    }

    std::set<int> child_union;
    unsigned int total_child_elems = 0;

    for (auto child : children) {
      // Children must be valid nodes
      if (!child.is_valid()) {
        return false;
      }
      child_union.insert(child.label.begin(), child.label.end());
      total_child_elems += child.label.size();
      // Children labels must be subset of label
      if (!std::includes(label.begin(), label.end(),
            child.label.begin(), child.label.end())) {
        std::cout << "Child is not subset" << std::endl;
        return false;
      }
      // Children must be older than current node
      if (child.name <= name) {
        std::cout << "Child are too young" << std::endl;
        return false;
      }
    }

    // Children labels must be disjoint (Horizontal Merge)
    if (total_child_elems != child_union.size()) {
      std::cout << "Child labels not disjoint" << std::endl;
      return false;
    }

    // Children labels cannot form a partition of label (Vertical Merge)
    if (total_child_elems == label.size()) {
      std::cout << "Child labels forming partition" << std::endl;
    }
    return total_child_elems != label.size();
  }

  int SafraNode::find_name(int n) const {
    if (n == name && mark) {
      return 1;
    }
    if (n == name) {
      return 0;
    }

    int status;
    for (auto child : children) {
      status = child.find_name(n);
      if (status >= 0) return status;
    }
    return -1;
  }


  bool operator== (const SafraNode &o1, const SafraNode &o2) {
    return o1.name == o2.name && o1.mark == o2.mark && o1.label == o2.label
      && o1.children == o2.children;
  }

  std::ostream& operator<< (std::ostream& stream, const SafraNode& node) {
    if (node.mark) {
      stream << "!";
    }
    stream << node.name;
    stream << " {";
    for (auto l : node.label) stream << l << ", ";
    stream << "}" << std::endl;

    return stream;
  }

}
