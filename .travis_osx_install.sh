
# ls -la /usr/local/lib/
# ls -la /usr/local/include/
# cat Setup
# otool -L `find . -name imageext.so`
# otool -L `find . -name image.so`
# otool -L /usr/local/opt/sdl_image/lib/libSDL_image-1.2.0.dylib


# Removes any X11R6 stuff so that png headers are not picked up by accident.
sed 's/-I\/usr\/X11R6\/include //g' Setup > Setup.new
mv Setup.new Setup

# Makes for i386 and x86_64.
CXXFLAGS="${BUILD_OSX_ARCH}" CFLAGS="${BUILD_OSX_ARCH}" LDFLAGS="${BUILD_OSX_ARCH}" $PYTHON_EXE setup.py install -noheaders
