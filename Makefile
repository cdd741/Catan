CXX = g++
CXXFLAGS = -std=c++14 -Wall -Werror=vla -MMD
EXEC = a5exec
OBJECTS = main.o address.o tile.o builder.o board.o resource.o status.o dice.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}


simple:
	g++ -g -std=c++14 address.cc board.cc builder.cc dice.cc main.cc resource.cc status.cc tile.cc -o ./${EXEC}
