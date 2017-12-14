#include "safra_node.hpp"

#include <iostream>
#include <algorithm>

namespace safra {

  bool SafraNode::is_valid() const {
    // No emtpy nodes
    if (label.size() == 0)
      return false;

    std::set<int> child_union;
    int total_child_elems;

    for (auto child : children) {
      // Children must be valid nodes
      if (!child.is_valid())
        return false;
      child_union.insert(child.label.begin(), child.label.end());
      total_child_elems += child.label.size();
      // Children labels must be subset of label
      if (!std::includes(child.label.begin(), child.label.end(),
            label.begin(), label.end()))
        return false;
      // Children must be older than current node
      if (child.name <= name)
        return false;
    }

    // Children labels must be disjoint (Horizontal Merge)
    if (total_child_elems != child_union.size())
      return false;

    // Children labels cannot form a partition of label (Vertical Merge)
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
      if (status > 0) return status;
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
  }

}
