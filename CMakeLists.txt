cmake_minimum_required (VERSION 2.8.12)

set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.6.0\\5.6\\msvc2013_64\\")
# set (boostRootPath "C:\\boost\\boost_1_59_0")

# set (CMAKE_PREFIX_PATH "I:\\WorkShop\\Software_Dev\\3rd_party\\Qt5.6.0\\5.6\\msvc2013_64\\")

# Maps to a solution file (SFToolsQT.sln). The solution will 
# have all targets (exe, lib, dll) as projects (.vcproj)
project (SFToolsQT)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set compiler flags and options. 
# Here it is setting the Visual Studio warning level to 4
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

# Command to output information to the console
# Useful for displaying errors, warnings, and debugging
message ("cxx Flags: " ${CMAKE_CXX_FLAGS})

# Sub-directories where more CMakeLists.txt exist
add_subdirectory(app/PcapReplayer)
add_subdirectory(lib/SystemService)
add_subdirectory(lib/ui/PcapReplayer)

# Turn on CMake testing capabilities
#enable_testing()

# Add test cases
#add_test(AppTest1 ${PROJECT_BINARY_DIR}/bin/app.exe 100)
#add_test(AppTest2 ${PROJECT_BINARY_DIR}/bin/app.exe 200)
#add_test(AppTest3 ${PROJECT_BINARY_DIR}/bin/app.exe 300)
