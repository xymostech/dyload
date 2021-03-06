# Dynamic Loading in C

This is a library/framework for dynamically reloading files
into a running program using the capabilities of dynamically
linked libraries.

Using the provided files, it is easy to transform an
existing file into a dynamically loading one, which can then
be easily injected into a project, making it much easier to
test modificiations to code

### Benefits

- Test modified code more easily
- See changes much faster
- Easy to create static and dynamic versions of code

### Drawbacks

- Currently only supports linux systems with inotify
- Only flow of functions can be changed, data types cannot
- Harder to configure when compiling

## Usage

Simply run

    $ make
    $ make install

from this directory, and the library and all examples will
be built, and the library installed

## Examples

There are several provided examples showing how this code
can be used.

### Console Test

This test shows a simple printing function which is passed
in an integer and is called once every two seconds. The
printing function can by dynamically changed and re-loaded,
and the new printing function is called automatically.

To see this in action, run

    $ ./consoletest-dynamic

Then, modify the code in `a.c` while the program is running
and remake from within the console directory. The new code
will be automatically reloaded!

A static version of the code is also automatically built.
You can see this by running

    $ ./consoletest

### OpenGL Test

This test shows that dynamic loading works even in parallel
with large open libraries, without breaking code or forcing
a large reload. The main drawing function is dynamically
loaded, so any changes to the drawing are automatically
re-loaded and the new scene is re-drawn.

To see this, run

    $ ./gltest-dynamic

Then, modify the drawing code in `draw.c` while the program
is running, and see the new scene be redrawn automatically.

A static version of the code is also built, which can be
used by

    $ ./gltest
