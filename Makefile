CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -O3 -Wall -Werror -Wextra -std=c++11
LDFLAGS=-g
LDLIBS=

SRCS=src/main.cpp src/safras.cpp src/automata/buechi.cpp src/automata/automata.cpp src/automata/rabin.cpp src/safratree/safra_node.cpp src/safratree/safra_tree.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
GOAL=safra

all: $(GOAL)

safra: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(GOAL) $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS) $(GOAL)

dist-clean: clean
	$(RM) *~ .dependtool

include .depend
