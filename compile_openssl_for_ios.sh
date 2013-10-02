#!/bin/sh
SDKVERSION="7.0"
TARGETVERSION="5.0"
ARCHS="i386 armv7 armv7s arm64"
CURRENTPATH=`pwd`
OPENSSL_PATH="${CURRENTPATH}/src"
BUILD_DIR="${CURRENTPATH}/build"
INSTALL_PATH="${CURRENTPATH}/lib"
XCODEPATH="/Applications/Xcode.app/Contents"
XCODEDEV=`xcode-select -print-path`

echo "Building OpenSSL with iOS SDK ${SDKVERSION} to target iOS ${TARGETVERSION}+ ..."

mkdir -p "$INSTALL_PATH"
mkdir -p "$BUILD_DIR"

cd "$OPENSSL_PATH"

SSL_LIBS="";
CRYPTO_LIBS="";

for ARCH in ${ARCHS}
do
	echo "... for architecture ${ARCH}"

	if [ "${ARCH}" == "i386" ];
	then
		PLATFORM="iPhoneSimulator"
	else
		PLATFORM="iPhoneOS"
	fi

	MINTARGET=$TARGETVERSION

	if [ "$ARCH" == "arm64" ] && [ "$TARGETVERSION" \< "7.0" ];
	then
		echo "(min target for arm64 is iOS 7.0)..."
		MINTARGET=7.0
	fi

	export CC="${XCODEDEV}/usr/bin/gcc -arch ${ARCH}"
	
	ARCHPATH="${BUILD_DIR}/iOS${SDKVERSION}-${ARCH}"
	mkdir -p "$ARCHPATH"
	LOG="${ARCHPATH}/build_openssl.log"
	
	./Configure BSD-generic32 --openssldir="${ARCHPATH}" > "${LOG}" 2>&1
	sed -ie "s!^CFLAG=!CFLAG=-isysroot ${XCODEPATH}/Developer/Platforms/${PLATFORM}.platform/Developer/SDKs/${PLATFORM}${SDKVERSION}.sdk -miphoneos-version-min=${MINTARGET} -all_load -ObjC !" "Makefile"

	make >> "$LOG" 2>&1
	make install >> "$LOG" 2>&1
	make clean >> "$LOG" 2>&1

	SSL_LIBS="${SSL_LIBS} ${ARCHPATH}/lib/libssl.a"
	CRYPTO_LIBS="${CRYPTO_LIBS} ${ARCHPATH}/lib/libcrypto.a"
done

echo "Build common library..."

lipo -create ${SSL_LIBS} -output "${INSTALL_PATH}/libssl.a"
lipo -create ${CRYPTO_LIBS} -output "${INSTALL_PATH}/libcrypto.a"

mkdir -p "${CURRENTPATH}/include"
cp -R "${ARCHPATH}/include/openssl" "${CURRENTPATH}/include/"

rm -rf "${BUILD_DIR}"

echo "Building done."
