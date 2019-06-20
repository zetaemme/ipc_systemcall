#include <unistd.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/types.h>

#include "../../lib/include/list_lib.h"
#include "../../lib/include/str_lib.h"
#include "../../lib/include/err_lib.h"
#include "../../lib/include/sem_lib.h"
#include "../../lib/include/exec_lib.h"

int main (int argc, char *argv[]) {
    if(argc < 4) {
        err_exit("<Exec> Not enough argumets");
    }

    // argv[1] = id, argv[2] = user_key, argv[3] = args
    int integer_user_key = atoi(argv[2]);

    // ottengo la chiave per il segmento di memoria condivisa
    key_t shmKey = ftok("../clientReq-server/src/server.c", 300);
    if(shmKey == -1){
        err_exit("ftok failed");
    }

    // ottengo il suo identificatore
    int shm_id = shmget(shmKey, sizeof(Node_t) * 20, S_IRUSR | S_IWUSR);

    if(shm_id == -1){
        err_exit("shmget failed");
    }

    // attacco clientExec al segmento di memoria condivisa
    Node_t *exec_shm = (Node_t *)shmat(shm_id, NULL, 0);

    if(exec_shm == (Node_t *)-1){
        err_exit("shmat failed");
    }

    // ottengo la chiave per il semaforo
    key_t semKey = ftok("../clientReq-server/src/clientReq.c", 100);

    // ottengo il semaforo
    int sem_id = semget(semKey, 1, S_IRUSR | S_IWUSR);
    if(sem_id == -1){
        err_exit("(mutex) semget failed");
    }

    // =========== OPERAZIONE PROTETTA ===========

    // Semaphore protects the operations below
    semOp(sem_id, 0, -1);

    // Current node on the shared memory list
    Node_t *current = get_node(exec_shm, argv[1], integer_user_key);

    if(current == NULL) {
        printf("Invalid key, generate another one!\n");
        semOp(sem_id, 0, 1);
        exit(1);
    } else if(has_been_used(current) == false) {
        printf("Already used key, generate another one!\n");
        semOp(sem_id, 0, 1);
        exit(1);
    } else {
        current -> has_been_used = 1;
    }

    // Shared Memory is now accessible to everyone who wants
    semOp(sem_id, 0, 1);

    // =========================================

    char *args = args_to_string(argv);

    int choice = get_first_digit(integer_user_key);

    switch(choice){
    	case 1:{
            if(execlp("./stampa", "./stampa", args, (char*)NULL) == -1){
                err_exit("execlp failure");
            }

            break;            
    	}
    	case 2:{
            char filename[100];
    		printf("Insert file's name: ");
    		scanf("%s", filename);

    		if(execlp("./salva", "./salva", filename, args, (char*)NULL) == -1){
                err_exit("execlp failure");
    		}

    		break;
    	}
	   	default:{
            int msq_key;
            printf("Insert Message Queue's key: ");
            scanf("%i", &msq_key);

            // Converts msq_key into string
            char key_str[30];
            sprintf(key_str, "%i", msq_key);

    		if(execlp("./invia", "./invia", key_str, args, (char*)NULL) == -1){
                err_exit("execlp failure");
            }

    		break;
    	}
    }
	   	    
    return 0;
}