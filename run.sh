nw-gyp build --target=0.22.3
cp build/Release/addon.node nwjs_addon/nwfile
nw nwjs_addon/nwfile
