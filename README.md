Safra's Determinization Algorithm
===================
An implementation of Safra's Determinization Algorithm specifically for converting Buechi Automata to Rabin Automata

#Documentation
The automata directory contains the Buechi, Rabin, and generalized Automata types. These files also contain
handy methods for visualizing the machines in plain text, graphviz dot format, or pdf. 
Note: pdf conversion may be slow for very large automata.

The safratree directory contains the code for the SafraTree type, the individual states in Safra's algorithm, represented as a tree of SafraNodes. These tree's each have their own name vocabulary of `2 * num_buechi_states` which they manage.  Furthermore for easy debugging, they come with their own `is_valid` methods that check the tree invariant.

Finally this is all tied together in `safras.cpp` which contains the actual safras determinization code. For the algorithm I followed [this approach](https://swt.informatik.uni-freiburg.de/teaching/WS2015-16/AutomataTheory/Resources/Slides/safradeterminizationalgorithm-seminarslides-adityaoak.pdf). This file contains the hook for converting Buechi objects to Rabin through a `to_rabin()` function. This function uses a map from SafraTree's to state numbers, as well as a frontier of unexplored SafraTrees to compute all the states and transitions. Whenever a new state is encountered, it is added to the frontier so that it's transitions may be later computed. The map ensures we do not take any duplicated states.

For actually computing the transitions, I split it up into 6 steps:

 1. Copy the current state tree and remove all marks
 2. Branch all nodes which have accepting states
 3. Transition all individual nodes using the Buechi transition function
 4. Perform a horizontal merge
 5. Remove empty nodes
 6. Perform a vertical merge

# Usage

To use as a standalone tool simply run `make` and follow the usage format printed when you run `./safra`

To use the code pass in a file name with the buechi automata, or specify the object yourself:
```
#include "automata/buechi.cpp"
#include "automata/rabin.cpp"

...
// Read Buechi from a file (check tests/ for example of format)
// The boolean specifies whether to use the standard english alphabet for transitions
Buechi b = Buechi("tests/monster5.aut", true);
Rabin r = b.to_rabin();

// Specify Buechi yourself
Buechi b = Buechi();
b.num_states = ...
b.alphabet_size = ...
b.num_transitions = ...
b.start_states.insert(state)
b.accept_states.insert(state)
b.transitions.emplace(std::pair<int, int>(from_state, letter), to_state)
Rabin r = b.to_rabin();
```

# Results

My implementation was able to achieve reasonable speed and accuracy. I was unable to generate the optimal Rabin machine, but in most cases it is pretty close. For verification hand verification was performed on all the smaller tests, but no verification was attempted for the larger tests.

Here is a breakdown over the most challenging tests
###Accuracy and Efficiency
Test     | Rabin States | Rabin Transitions | % more states than optimal| %more transitions than optimal | Runtime (s)|
-------- | :-----------:|:-----------------:|-----------------------------------------:|----------:|------:|
littlemonster3 | 21 | 63 | 57%| 57% | 0.004|
littlemonster4 | 257 | 1028 | 49% | 49% | 0.022|
littlemonster5 | 10000 | ??? | 33% | ??? | 1.472|
monster3 | 14 | 28 | 35% | 35% | 0.002 |
monster4 | 183 | 549 | 14% | 14% | 0.015 |
monster5 | 7108 | 28432 | 1% | 1% | 0.668 |

The tests were run on an Intel I7 4 core processor with 8GB of RAM.

The results seem to vary wildly, with poor accuracy on some of the smaller tests, but within 1% of optimal on the hardest test (monster5).
