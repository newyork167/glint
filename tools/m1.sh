rm ~/.conan/profiles/m1
conan profile new m1 --detect
sed -i '' 's/libstdc++/libc++/' ~/.conan/profiles/m1
#conan install -if . conanfile.py --profile:build default --profile:host m1 --build=missing