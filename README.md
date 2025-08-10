- Decode octrees into convenient structures from binary format and encode them back
- Find the difference between two batches to form `.optoctreepatch`

## Documentation
The documentation is quite short. [See here](https://maksimshchavelev.github.io/optoctreeparser/html/annotated.html)

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
