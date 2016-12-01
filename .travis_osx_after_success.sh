set -e

echo "BUILD_OSX_ARCH=${BUILD_OSX_ARCH}"
CXXFLAGS="${BUILD_OSX_ARCH}" CFLAGS="${BUILD_OSX_ARCH}" LDFLAGS="${BUILD_OSX_ARCH}" $PYTHON_EXE setup.py bdist_wheel

# This copies in the .dylib files that are linked to the .so files.
# It also rewrites the .so file linking options. https://pypi.python.org/pypi/delocate
delocate-wheel -v dist/*.whl


$PYTHON_EXE .travis_osx_upload_whl.py --no-config

# Test the created wheel

# uninstall everything
brew uninstall --force --ignore-dependencies sdl
brew uninstall --force --ignore-dependencies sdl_image
brew uninstall --force --ignore-dependencies sdl_mixer
brew uninstall --force --ignore-dependencies sdl_ttf
brew uninstall --force --ignore-dependencies smpeg
brew uninstall --force --ignore-dependencies jpeg
brew uninstall --force --ignore-dependencies libpng
brew uninstall --force --ignore-dependencies libtiff
brew uninstall --force --ignore-dependencies webp
brew uninstall --force --ignore-dependencies flac
brew uninstall --force --ignore-dependencies fluid-synth
brew uninstall --force --ignore-dependencies libmikmod
brew uninstall --force --ignore-dependencies libvorbis
brew uninstall --force --ignore-dependencies smpeg
brew uninstall --force --ignore-dependencies portmidi
brew uninstall --force --ignore-dependencies freetype

# Create new virtualenv
$PIP_CMD install virtualenv
$PYTHON_EXE -m virtualenv -p $PYTHON_EXE venv
source venv/bin/activate
python --version
which python
pip --version

# Install wheel
pip install dist/*.whl

# Run test
python -m pygame.tests.__main__ --exclude opengl --time_out 300
