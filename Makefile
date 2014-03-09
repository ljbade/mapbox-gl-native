-include config.mk

BUILDTYPE ?= Release
PYTHON ?= python
V ?= 1

all: llmr

llmr: config.gypi src llmr.gyp
	$(MAKE) -C out BUILDTYPE=Release V=$(V) llmr

# build OS X app with pure make
app: config.gypi src macosx/llmr-app.gyp
	deps/run_gyp macosx/llmr-app.gyp -Goutput_dir=./out/ --depth=. --generator-output=./build/macosx-make -f make
	make -C build/macosx-make V=$(V)
	open build/macosx-make/out/Release/llmr.app

linux: config.gypi src linux/llmr-app.gyp
	deps/run_gyp linux/llmr-app.gyp -Goutput_dir=./out/ --depth=. --generator-output=./build/linux-make -f make
	make -C build/linux-make V=$(V)
	./build/linux-make/out/Release/llmr.app

ndk/android-ndk-r9c:
	mkdir -p ndk
	cd ndk && \
	curl -O http://dl.google.com/android/ndk/android-ndk-r9c-darwin-x86_64.tar.bz2 && \
	tar -xf android-ndk-r9c-darwin-x86_64.tar.bz2

ndk/android-ndk-r9c/active-platform: ndk/android-ndk-r9c
	export API_LEVEL="android-19" && \
	export ANDROID_CROSS_COMPILER="arm-linux-androideabi-clang3.3" && \
	export NDK_PATH=ndk/android-ndk-r9c && \
	export PLATFORM_PREFIX="$${NDK_PATH}/active-platform" && \
	"$${NDK_PATH}/build/tools/make-standalone-toolchain.sh"  \
	  --toolchain="$${ANDROID_CROSS_COMPILER}" \
	  --install-dir="$${PLATFORM_PREFIX}" \
	  --stl=gnustl \
	  --arch=arm \
	  --platform="$${API_LEVEL}"

android-osx: ndk/android-ndk-r9c/active-platform config.gypi src linux/llmr-app.gyp
	export CXX=arm-linux-androideabi-clang++ && \
	export CC=arm-linux-androideabi-clang && \
	export LD="arm-linux-androideabi-ld" && \
	export AR="arm-linux-androideabi-ar" && \
	export RANLIB="arm-linux-androideabi-ranlib" && \
	export NDK_PATH=ndk/android-ndk-r9c && \
	export PLATFORM_PREFIX="`pwd`/$${NDK_PATH}/active-platform/" && \
	export PATH=$$PATH:$${PLATFORM_PREFIX}/bin && \
	echo $$PATH && \
	export ANDROID_BUILD_TOP=$${PLATFORM_PREFIX} && \
	deps/run_gyp llmr.gyp -Goutput_dir=./out/ --depth=. -f android


# build OS X app with Xcode
lproj: config.gypi src linux/llmr-app.gyp
	deps/run_gyp linux/llmr-app.gyp -Goutput_dir=./out/ --depth=. --generator-output=./ -f xcode
	open ./linux/llmr-app.xcodeproj

# build just xcode project for libllmr
xcode: config.gypi llmr.gyp
	deps/run_gyp llmr.gyp -Goutput_dir=./out/ --depth=. --generator-output=./ -f xcode

# build OS X app with Xcode
xproj: config.gypi src macosx/llmr-app.gyp
	deps/run_gyp macosx/llmr-app.gyp -Goutput_dir=./out/ --depth=. --generator-output=./ -f xcode
	open ./macosx/llmr-app.xcodeproj

# build OS X app with xcodebuild
xapp: config.gypi src macosx/llmr-app.gyp
	deps/run_gyp macosx/llmr-app.gyp -Goutput_dir=./out/ --depth=. --generator-output=./ -f xcode
	xcodebuild -project ./macosx/llmr-app.xcodeproj
	open macosx/build/Release/llmr.app

# build iOS app with Xcode
iproj: config.gypi src ios/llmr-app.gyp
	deps/run_gyp ios/llmr-app.gyp -Goutput_dir=./out/ --depth=. --generator-output=./ -f xcode
	open ./ios/llmr-app.xcodeproj

# build iOS app with xcodebuild
iapp: config.gypi src ios/llmr-app.gyp
	deps/run_gyp ios/llmr-app.gyp -Goutput_dir=./out/ --depth=. --generator-output=./ -f xcode
	xcodebuild -project ./ios/llmr-app.xcodeproj
	# launch app with ios-sim?

isim: config.gypi src ios/llmr-app.gyp
	deps/run_gyp ios/llmr-app.gyp -Goutput_dir=./out/ --depth=. --generator-output=./ -f xcode
	xcodebuild -project ./ios/llmr-app.xcodeproj -arch i386 -sdk iphonesimulator
	# does not work
	#"/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/Applications/iPhone Simulator.app/Contents/MacOS/iPhone Simulator" -SimulateApplication ios/build/Release-iphonesimulator/llmr.app/llmr

clean:
	-rm -rf out
	-rm -rf build
	-rm -rf macosx/build
	-rm -rf ios/build
	-rm -rf llmr.xcodeproj
	-rm -rf macosx/llmr-app.xcodeproj
	-rm -rf ios/llmr-app.xcodeproj
	-rm -f llmr*.target.mk
	-rm -f GypAndroid.mk

distclean:
	-rm -f config.gypi
	-rm -f config.mk

test: all
	echo test

.PHONY: test linux
