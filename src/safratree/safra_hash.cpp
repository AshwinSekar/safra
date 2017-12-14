#include "safra_node.hpp"
#include "safra_tree.hpp"

std::size_t combine_hash(const std::size_t h, const std::size_t v) {
  return h ^ (v + 0x9e3779b9 + (h << 6) + (h >> 2));
}

namespace std {

  template <> struct hash<safra::SafraNode> {
    size_t operator()(const safra::SafraNode& node) const {
      size_t h = hash<int>()(node.name);
      for (int l : node.label) h = combine_hash(h, hash<int>()(l));
      h = combine_hash(h, hash<bool>()(node.mark));
      for (auto child : node.children) {
        h = combine_hash(h, hash<safra::SafraNode>()(child));
      }
      return h;
    }
  };

  template <> struct hash<safra::SafraTree> {
    size_t operator()(const safra::SafraTree& tree) const {
      size_t h = hash<vector<bool>>()(tree.names);
      h = combine_hash(h, hash<safra::SafraNode>()(tree.root));
      return h;
    }
  };

}
