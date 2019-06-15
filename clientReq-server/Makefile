CFLAGS=-Wall -std=gnu99

# list source code files for server executable
SERVER_SRCS = src/server.c ../lib/src/err_lib.c ../lib/src/server_lib.c ../lib/src/list_lib.c ../lib/src/sig_lib.c ../lib/src/sem_lib.c

# list source code files for client executable
CLIENT_SRCS = src/clientReq.c ../lib/src/err_lib.c ../lib/src/str_lib.c ../lib/src/server_lib.c

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
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	@rm -f src/*.o server clientReq ../lib/src/*.o ./FIFO/FIFOSERVER ./FIFO/FIFOCLIENT
	@echo "Removed object files and executables..."
