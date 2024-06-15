#!/bin/sh

source scripts/setup_emscripten_sdk.sh

emcmake cmake . -B build-web
cmake --build build-web -j15
cp -rv web/* build-web
cp -rv build-web/examples/Playground/Playground* build-web
cp -rv build-web/examples/Calculator/Calculator* build-web
cp -rv build-web/examples/Widget/ExampleWidget* build-web
cp -rv build-web/examples/Renderer2D/ExampleRenderer2D* build-web
