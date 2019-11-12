add_compile_options(-Wall -Werror -Wextra -Wpedantic -std=c99 -pedantic -lm)

set(MySources ptr_string.c parser.c iterator.c error.c deque.c)

add_library(ifj ${MySources})

add_executable(ifj19 main.c)

target_link_libraries(ifj19 ifj m)
