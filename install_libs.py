#!/usr/bin/env python3

import logging
import os
import platform
import shutil
import subprocess
import sys
from distutils import dir_util
from pathlib import Path


VCPKG_REF = "2019.06"
VCPKG_LIBS = [
    # gfx
    "glad",
    "glfw3",

    "doctest",
    "fmt",
]


def is_win():
    return platform.system() == "Windows"


def install_vcpkg():
    logging.info("installing vcpkg")
    if Path("vcpkg/.git").exists():
        logging.warning("vcpkg seems already installed, trying to update")
        subprocess.check_call(["git", "-C", "vcpkg", "reset", "--hard"])
        subprocess.check_call(["git", "-C", "vcpkg", "clean", "-df"])
        subprocess.check_call(["git", "-C", "vcpkg", "fetch", "-p"])
        if (subprocess.check_output(["git", "-C", "vcpkg", "rev-parse", "HEAD"])
                == subprocess.check_output(["git", "-C", "vcpkg", "rev-parse", VCPKG_REF])):
            logging.warning("vcpkg seems already up-to-date, skipping")
            return
    else:
        subprocess.check_call(["git", "clone", "https://github.com/Microsoft/vcpkg"])
    subprocess.check_call(["git", "-C", "vcpkg", "checkout", VCPKG_REF])
    if is_win():
        subprocess.check_call([str(Path("vcpkg/bootstrap-vcpkg.bat")), "-disableMetrics"])
    else:
        subprocess.check_call([str(Path("vcpkg/bootstrap-vcpkg.sh")), "-disableMetrics", "-useSystemBinaries"])


def install_vcpkg_packages():
    logging.info("installing vcpkg libs")
    triplet = "x64-windows" if is_win() else "x64-linux"
    Path("vcpkg_config/ports").mkdir(exist_ok=True)
    vcpkg_cmd = [str(Path("vcpkg/vcpkg")),
                 f"--overlay-triplets={str(Path('vcpkg_config/triplets').resolve())}",
                 f"--overlay-ports={str(Path('vcpkg_config/ports').resolve())}",]
    subprocess.check_call(vcpkg_cmd + ["remove", "--outdated", "--no-purge", "--recurse"])
    subprocess.check_call(vcpkg_cmd + ["install", "--triplet", triplet] + VCPKG_LIBS)


def cleanup_vcpkg():
    logging.info("cleaning up vcpkg")
    vcpkg_path = Path("vcpkg")
    shutil.rmtree(vcpkg_path / "packages", True)
    for lib in (vcpkg_path / "buildtrees").glob("*"):
        for dir in lib.glob("*"):
            if dir.is_dir() and dir.name != "src":  # keep source for debugging
                shutil.rmtree(str(dir))
    for file in (vcpkg_path / "downloads").glob("*"):
        if file.is_file():
            file.unlink()


if __name__ == "__main__":
    logging.basicConfig(datefmt="%H:%M:%S",
                        format="[%(asctime)s][%(levelname)s] %(message)s",
                        level=logging.DEBUG,
                        stream=sys.stderr)
    os.chdir(str(Path(__file__).parent))

    logging.info(f"detected platform: {platform.system()}")

    install_vcpkg()
    install_vcpkg_packages()
    cleanup_vcpkg()
