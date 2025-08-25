# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(temp-logger)

target_include_directories(app PRIVATE include)

target_sources(app PRIVATE 
    src/main.c
    src/sensorsHandler.c
    src/temperature.c
    src/timers.c
    src/ledHandler.c
    src/buttonHandler.c
    src/flashHandler.c
)
