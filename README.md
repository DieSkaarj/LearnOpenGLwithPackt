# LearnOpenGLwithPackt
Learn OpenGL Exercises from the book by Frahaan Hussain for Packt. Tweaked a bit to use c++11 features.

## Compile
Advised: using msys2 Mingw64 environment.
Required: put soil2.dll in base directory.
Compile: individual Chapters with make e.g. make Chapter2-3

Some exercises have alternate versions, you can use relative preprocessor directives to compile which one you want to see, however reviewing the source file first is advised as to what the flags actually are. You can use the DEFINE flags with make to set these e.g. make Chapter3-2 DEFINE=-DPERSPECTIVE
