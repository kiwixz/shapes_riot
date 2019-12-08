#!/usr/bin/env python3

import logging
import os
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor


def init():
    logging.basicConfig(datefmt="%H:%M:%S",
                        format="[%(asctime)s.%(msecs)d][%(levelname)s][%(thread)d] %(message)s",
                        level=logging.DEBUG,
                        stream=sys.stderr)
    os.chdir(os.path.dirname(os.path.abspath(__file__)))


def check_files(check, filter=None):
    git_files = subprocess.check_output(["git", "ls-files"], text=True).splitlines()
    with ThreadPoolExecutor(os.cpu_count() + 2) as executor:
        results = []
        count = 1
        for path in git_files:
            if filter is not None and not filter(path):
                continue
            results.append(executor.submit(check, path, count))
            count += 1

        nr_checks = count - 1
        count = 0
        exit_code = 0
        for future in results:
            if not future.result()(nr_checks):
                exit_code = 1
        exit(exit_code)
