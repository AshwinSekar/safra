#ifndef _safra_SAFRA_TREE_HPP_
#define _safra_SAFRA_TREE_HPP_

#include "safra_node.hpp"
#include "../automata/buechi.hpp"

#include <vector>

namespace safra {

  class SafraTree {
    public:
      // Available names between 0 and 2n
      std::vector<bool> names;
      SafraNode root;

      SafraTree(int buechi_size);
      SafraTree(const Buechi& buechi);

      bool is_valid() const;

      int find_name(int n) { return root.find_name(n); }

      friend bool operator== (const SafraTree &o1, const SafraTree &o2);
      friend bool operator!= (const SafraTree &o1, const SafraTree &o2) {
        return !(o1 == o2);
      }

      friend std::ostream& operator<< (std::ostream& stream, const SafraTree& tree);
    private:
      // Get the next smallest unused name and mark it used
      int next_name();
      // Mark node and childrens names as unused
      void remove_names(SafraNode& node);
  };

}

#endif
