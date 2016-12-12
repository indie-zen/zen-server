
## V8

Zen Server currently requires V8 version 5.6.326.25 or greater.

For V8, Zen Server depends on the libv8.so, which isn't built by default.  To build it, do the following after you've checked out the source code.

To do a static build
```
tools/dev/v8gen.py x64.release
ninja -C out.gn/x64.release
```

To do a shared build (Zen Server needs a shared build right now)
```
gn gen out.gn.shared/x64.release --args='is_component_build=true'
ninja -C out.gn.shared/x64.release
```

Eventually this dependency will be removed (See issue #7)
