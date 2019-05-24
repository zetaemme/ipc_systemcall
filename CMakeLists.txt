set(PROJECT "ipc_systemcall")
project(${PROJECT})

cmake_minimum_required(VERSION 3.12)

include_directories(REQ_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/clientReq-server/include)
include_directories(UTILS_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/utils/include)
include_directories(EXEC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/clientExec/include)

set(REQ_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/clientReq-server/src/clientReq.c
)

set(SERVER_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/clientReq-server/src/server.c
)

set(UTILS_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/src/errExit.c
)

set(EXEC_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/clientExec/src/clientExec.c
        ${CMAKE_CURRENT_SOURCE_DIR}/clientExec/src/invia.c
        ${CMAKE_CURRENT_SOURCE_DIR}/clientExec/src/salva.c
        ${CMAKE_CURRENT_SOURCE_DIR}/clientExec/src/stampa.c
)

add_executable(SERVER ${SERVER_SRC} ${REQ_INCLUDE} ${UTILS_SRC} ${UTILS_INCLUDE})
add_executable(REQUEST ${REQ_SRC} ${REQ_INCLUDE} ${UTILS_SRC} ${UTILS_INCLUDE})
add_executable(EXEC ${EXEC_SRC} ${EXEC_INCLUDE} ${UTILS_SRC} ${UTILS_INCLUDE})