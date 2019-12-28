cmake_minimum_required(VERSION 3.12)
include("third_party")

third_party("fmt" "fmtlib/fmt" "6.0.0")

get_target_property(inc "fmt" INTERFACE_INCLUDE_DIRECTORIES)
set_target_properties("fmt" PROPERTIES
    INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${inc}"
)
