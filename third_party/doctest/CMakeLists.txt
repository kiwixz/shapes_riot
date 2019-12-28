cmake_minimum_required(VERSION 3.12)
include("third_party")

third_party("doctest" "onqtam/doctest" "2.3.6")
system_include_dirs("doctest")
