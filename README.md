# miniGL

_miniGL_ is a set of C++ classes aiming at creating a small openGL based rendering engine.
The idea behind it was simply for me to learn a little more about openGL. **All the code is based on the tutorials provided by [ogldev](http://ogldev.atspace.co.uk "ogldev tutorials"). All the assets in the resource directory also come from ogldev. Some of them were modified to fit the current implementation.**

## Illustration


## Project structure
The project is divided in two main parts:

* miniGL which contains all the code
* miniGL_test which contains unit tests for some of the classes of miniGL (mainly the ones related to linear algebra). The tests are written using [googletest](https://github.com/google/googletest "google test").


## External dependencies
This project requires the following external libraries:

   - [ImageMagick](https://www.imagemagick.org/script/index.php "image magick")
   - [Assimp](http://www.assimp.org/ "Open Asset Import Library")
   - [GLFW3](https://www.glfw.org/ "glfw")
   - [GLFX](https://github.com/maizensh/glfx "glfx")
   - [Ant Tweak Bar](http://anttweakbar.sourceforge.net/doc/ "Ant Tweak Bar")


## Build and run on macOs
Image magick, Assimp and GLFW3 can be installed using [brew](https://brew.sh/ "brew") whereas glfx and Ant Tweak Bar (ATB) have to be downloaded and installed manually.

### Install external dependencies with brew
Image magick, Assimp and GLFW3 can be installed using [brew](https://brew.sh/ "brew"):

`brew install imageMagick`

`brew install assip`

`brew install glfw3`

### Install external dependencies manually
ATB and glfx that are installed manually in the **external** directory.

`cd minigl; mkdir external`

##### Ant Tweak bar
1. Download and extract ATB in the **external** directory
2. I had to do some modifications to the source code to be able to compile it. In the **src** directory, edit the following files:
	- file: LoadOGLCore.h
	    - function: _glMultiDrawElements_ (line 149)
	    - modification: add **const** ot the fourth parameter
			- `ANT_GL_CORE_DECL(void, glMultiDrawElements, (GLenum mode, const GLsizei *count, GLenum type, const GLvoid* const *indices, GLsizei primcount))`
        - function: _glShaderSource_ (line 214)
		- modification: add **const** to the third parameter
			- `ANT_GL_CORE_DECL(void, glShaderSource, (GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length))`
    - file: LoadOGLCore.cpp
		- function: _NSGLCoreGetProcAddressNew_ (line 487)
		- modification: provide an absolute path for OpenGL
			- `gl_dyld = dlopen("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL",RTLD_LAZY);`
	- file: LoadOGL.cpp
		- function: _NSGLGetProcAddressNew_ (line 503)
		- modification: provide an absolute path for OpenGL
			- modified: `gl_dyld = dlopen("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL",RTLD_LAZY);`
			
3. Build ATB
    - Go to the source directory and build the library using the osx makefile: `make -f makefile.osx`
	- You should now see 2 new files in the **lib** directory: **libAntTweakBar.a** and **libAntTweakBar.dylib** 

##### GLFX
1. Clone glfx in the **external** directory.
	- `cd minigl/external``
	- `git clone https://github.com/maizensh/glfx.git`
2. Fix the path to OpenGL
	- `cd glfx/src`
	- Use your favourite text editor to edit the following file
		- file: CMakeLists.txt
	    	- function: _target_link_libraries_ (line 42)
	    	- modification: replace **"GL"** with **${OPENGL_LIBRARIES}**
				- `target_link_libraries( glfx ${OPENGL_LIBRARIES} "GLEW" )`
3. Build GLFX
	- Go back the build directory to build the library
		- `cd ..`
		- `cmake ..;make -j8`
	- You should now see 2 new files in the **src** directory (within the build directory): **libglfx_static.a** and **libglfx.dylib** 

### Build miniGL with CMake
1. Set current version in FindGLFW3.cmake
    - Go to the cmake directory: `cd minigl/cmake`
    - Open the FindGLFW3.cmake file with your favourite text editor
    - Set the **current_version** variable to your version of glfw3 (previously installed using brew)
2. Create a build directory in miniGL
	- `cd ..;mkdir build`
3. Build miniGL
	- Build a Xcode project
		- `cd build`
		- `cmake -G Xcode ..`
		- `open miniGL.xcodeproj`
		- Press "play" with the "ALL_BUILD" target
	- Build directly from the terminal
		- `cd build`
		- `cmake ..; make -j8`

### Run miniGL
1. To run miniGL on Xcode, make sure the **Working Directory** points to the **resources** directory
	- In `Edit scheme...` -> Run -> Options: Select the option **Use custom working directory** and specify the path to the **resources** folder in the build directory containing the Xcode project.
2. To run miniGL directly from the terminal, run the executable from the resources directory (so that the current working directory is the resources directory. This is necessary to access all the files in the resources directory)
	- `cd resources`
	- `../miniGL`

### Run miniGL_test
1. To run miniGL_test on Xcode, select the specific target and run.
2. To run miniGL_test directly from the terminal, use the executable in the test folder
	- `cd test`
	- `./miniGL_test`


## Build and run on Windows (using Microsoft Visual Studio)
You probably have to download and install all the external dependencies manually. The dependencies that provide pre-built libraries do so in _release_ in general. In the following, we will only consider setting up the Visual Studio Solution in _release_.

### Install external dependencies manually

#### ImageMagick
You can download an installer which provides `CORE_RL_Magick++_.lib`

#### Assimp
You can download an installer which provides `assimp-vc140-mt.lib`
	   
#### GLFW3
You can use CMake to configure a Visual Studio Solution with the CMakeLists.txt provided
 
#### GLFX
A Visual Studio solution is provided to generate libraries.
- Remark: When I built it, I had to fix an error in glfx.cpp where it couldn't find the _max_ function. I added the `<algorithm>` header to fix it.

#### Ant Tweak Bar
`AntTweakBar64.lib` is already provided

#### Google test
You can clone the project and use CMake to configure a Visual Studio Solution with the CMakeLists.txt provided

### Build miniGL with CMake
You can use CMake-GUI to configure the Visual Studio Solution

1. _Where is the source code_: select the miniGL directory
2. _Where to build the binaries_: create a build directory in miniGL and select it
3. Click _Configure_
4. Fill the different paths for the **libraries** and **include directories** for each external dependency. 
	- For Assimp, you need to include both the path to the include directory and also the path to the assimp folder in the include directory. For example: `C:/Program Files/Assimp/include;C:/Program Files/Assimp/include/assimp`
5. Click _Generate_ to generate the Visual Studio solution. 
6. Click _Open Project_ to open the solution

### Run miniGL
1. To run miniGL, make sure the **Working Directory** points to the **resources** directory
	- In the miniGL project properties, under Configuration Properties / Debugging, set Working Directory to **$(ProjectDir)resources**
2. You will also need to make sure the executable can access all the relevant _.dll_ (corresponding to the libraries provided to CMake). One quick way to do so is to copy them all next to your executable.

### Run miniGL_test
1. To run miniGL_test, make sure the **Working Directory** points to the **Release** directory
	- In the miniGL_test project properties, under Configuration Properties / Debugging, set Working Directory to **$(ProjectDir)Release**

## Usage

### Keyboard shortcuts
1. Camera 
	- Move left: **left** arrow 
	- Move right: **right** arrow
	- Move forward: **up** arrow
	- Move backward: **down** arrow
	- Move up: **n** key
	- Move down: **m** key
2. Tessellation factor for terrain
	- increase: **=** key
	- decrease: **-** key
3. Tessellation factor for Suzanne (Blender's mascot character)
	- increase: **t** key
	- decrease: **y** key
4. Toogle the wireframe mode: **z** key
5. Select a triangle on the spiders in the 3D picking example: maintain **ctrl** and hover with mouse

### Bugs
1. This code was mainly written on macOS with Xcode. Whilst it is possible to use it on windows as well, there are a few bugs that I have seen while testing: some of the scenes are not rendered correcly resulting in a black screen. 
I unfortunately don't have time to fix those scenes in windows.
2. In the _multipass shadow map_ example, the shadow is not computed correctly when moving the light away from the center position
3. In the _particles_ example, some particles are created at the origin (0,0,0) when they should not be

## Code documentation
The code is documented to be able to use [doxygen](http://www.doxygen.nl/ "doxygen").

1. Download and intall doxygen 
2. Start doxygen 
3. Go to File -> Open and choose `Doxyfile_Core.in`. Once loaded the doxygen project should be configurated  to generate the documentation 
4. Go to the `Run` tab and click `Run doxygen` 
