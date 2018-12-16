# nCine
nCine is a multi-platform 2D game engine.  
It is released under the MIT License, Copyright (c) 2011-2018 Angelo Theodorou.  
For additional information: https://ncine.github.io

## Information

### Dependencies
- GLEW
- GLFW 3.x
- SDL 2
- libpng
- libwebp
- OpenAL-soft
- libogg, libvorbis, libvorbisfile
- Lua 5.3
- Dear ImGui

### Supported Platforms and Compilers
- Windows (MSVC, MinGW-w64 on MSYS2)
- Mac Os X (Clang)
- Linux (GCC, Clang)
- Android (GCC, Clang)

### Development Tools
- git
- CMake
- Qt Creator
- Doxygen with GraphViz
- Valgrind
- Cppcheck
- Artistic Style and Uncrustify
- Google Test and Gcovr
- Tracy frame profiler

## Development

### Conventions

#### Coding Conventions
- Indent with tabs (4 spaces) but use spaces for continuation line alignment
- Allman brackets
  - No brackets around one line conditional statements
- Padding space after parenthesis headers (`if`, `do`, `while`, `for`, `switch`)
- Align pointer and reference operator to the variable or function name
- `inline` keyword always before `virtual` and `static`
- Mark an override method with the `override` specifier and remove `virtual`
- Access specifiers order: `public`, `protected`, `private`
  - One half indent for access specifiers (2 spaces)
- Declarations order: typedefs and enums, data members, constructors, destructors, methods
  - Friends defined in the private section of a class, after everything else
- One space padding around operators
- Use of the `explicit` keyword all the times it makes sense

#### Naming Conventions
- Pascal case for classes, enumerations, typedefs and constants
- Camel case for variables, functions and parameters
- All upper case for enumerators in an enumeration

#### Documenting Conventions
- Put Doxygen brief descriptions in header files and additional documentation in implementation ones
- Use Qt style for Doxygen detailed descriptions (`/*! */`) and end them with a period
