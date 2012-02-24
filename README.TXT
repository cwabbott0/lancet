To compile:

qmake-qt4 ; make

Prerequisites:

adb set up
mali_compile compiled from my compile.c as /data/limare/compile

To use:

Drop a shader in the editor window. In the 'Subs' window, add a
string-separated list of values, e.g. '0.0 1.0'. Wherever in the
shader there is a $, it will be substituted with each value in turn.
The 'args' line gets passed on to male_compile, so it's -v vertex
or -f fragment shader.

Hit 'Go'. In the Diff window there will be a side-by-side comparison
of the shader binary with each input value. 'New diff' will spawn
additional diff windows so you can look at the data in multiple ways at once.

