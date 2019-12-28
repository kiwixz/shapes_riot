cmake_minimum_required(VERSION 3.12)
include("third_party")

third_party("fmt" "fmtlib/fmt" "6.0.0")
system_include_dirs("fmt")
