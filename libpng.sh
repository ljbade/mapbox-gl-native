mkdir -p libpngtmp

echo "setting platform ${PLATFORM_PREFIX}"
export PATH="${PLATFORM_PREFIX}/bin":$PATH
cd libpngtmp


# wget "http://prdownloads.sourceforge.net/libpng/libpng-1.6.9.tar.gz?download" -O libpng-1.6.9.tar.gz
# tar xzvf "libpng-1.6.9.tar.gz"
cd libpng-1.6.9
./configure \
    --host=arm-linux-androideabi \
    CC=arm-linux-androideabi-clang \
    LD=arm-linux-androideabi-ld \
    STRIP=arm-linux-androideabi-strip \
    --prefix=$PLATFORM_PREFIX \
    AR=arm-linux-androideabi-ar \
    RANLIB=arm-linux-androideabi-ranlib
make
make install
cd ../../
cp $PLATFORM_PREFIX/lib/libpng* jni/
