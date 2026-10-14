#!/bin/zsh
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#   run_tests.sh                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#   By: capapes <capapes@student.42.fr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#   Created: 2026/10/14 22:43:50 by capapes            #+#    #+#              #
#   Updated: 2026/10/14 22:43:50 by capapes           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


test_dir=${1:-bin/tests}
if [[ ! -d "$test_dir" ]]; then
	print -r -- "test directory does not exist: $test_dir"
	exit 1
fi

tests=()
while IFS= read -r -d '' test; do
	tests+=("$test")
done < <(find "$test_dir" -type f -name '*.test.run' -print0 | sort -z)

if (( ${#tests} == 0 )); then
	print -r -- "no test executables found in $test_dir"
	exit 1
fi

# Keep each suite's diagnostics together and continue after a failure.
failures=0
for test in "${tests[@]}"; do
	print -r -- "RUN: $test"
	"$test"
	test_status=$?
	if (( test_status == 0 )); then
		print -r -- "PASS: $test"
	else
		print -r -- "FAIL: $test (exit $test_status)"
		(( failures += 1 ))
	fi
done

print -r -- "${#tests} suites, $failures failed"
(( failures == 0 ))
