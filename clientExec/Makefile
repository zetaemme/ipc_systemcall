CFLAGS = -Wall -std=gnu99

# list source code files for client exec executable
CLIENT_SRCS = src/clientExec.c ../lib/src/list_lib.c ../lib/src/str_lib.c ../lib/src/server_lib.c ../lib/src/err_lib.c

# list source code files for salva executable
SALVA_SRCS = src/salva ../lib/src/err_lib.c

# list source code files for stampa executable
STAMPA_SRCS = src/stampa.c

# list source code files for invia executable
INVIA_SRC = src/invia.c ../lib/src/err_lib.c ../src/msg_lib.c

##------------------------------------------------------------------------------
## DO NOT TOUCH BELOW THIS LINE!
##------------------------------------------------------------------------------
CLIENT_OBJS=$(CLIENT_SRCS:.c=.o)
SALVA_OBJS=$(SALVA_SRCS:.c=.o)
STAMPA_OBJS=$(STAMPA_SRCS:.c=.o)
INVIA_OBJS=$(INVIA_SRCS:.c=.o)

all: clientExec salva stampa invia

clientExec: $(CLIENT_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@

salva: $(SALVA_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@

stampa: $(STAMPA_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@

invia: $(INVIA_OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@

.c.o:
	@echo "Compiling: "$<
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	@rm -f src/*.o clientExec stampa salva invia ../lib/src/*.o
	@echo "Removed object files and executables..."
