#!/bin/bash

RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
RESET="\033[0m"
i=1

tests=(
	"1 2 3"
	"1 2 3 4 5 6"
	"-1 2 3 4"
	"1 word 3 4"
	"1 2 '' 4"
	"1 2 3 2147483648"
)

for t in "${tests[@]}"
do
	printf "${YELLOW}Test %d: ./prog %s${RESET}\n" "$i" "$t"
	i=$((i + 1))

	eval ./philo $t

	if [ $? -eq 1 ]; then
		printf "${GREEN}Result: OK${RESET}\n\n"
	else
		printf "${RED}Result: ERROR${RESET}\n\n"
	fi
done
