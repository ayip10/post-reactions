# Makefile for HW5, ecs36b, s2020
#

CC = g++ -std=c++14
CFLAGS = -g -I/usr/include/jsoncpp

# CFLAGS = -g -D_ECS35B_DEBUG_ -I/usr/include/jsoncpp
# CFLAGS = -g -Wall -Wstrict-prototypes
# CFLAGS = -O3

LDFLAGS = 	-ljsoncpp -lmicrohttpd -ljsonrpccpp-common -ljsonrpccpp-server -lcurl -ljsonrpccpp-client

CORE_INCS =	Core.h Person.h Post.h Comment.h Reaction.h	\
		Action.h Link.h Message.h JvTime.h Tag.h Transaction.h
CORE_OBJS =	Core.o Person.o Post.o Comment.o Reaction.o	\
	 	Action.o Link.o JvTime.o Tag.o Transaction.o 

# rules.
all: 	hw7update hw7search hw7history hw7validate hw6client 

#
#

hw6client.h:		ecs36b_hw6.json
	jsonrpcstub ecs36b_hw6.json --cpp-server=hw6Server --cpp-client=hw6Client

hw6client.o:		hw6client.h hw6client.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGS) hw6client.cpp

hw7validate.o:		hw6client.h hw6client.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGS) hw7validate.cpp

hw6update.o:		hw6update.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGS) hw6update.cpp

hw7update.o:		hw7update.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGS) hw7update.cpp

hw7search.o:		hw7search.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGS) hw7search.cpp

hw7history.o:		hw7history.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGS) hw7history.cpp

hw7update:		$(CORE_OBJS) hw7update.o JSON_Post.o
	$(CC) -o hw7update $(CORE_OBJS) hw7update.o JSON_Post.o $(LDFLAGS)

hw7history:		$(CORE_OBJS) hw7history.o JSON_Post.o
	$(CC) -o hw7history $(CORE_OBJS) hw7history.o JSON_Post.o $(LDFLAGS)

hw7server:		$(CORE_OBJS) hw7server.o JSON_Post.o
	$(CC) -o hw7server $(CORE_OBJS) hw6server.o JSON_Post.o $(LDFLAGS)

hw6client:		$(CORE_OBJS) hw6client.o JSON_Post.o
	$(CC) -o hw6client $(CORE_OBJS) hw6client.o JSON_Post.o $(LDFLAGS)

hw6update:		$(CORE_OBJS) hw6update.o JSON_Post.o
	$(CC) -o hw6update $(CORE_OBJS) hw6update.o JSON_Post.o $(LDFLAGS)

hw7search:		$(CORE_OBJS) hw7search.o JSON_Post.o
	$(CC) -o hw7search $(CORE_OBJS) hw7search.o JSON_Post.o $(LDFLAGS)

hw7validate:		$(CORE_OBJS) hw7validate.o JSON_Post.o
	$(CC) -o hw7validate $(CORE_OBJS) hw7validate.o JSON_Post.o $(LDFLAGS)

Core.o:			Core.cpp Core.h
	$(CC) -c $(CFLAGS) Core.cpp

Tag.o:			Tag.cpp Tag.h
	$(CC) -c $(CFLAGS) Tag.cpp

Person.o:		Person.cpp Person.h Core.h
	$(CC) -c $(CFLAGS) Person.cpp

Post.o:			Post.cpp Post.h
	$(CC) -c $(CFLAGS) Post.cpp

Comment.o:		Comment.cpp Comment.h
	$(CC) -c $(CFLAGS) Comment.cpp

Reaction.o:		Reaction.cpp Reaction.h
	$(CC) -c $(CFLAGS) Reaction.cpp

JvTime.o:		JvTime.cpp JvTime.h
	$(CC) -c $(CFLAGS) JvTime.cpp

Transaction.o:		Transaction.cpp Transaction.h
	$(CC) -c $(CFLAGS) Transaction.cpp

Link.o:			Link.cpp Link.h
	$(CC) -c $(CFLAGS) Link.cpp

Action.o:		Action.cpp Action.h
	$(CC) -c $(CFLAGS) Action.cpp

JSON_Post.o:		JSON_Post.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGS) JSON_Post.cpp

clean:
	rm -f *.o *~ Core hw6server hw6client hw6search hw6update 


