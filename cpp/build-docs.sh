#!/bin/sh

cmake -Sdocumentation -Bbuild
cmake --build build --target GenerateDocs