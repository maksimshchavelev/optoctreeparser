This is a C++ library that allows you to work with `.optoctrees` (as well as `.optoctreepatch` for the [TerrainPatcher](https://github.com/Esper89/Subnautica-TerrainPatcher/tree/master) mod) landscape files in the game Subnautica. It can:
- Read and write `.optoctrees` and `.optoctreepatch` in binary format
- Decode octrees into convenient structures from binary format and encode them back
- Find the difference between two batches to form `.optoctreepatch`

## Documentation
The documentation is quite short. [See here](https://maksimshchavelev.github.io/optoctreeparser/html/)

## Connecting to your project
In CMakeLists.txt, write:
```.cmake 
include(FetchContent)

FetchContent_Declare(
    optoctreeparser
    GIT_REPOSITORY https://github.com/maksimshchavelev/optoctreeparser.git
    GIT_TAG        v1.0.0
)

FetchContent_MakeAvailable(optoctreeparser)

# Link with your target
target_link_libraries(YourApp PRIVATE optoctreeparser)
```
