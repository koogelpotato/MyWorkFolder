
# Homework 2

built three folders each one corresponding to their builds

## Build-static

`static/build`

built with SDL3 static linking **note: SDL 3 will replace library linking with dynamic version instead of static for more information refer to this link:** [Dynamic API](https://github.com/libsdl-org/SDL/blob/main/docs/README-dynapi.md)

## Build-dynamic

`dynamic/build`

built with SDL3 dynamic linking


## **TASK 4**

Debugged in gdb, debug calls are as follows:

```
Loaded '/usr/local/lib/libSDL3.so.0'. Symbols loaded.
Loaded '/lib/x86_64-linux-gnu/libstdc++.so.6'. Symbols loaded.
Loaded '/lib/x86_64-linux-gnu/libc.so.6'. Symbols loaded.
Loaded '/lib/x86_64-linux-gnu/libm.so.6'. Symbols loaded.
Loaded '/lib/x86_64-linux-gnu/libgcc_s.so.1'. Symbols loaded.
```

**First call:** 

```
Loaded '/usr/local/lib/libSDL3.so.0'. Symbols loaded
```


## Build-fun


**WARNING: UNTESTED BUILD**

PS VITA vpk with dynamicaly linked SDL3 


## Build-static-sub

A build that substitutes a static library for dynamic library(Check `CMakeLists.txt`)
