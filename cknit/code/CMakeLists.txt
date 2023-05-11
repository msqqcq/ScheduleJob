cmake_minimum_required(VERSION 3.10)
project(cknit C)

set(CMAKE_C_STANDARD 99)
set(MAIN_SOURCE main.c)
set(
    ALL_PERMISSIONS
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_READ GROUP_WRITE GROUP_EXECUTE
    WORLD_READ WORLD_WRITE WORLD_EXECUTE
)

include_directories(
    src/json
    src/http
    src/task
    src/timer
    src/events
    src/log
    src/tool
)

aux_source_directory(src/json   JSON_SOURCE)
aux_source_directory(src/http   HTTP_SORUCE)
aux_source_directory(src/task   TASK_SOURCE)
aux_source_directory(src/timer  TIMER_SOURCE)
aux_source_directory(src/events EVENT_SOURCE)
aux_source_directory(src/log    LOG_SOURCE)
aux_source_directory(src/tool   TOOL_SOURCE)

add_executable(
        cknit
        ${MAIN_SOURCE}
        ${JSON_SOURCE}
        ${HTTP_SORUCE}
        ${TASK_SOURCE}
        ${TIMER_SOURCE}
        ${EVENT_SOURCE}
        ${LOG_SOURCE}
        ${TOOL_SOURCE}
)
target_link_libraries(cknit pthread)

install(TARGETS cknit RUNTIME DESTINATION /usr/local/bin)
install(FILES conf/cknit.exjson DESTINATION /etc/cknit)
install(FILES conf/task.exjson DESTINATION /etc/cknit)
install(DIRECTORY DESTINATION /var/log/cknit DIRECTORY_PERMISSIONS ${ALL_PERMISSIONS})
install(FILES conf/default_api.exjson DESTINATION /etc/cknit)


