add_compile_options(-Wall -Werror -Wextra -Wpedantic -std=c99 -pedantic -lm)

set(MySources 
    ${PROJECT_SOURCE_DIR}/src/ptr_string
    ${PROJECT_SOURCE_DIR}/src/parser
    ${PROJECT_SOURCE_DIR}/src/iterator 
    ${PROJECT_SOURCE_DIR}/src/error
    ${PROJECT_SOURCE_DIR}/src/deque 
    ${PROJECT_SOURCE_DIR}/src/queue
    ${PROJECT_SOURCE_DIR}/src/scaner
    ${PROJECT_SOURCE_DIR}/src/stack
)

foreach(source ${MySource})
    add_library("ifj${source}" ${source}.c)
endforeach(source)

# add_executable(ifj19 main.c)

# target_link_libraries(ifj19 ifj m)
