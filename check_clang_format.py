#!/usr/bin/env python3

import difflib
import logging
import subprocess
import sys

import _utils as utils


def check(path, check_id):
    with open(path) as f:
        original = f.read()
    formatted = subprocess.check_output(["clang-format", "-assume-filename", path],
                                        input=original, text=True)
    diff = difflib.unified_diff(original.splitlines(keepends=True),
                                formatted.splitlines(keepends=True),
                                path,
                                "formatted")
    diff_result = list(diff)

    def result(nr_checks):
        logging.info(f"[{check_id}/{nr_checks}] checked clang-format: {path}")
        if not diff_result:
            return True
        logging.warning("differences found:")
        sys.stderr.writelines(diff_result)
        print()
        return False

    return result


if __name__ == "__main__":
    utils.init()

    def filter(path):
        return (path.endswith(".cpp") or path.endswith(".h")) and not path.startswith("cmake/")
    utils.check_files(check, filter)
