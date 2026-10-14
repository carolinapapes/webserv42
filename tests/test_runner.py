#!/usr/bin/env python3
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#   test_runner.py                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#   By: capapes <capapes@student.42.fr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#   Created: 2026/10/14 23:00:00 by capapes            #+#    #+#              #
#   Updated: 2026/10/14 23:00:00 by capapes           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

"""Exercise real assertion and shell exit codes inside the Linux container."""

from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parent.parent
ASSERTIONS = ROOT / "bin/tests/http/request/assertions.test.run"


class RunnerTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix="webserv-tests-")
        self.addCleanup(self.temporary.cleanup)
        self.directory = Path(self.temporary.name)

    def executable(self, name, script):
        path = self.directory / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text("#!/bin/sh\n" + script + "\n")
        path.chmod(0o755)

    def run_suites(self, directory=None):
        return subprocess.run(
            ["zsh", str(ROOT / "run_tests.sh"), str(directory or self.directory)],
            cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
            text=True, timeout=20,
        )

    def test_passing_suites_return_zero(self):
        self.executable("first.test.run", "exit 0")
        self.executable("second.test.run", "exit 0")
        result = self.run_suites()
        self.assertEqual(result.returncode, 0, result.stdout)
        self.assertIn("2 suites, 0 failed", result.stdout)

    def test_failure_is_propagated_and_later_suites_still_run(self):
        self.executable("a_failure.test.run", "exit 7")
        self.executable("z_success.test.run", "echo later-suite-ran\nexit 0")
        result = self.run_suites()
        self.assertEqual(result.returncode, 1, result.stdout)
        self.assertIn("(exit 7)", result.stdout)
        self.assertIn("later-suite-ran", result.stdout)
        self.assertIn("2 suites, 1 failed", result.stdout)

    def test_empty_directory_is_a_failure(self):
        result = self.run_suites()
        self.assertEqual(result.returncode, 1, result.stdout)
        self.assertIn("no test executables found", result.stdout)

    def test_missing_directory_is_a_failure(self):
        result = self.run_suites(self.directory / "missing")
        self.assertEqual(result.returncode, 1, result.stdout)
        self.assertIn("test directory does not exist", result.stdout)

    def test_paths_containing_spaces(self):
        self.executable("nested directory/passing suite.test.run", "exit 0")
        result = self.run_suites()
        self.assertEqual(result.returncode, 0, result.stdout)
        self.assertIn("1 suites, 0 failed", result.stdout)

    def test_signal_termination_is_a_failure(self):
        self.executable("signal.test.run", "kill -TERM $$")
        result = self.run_suites()
        self.assertEqual(result.returncode, 1, result.stdout)
        self.assertIn("1 suites, 1 failed", result.stdout)

    def test_each_incorrect_request_field_exits_nonzero(self):
        fields = ("status", "method", "target", "version", "headers",
                  "missing-header", "extra-header", "body")
        for field in fields:
            with self.subTest(field=field):
                result = subprocess.run(
                    [str(ASSERTIONS), field], cwd=ROOT, stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT, text=True, timeout=20,
                )
                self.assertEqual(result.returncode, 1, result.stdout)
                self.assertIn(field, result.stdout)
                self.assertIn("1 failures", result.stdout)

    def test_matching_request_fields_exit_zero(self):
        result = subprocess.run(
            [str(ASSERTIONS)], cwd=ROOT, stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT, text=True, timeout=20,
        )
        self.assertEqual(result.returncode, 0, result.stdout)
        self.assertIn("0 failures", result.stdout)


if __name__ == "__main__":
    unittest.main(verbosity=2)
