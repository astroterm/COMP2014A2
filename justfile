cxx := "c++"
cxx_ver := "26"
flags := "-std=c++" + cxx_ver + " -Wall -Wextra -pedantic"

run: build
    target/main

build:
    {{cxx}} {{flags}} src/*.cpp -Iinclude -o "target/main"
