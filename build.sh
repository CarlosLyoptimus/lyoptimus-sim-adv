#!/bin/bash
# Install Python dependencies
pip install flask flask-cors pytz

# Always recompile to ensure compatibility with Render's system libraries
echo "Compiling advanced C++ executable..."
rm -f kv_1d_dried_layer_adv
g++ -O2 -o kv_1d_dried_layer_adv Kv_1d_dried_layer_adv.cpp -lm
echo "Compilation done."
