#include "safra_tree.hpp"

#include <assert.h>

namespace safra {

  SafraTree::SafraTree(int buechi_size) {
    for(int i = 0; i < buechi_size * 2; i++) {
      names.push_back(false);
    }
    root.name = -1;
  }

  SafraTree::SafraTree(const Buechi& buechi) {
    for(int i = 0; i < buechi.num_states * 2; i++) {
      names.push_back(false);
    }
    root.name = 0;
    names[0] = true;
    // By Vertical merge
    root.mark = false;
    // Create label with initial (start states)
    root.label.insert(buechi.start_states.begin(), buechi.start_states.end());
  }

  bool SafraTree::is_valid() const {
    if (root.name == -1) {
      return true;
    }
    return (root.name == 0) && root.is_valid();
  }

  int SafraTree::next_name() {
    unsigned int i;
    for (i = 0; i < names.size(); i++) {
      if (!names[i]) break;
    }
    assert(i < names.size());
    names[i] = true;
    return i;
  }

  void SafraTree::remove_names(SafraNode& node) {
    assert(names[node.name]);
    names[node.name] = false;
    for (auto child : node.children) remove_names(child);
  }

  bool operator== (const SafraTree &o1, const SafraTree &o2) {
    return o1.names == o2.names && o1.root == o2.root;
  }

  void visualize_tree(std::ostream& stream, const SafraNode& node, int pad) {
    for (int i = 0; i < pad; i++) stream << "    ";
    stream << node << std::endl;
    for (auto child : node.children) visualize_tree(stream, child, pad + 1);
  }

  std::ostream& operator<< (std::ostream& stream, const SafraTree& tree) {
    stream << "[Names] size " << tree.names.size() << std::endl;
    for (bool b : tree.names) {
      stream << b << " ";
    }
    stream << std::endl;
    visualize_tree(stream, tree.root, 0);
    return stream;
  }

}
