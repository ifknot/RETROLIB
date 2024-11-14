# RETROLIB
![logo](https://cldup.com/DL5x924VJS.jpg)

I use [Linux Mint](https://linuxmint.com/) with [Zed](https://zed.dev/) and [CMake](https://cmake.org/) to compile [DOS](https://en.wikipedia.org/wiki/DOS) programs with [Open Watcom v2](https://github.com/open-watcom/open-watcom-v2) 

The compiler supports C89/C90 standards by default. But... 
Open Watcom supports partial compatibility with the C99 standard. It implements the most commonly used parts of the standard. However, they are enabled only through the undocumented command-line switch "-za99". Three C99 features have been bundled as C90 Extension since pre-v1.0: C++ style comments (//), flexible array members, trailing comma allowed in enum declaration.
