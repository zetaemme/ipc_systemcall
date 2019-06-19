CFLAGS = -Wall -std=gnu99

# list source code files for client exec executable
CLIENT_SRCS = src/clientExec.c ../lib/src/list_lib.c ../lib/src/str_lib.c ../lib/src/err_lib.c ../lib/src/server_lib.c ../lib/src/sem_lib.c

# list source code files for salva executable
SALVA_SRCS = src/salva.c ../lib/src/err_lib.c

# list source code files for stampa executable
STAMPA_SRCS = ./src/stampa.c ../lib/src/err_lib.c

# list source code files for invia executable
INVIA_SRCS = ./src/invia.c ../lib/src/err_lib.c ../lib/src/msg_lib.c

##------------------------------------------------------------------------------
## DO NOT TOUCH BELOW THIS LINE!
##------------------------------------------------------------------------------
CLIENT_OBJS=$(CLIENT_SRCS:.c=.o)
SALVA_OBJS=$(SALVA_SRCS:.c=.o)
STAMPA_OBJS=$(STAMPA_SRCS:.c=.o)
INVIA_OBJS=$(INVIA_SRCS:.c=.o)

all: clientExec salva stampa invia

clientExec: $(CLIENT_OBJS)
	@echo "Making executable: "`tput bold`$@`tput sgr0`
	@echo
	@$(CC) $^ -o $@

salva: $(SALVA_OBJS)
	@echo "Making executable: "`tput bold`$@`tput sgr0`
	@echo
	@$(CC) $^ -o $@

stampa: $(STAMPA_OBJS)
	@echo "Making executable: "`tput bold`$@`tput sgr0`
	@echo
	@$(CC) $^ -o $@

invia: $(INVIA_OBJS)
	@echo "Making executable: "`tput bold`$@`tput sgr0`
	@$(CC) $^ -o $@

.c.o:
	@echo "Compiling: "$<
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	@rm -f src/*.o clientExec salva stampa invia ../lib/src/*.o
	@ipcrm -a
	@echo "Removed object files and executables..."
