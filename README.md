## Document Architecture

```bash
├── Makefile # the makefile for the project
├── Readme # this readme file
├── include # some header files or warpper files
│   └── GL
│       ├── freeglut.h
│       ├── freeglut_ext.h
│       ├── freeglut_std.h
│       └── glut.h
├── lib # the library files
│   ├── freeglut.lib
│   ├── glut.lib
│   └── glut32.lib
├── output # the output files(including some dll files to run the program)
│   ├── freeglut.dll
│   ├── glut.dll
│   ├── glut32.dll
│   ├── opengl32.dll
│   └── winmm.dll
└── src
    └── main.cpp # the source code of the project
```

## Compile & Run

Make sure that you have configured  `MinGW`.

You can use the following command to compile this project.

```bash
$ make all
```

You can use this command to directly run this project.

```bash
$ make run
```

If you want to clear the compiled product, you can use this command.

```bash
$ make clean
```

