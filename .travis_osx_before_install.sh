# Compiles pygame on homebrew for distribution.
# This may not be what you want to do if not on travisci.

set -e

echo -en 'travis_fold:start:brew.update\\r'
brew update
echo -en 'travis_fold:end:brew.update\\r'
export HOMEBREW_NO_AUTO_UPDATE=1

# read -d '' PATCH_BREW <<EOF
# diff --git a/Library/Homebrew/brew.sh b/Library/Homebrew/brew.sh
# index 9295300..ddd1e26 100644
# --- a/Library/Homebrew/brew.sh
# +++ b/Library/Homebrew/brew.sh
# @@ -96,7 +96,7 @@ then
#    HOMEBREW_SYSTEM="Macintosh"
#    # This is i386 even on x86_64 machines
#    [[ "\$HOMEBREW_PROCESSOR" = "i386" ]] && HOMEBREW_PROCESSOR="Intel"
# -  HOMEBREW_MACOS_VERSION="\$(/usr/bin/sw_vers -productVersion)"
# +  HOMEBREW_MACOS_VERSION="10.9"
#    HOMEBREW_OS_VERSION="macOS \$HOMEBREW_MACOS_VERSION"
#
#    printf -v HOMEBREW_MACOS_VERSION_NUMERIC "%02d%02d%02d" \${HOMEBREW_MACOS_VERSION//./ }
# EOF

#(cd /usr/local/Homebrew && git apply <<< "${PATCH_BREW}")

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

if [[ ${BUILD_OSX_ARCH} == *"-arch i386"* ]]; then
  UNIVERSAL_BUILD='--universal'
else
  UNIVERSAL_BUILD=''
fi

BUILD_FROM_SOURCE=''
#BUILD_FROM_SOURCE='--build-from-source'

brew install sdl ${UNIVERSAL_BUILD} ${BUILD_FROM_SOURCE}
brew install jpeg ${UNIVERSAL_BUILD} ${BUILD_FROM_SOURCE}
brew install libpng ${UNIVERSAL_BUILD} ${BUILD_FROM_SOURCE}
brew install libtiff ${UNIVERSAL_BUILD} ${BUILD_FROM_SOURCE} --with-xz
brew install webp ${UNIVERSAL_BUILD} ${BUILD_FROM_SOURCE} --with-libtiff --with-giflib
brew install libogg ${UNIVERSAL_BUILD} ${BUILD_FROM_SOURCE}
brew install libvorbis ${UNIVERSAL_BUILD} ${BUILD_FROM_SOURCE}
brew install flac ${UNIVERSAL_BUILD} ${BUILD_FROM_SOURCE} --with-libogg
brew install fluid-synth ${BUILD_FROM_SOURCE}
brew install libmikmod ${UNIVERSAL_BUILD} ${BUILD_FROM_SOURCE}
brew install smpeg ${BUILD_FROM_SOURCE}
brew install portmidi ${UNIVERSAL_BUILD} ${BUILD_FROM_SOURCE}
brew install freetype ${UNIVERSAL_BUILD} ${BUILD_FROM_SOURCE}
brew install sdl_ttf ${UNIVERSAL_BUILD} ${BUILD_FROM_SOURCE}
brew install sdl_image ${UNIVERSAL_BUILD} ${BUILD_FROM_SOURCE}
brew install sdl_mixer ${UNIVERSAL_BUILD} ${BUILD_FROM_SOURCE} --with-flac --with-fluid-synth --with-libmikmod --with-libvorbis --with-smpeg
