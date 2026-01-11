#!/bin/bash

# Build script for Mansion Tycoon browser game

echo "==================================="
echo "Mansion Tycoon - Browser Build"
echo "==================================="

# Check for Emscripten
if ! command -v emcc &> /dev/null; then
    echo "ERROR: Emscripten not found!"
    echo "Please install Emscripten SDK:"
    echo "  git clone https://github.com/emscripten-core/emsdk.git"
    echo "  cd emsdk"
    echo "  ./emsdk install latest"
    echo "  ./emsdk activate latest"
    echo "  source ./emsdk_env.sh"
    exit 1
fi

echo "✓ Emscripten found"

# Create build directory
BUILD_DIR="build-web"
rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR

echo "✓ Build directory created"

# Configure with CMake
echo "Configuring with CMake..."
emcmake cmake -B $BUILD_DIR \
    -DCMAKE_BUILD_TYPE=Release \
    -DPLATFORM=EMSCRIPTEN

if [ $? -ne 0 ]; then
    echo "ERROR: CMake configuration failed!"
    exit 1
fi

echo "✓ Configuration complete"

# Build
echo "Building game..."
cmake --build $BUILD_DIR -j$(nproc)

if [ $? -ne 0 ]; then
    echo "ERROR: Build failed!"
    exit 1
fi

echo "✓ Build complete"

# Create distribution directory
DIST_DIR="dist"
rm -rf $DIST_DIR
mkdir -p $DIST_DIR

echo "✓ Distribution directory created"

# Copy files
echo "Copying files to distribution..."
cp index.html $DIST_DIR/
cp $BUILD_DIR/bin/MansionSim.js $DIST_DIR/ 2>/dev/null || echo "Warning: MansionSim.js not found"
cp $BUILD_DIR/bin/MansionSim.wasm $DIST_DIR/ 2>/dev/null || echo "Warning: MansionSim.wasm not found"
cp $BUILD_DIR/bin/MansionSim.data $DIST_DIR/ 2>/dev/null || true

echo "==================================="
echo "✓ Build Complete!"
echo "==================================="
echo ""
echo "To run the game:"
echo "  1. cd dist"
echo "  2. python3 -m http.server 8000"
echo "  3. Open http://localhost:8000 in your browser"
echo ""
