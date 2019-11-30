#! /bin/sh
if [ $# -eq 0 ]; then
printf "simple_add\n"
./ic19int simple_add
printf "\n\n"

printf "add_with_return\n"
./ic19int add_with_return
printf "\n\n"

printf "if_elif_else_condition\n"
./ic19int if_elif_else_condition
printf "\n\n"

printf "simple_while\n"
./ic19int simple_while
printf "\n\n"

printf "advanced_while_with_if\n"
./ic19int advanced_while_with_if

elif [ $# -eq 1 ] && [ $1 == "-code" ]; then
printf "simple_add\n"
cat simple_add
printf "\n\n"

printf "add_with_return\n"
cat add_with_return
printf "\n\n"

printf "if_elif_else_condition\n"
cat if_elif_else_condition
printf "\n\n"

printf "simple_while\n"
cat simple_while
printf "\n\n"

printf "advanced_while_with_if\n"
cat advanced_while_with_if

else printf "RUN WITHOUT ARGUMENTS OR WITH ARGUMENT -code\n"
fi
