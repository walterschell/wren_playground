# wren_playground
Playground for testing possible new wren features

# Instructions
1. Update submodules `git submodule update --init --recursive`
2. `mkdir -p build && cd build && cmake .. && make`
3. `./wren_playground combined_demo.wren`

# What is being demoed
1. Ability to register a builtin wren module (readline)
2. Ability to load a pure wren module (uname)
3. Ability to load a dynamic wren plugin (posix)

# Notes / Caveots
1. No tests 
2. Uses and embeds a copy of khash (BSD License)
3. Function/Type names need some (lots of) cleanup
4. Linux Only
5. Needs porting to make from CMake
6. Needs documentation
7. Needs commenting

