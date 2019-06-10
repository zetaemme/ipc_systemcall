CFLAGS=-Wall -std=gnu99

# list source code files for server executable
SERVER_SRCS = src/server.c ../lib/include/err_lib.h ../lib/include/server_lib.h ../lib/include/list_lib.h ../lib/include/request_lib.h ../lib/include/sig_lib.h ../lib/include/sem_lib.h

# list source code files for client executable
CLIENT_SRCS = src/clientReq.c ../lib/include/err_lib.h ../lib/include/str_lib.h ../lib/include/request_lib.h ../lib/include/server_lib.h

##------------------------------------------------------------------------------
## DO NOT TOUCH BELOW THIS LINE!
##------------------------------------------------------------------------------
SERVER_OBJS=$(SERVER_SRCS:.c=.o)
CLIENT_OBJS=$(CLIENT_SRCS:.c=.o)

all: server clientReq

server: $(SERVER_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@

clientReq: $(CLIENT_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@

.c.o:
	@echo "Compiling: "$<
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean

clean:
	@rm -f src/*.o server clientReq ../lib/src/*.o
	@echo "Removed object files and executables..."
