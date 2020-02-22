# Using [Gist][] from AlloLib

    git clone https://github.com/kybr/Gist
    cd Gist
    make

Execute the commands above in this folder to get Karl's fork of [Gist][] and build a static library. Next, run `./run.sh path/to/use-gist/main.cpp` from [allolib_playground][].

Have a look at `flags.cmake` in this folder. This is one way we can build against non-AlloLib c++ libraries.


[Gist]: https://github.com/adamstark/Gist
[allolib_playground]: https://github.com/AlloSphere-Research-Group/allolib_playground
