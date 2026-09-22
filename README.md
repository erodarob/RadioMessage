# RadioMessage
A library for encoding and decoding messages related to radio communication.

## Building
Building RadioMessage is supported using GCC and the Arduino compiler. 

### Arduino
Building via the Arduino compiler will vary based on the build system used, but generally you will need to add the library to the correct folder in your project and include the ```RadioMessage.h``` header file wherever you would like to use RadioMessage. RadioMessage should then be compiled with your project the next time it is built.

### GCC
RadioMessage also supports GCC builds to run on desktop. Users on Windows should build RadioMessage using the GCC for MinGW. RadioMessage uses a CMake build system, and includes a ```CMakeLists.txt``` file in the root directory of the library that defines a CMake library called RadioMessage.

Include the library by including its CMake file in your project's main CMake file by adding:
```
include(${CMAKE_CURRENT_LIST_DIR}/../CMakeLists.txt)
```

Then, add RadioMessage as a dependency to the executables that need it just like any other library:
```
target_link_libraries(your-executable RadioMessage)
```

### Tests
RadioMessage includes a basic test suite to test whether RadioMessage's main functions are working properly. To build the test suite first make and enter the build folder:
```
mkdir build && cd build
```

Then generate the cmake project with:
```
cmake ../test
```

Then build the project with:
```
make
```

### Documentation
RadioMessage is documented with Doxygen. If not already done, create and enter the build folder:
```
mkdir build && cd build
```

Then, build the documentation via:
```
doxygen ../Doxyfile
```
Note: The documentation must be built from the ```/build``` folder.

HTML documentation will be generated that can be viewed from your web browser by opening ```/build/html/index.html```.

## Usage
To use RadioMessage, simply include the ```RadioMessage.h``` header in your file:
```
#include "RadioMessage.h"
```

The default size of the Message object buffer is 10 kB, but this can be changed using a macro. The following example changes the buffer to 1 kB.
```
#define MSG_SIZE_OVRD 1000
```

The typical method of encoding a Message involves creating a Data object, such as GenericData, filling it with some data to be encoded, and passing the Data object to a Message. A basic example of this is shown below:
```
#include "RadioMessage.h"

int main(int argc, char *argv[])
{
    Message m;
    GenericData d;
    uint8_t data[] = {1, 2, 3, 4, 5, 6};

    d.fill(data, sizeof(data));
    m.encode(&d);

    return 0;
}
```

To decode the data simply reverse the process:
```
#include "RadioMessage.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    // encode
    Message m;
    GenericData d;
    uint8_t data[] = {1, 2, 3, 4, 5, 6};

    d.fill(data, sizeof(data));
    m.encode(&d);

    // deocde
    GenericData dout;
    m.decode(&dout);
    
    // will print: 1 2 3 4 5 6
    for (int i = 0; i < dout.size; i++) 
    {
        printf("%d ", dout.data[i]);
    }

    return 0;
}
```