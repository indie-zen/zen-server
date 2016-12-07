

## V8

V8 depends on the libv8.so, which isn't built by default.  To build it, do the following after you've checked out the source code.

```
gn gen out.gn/x64.release --args='is_component_build=true'
ninja -C out.gn/x64.release
```

(or after ninja, does `make native library=shared` work?)