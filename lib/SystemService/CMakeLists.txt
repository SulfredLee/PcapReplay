set (boostRootPath "C:\\boost\\boost_1_59_0")
# Collect sources into the variable SYSTEMSERVICE_SOURCES  
file (GLOB SYSTEMSERVICE_SOURCES
      "*.h"
      "*.cpp")

# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("SystemService" FILES ${SYSTEMSERVICE_SOURCES})

# Properties->C/C++->General->Additional Include Directories
include_directories (.
					 ${boostRootPath})

# Set Properties->General->Configuration Type to Dynamic Library(.dll)
# Creates SystemService.dll with the listed sources collected in the variables
# Also adds sources to the Solution Explorer
add_library(SystemService ${SYSTEMSERVICE_SOURCES})
# add_library(SystemService SHARED ${SYSTEMSERVICE_SOURCES})

# Creates a folder "libraries" and adds target project (SystemService.vcproj) under it
set_property(TARGET SystemService PROPERTY FOLDER "libraries")

# Adds logic to INSTALL.vcproj to copy SystemService.dll to the destination directory
install (TARGETS SystemService
		 ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/bin
		 RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
		 LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/bin)
