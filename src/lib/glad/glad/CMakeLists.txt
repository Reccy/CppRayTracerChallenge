cmake_minimum_required(VERSION 3.15.0)
project(glad)

add_library(glad STATIC glad.h khrplatform.h glad.c)
target_include_directories(glad PUBLIC ".")