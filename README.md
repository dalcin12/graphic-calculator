# Graphic Calculator in C

This is a basic Graphic Calculator in C. Still in tests. Made with Raylib.

To set the function, you need to change it in the code. Implementing a better way to do it soon.

## LINUX

### To compile, use this command in the same directory:

<code>gcc -o plot plot.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11</code>

You can also include the <code>-Wextra -Wall</code> for extra warnings.

Finally, execute the program using <code>./plot</code>.

## WINDOWS
### To compile, use this command instead:

<code>gcc -o plot plot.c -I include -L lib -lraylib -lgdi32 -lwinmm</code>

To run, use <code>./plot</code>.

