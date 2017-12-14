#ifndef _safra_SAFRA_NODE_HPP_
#define _safra_SAFRA_NODE_HPP_

#include <set>
#include <vector>
#include <iostream>

namespace safra {

  class SafraNode {
    public:
      /*
       * Unique identifier within the tree, from 0 to 2n where n is the number of
       * states in the underlying Buechi automata.
       */
      int name;
      /*
       * The label is the subset of the power set of states in the Buechi automata.
       */
      std::set<int> label;
      /*
       * Mark determines acceptance criteria of Rabin equivalent.
       */
      bool mark;
      /*
       * Child nodes. Union of labels of the children = this nodes label.
       */
      std::vector<SafraNode> children;

      SafraNode() {};
      SafraNode(int _name, bool _mark) {name = _name; mark = _mark; };

      // Invariant check
      bool is_valid() const;

      // Returns -1 if not found, 0 if found, 1 if found and also marked
      int find_name(int n) const;

      /*
       * We need equality operators to remove duplicate nodes/states.
       * All members must be equal
       * TODO: figure out if there is a faster method of comparing equality
       * on nodes with large amounts of children.
       */
      friend bool operator== (const SafraNode &o1, const SafraNode &o2);
      friend bool operator!= (const SafraNode &o1, const SafraNode &o2) {
        return !(o1 == o2);
      }

      friend std::ostream& operator<< (std::ostream& stream, const SafraNode& node);

  };
}

#endif
