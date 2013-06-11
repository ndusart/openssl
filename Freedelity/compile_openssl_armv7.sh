#!/bin/sh

# Compile the OpenSSL source for the iPad

SDKVERSION="6.1"
TARGETVERSION="5.0"
XCODEPATH="/Applications/Xcode.app/Contents"
CURRENTPATH=`pwd`

echo "Building OpenSSL with iOS SDK ${SDKVERSION} for iOS ${TARGETVERSION}..."

export CC="${XCODEPATH}/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc -arch armv7"

FINALDIR="${CURRENTPATH}/iOS${SDKVERSION}-armv7"
LOG="${FINALDIR}.log"
mkdir -p ${FINALDIR}

cd ..
./Configure BSD-generic32 --openssldir="${FINALDIR}" >> "${LOG}" 2>&1
sed -ie "s!^CFLAG=!CFLAG=-isysroot ${XCODEPATH}/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS${SDKVERSION}.sdk -miphoneos-version-min=${TARGETVERSION} -all_load -ObjC !" "Makefile"

make >> "${LOG}" 2>&1
make install >> "${LOG}" 2>&1
make clean >> "${LOG}" 2>&1

echo "Build library..."
lipo -create ${FINALDIR}/lib/libssl.a -output ${CURRENTPATH}/libssl.a
lipo -create ${FINALDIR}/lib/libcrypto.a -output ${CURRENTPATH}/libcrypto.a

mkdir -p ${CURRENTPATH}/include
cp -R ${FINALDIR}/include/openssl ${CURRENTPATH}/include/

echo "Building done"
