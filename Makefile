CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -O3 -Wall -Werror -Wextra -std=c++11
LDFLAGS=-g
LDLIBS=

SRCS=src/main.cpp src/automata/automata.cpp src/automata/buechi.cpp src/automata/rabin.cpp
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
