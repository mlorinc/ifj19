add_compile_options(-Wall -Werror -Wextra -Wpedantic -std=c99 -pedantic -lm)

set(MySources 
    ${PROJECT_SOURCE_DIR}/src/ptr_string.c 
    ${PROJECT_SOURCE_DIR}/src/parser.c 
    ${PROJECT_SOURCE_DIR}/src/iterator.c 
    ${PROJECT_SOURCE_DIR}/src/error.c 
    ${PROJECT_SOURCE_DIR}/src/deque.c 
    ${PROJECT_SOURCE_DIR}/src/queue.c 
    ${PROJECT_SOURCE_DIR}/src/scaner.c 
    ${PROJECT_SOURCE_DIR}/src/stack.c
)

add_library(ifj ${MySources})

# add_executable(ifj19 main.c)

# target_link_libraries(ifj19 ifj m)
