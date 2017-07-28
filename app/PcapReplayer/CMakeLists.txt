# CMakeLists.txt
# Handle boost libraries
find_package (Boost REQUIRED
	log
	system
	filesystem
	date_time
	thread
	log_setup
	chrono
	regex
)

message(STATUS "Boost library status:")
message(STATUS "    version: ${BoosT_VERSION}")
message(STATUS "    libraries: ${Boost_LIBRARIES}")
message(STATUS "    include path: ${Boost_INCLUDE_DIR}")

# Handle QT libraries
find_package( Qt5 REQUIRED
	Core
	Gui
	Widgets
)
message(STATUS "QT library status:")
message(STATUS "    version: ${QT_VERSION}")
message(STATUS "    libraries: ${QT_LIBRARIES}")
message(STATUS "    include path: ${QT_INCLUDE_DIR}")


file (GLOB MyProjectBin_src
      "${PROJECT_SOURCE_DIR}/app/PcapReplayer/*.cpp"
      "${PROJECT_SOURCE_DIR}/app/PcapReplayer/*.h"
	  "${PROJECT_SOURCE_DIR}/app/PcapReplayer/ConditionChecker/*.cpp"
	  "${PROJECT_SOURCE_DIR}/app/PcapReplayer/ConditionChecker/*.h"
	  "${PROJECT_SOURCE_DIR}/app/PcapReplayer/PcapReader/*.cpp"
	  "${PROJECT_SOURCE_DIR}/app/PcapReplayer/PcapReader/*.h")

# include_directories (${PROJECT_SOURCE_DIR}/lib/ui/PcapReplayer)
include_directories (${PROJECT_SOURCE_DIR}/lib/WpdPack/Include
					${QT_INCLUDE_DIR}
					${PROJECT_SOURCE_DIR}/lib/SystemService
					${PROJECT_SOURCE_DIR}/app/PcapReplayer
					${PROJECT_SOURCE_DIR}/app/PcapReplayer/ConditionChecker
					${PROJECT_SOURCE_DIR}/app/PcapReplayer/PcapReader
					${PROJECT_SOURCE_DIR}/lib/ui/PcapReplayer)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${QT_LIBRARY_DIR})

# Set Properties->General->Configuration Type to Application(.exe)
# Creates PcapReplayer.exe with the listed sources (maindialog.cpp)
# Adds sources to the Solution Explorer
# add_executable(PcapReplayer WIN32 ${MyProjectBin_src})
add_executable(PcapReplayer ${MyProjectBin_src})
add_dependencies(PcapReplayer SystemService PcapReplayUI)

# ${PROJECT_SOURCE_DIR}/lib/ui/PcapReplayer/resource.qrc


# Properties->Linker->Input->Additional Dependencies
#target_link_libraries (PcapReplayer ${MyProjectLib_link})
target_link_libraries (PcapReplayer 
						debug ${PROJECT_SOURCE_DIR}/lib/WpdPack/Lib/x64/wpcap.lib
						debug ${PROJECT_SOURCE_DIR}/lib/WpdPack/Lib/x64/Packet.lib
						debug ${PROJECT_BINARY_DIR}/lib/SystemService/$(Configuration)/SystemService.lib
						debug ${PROJECT_BINARY_DIR}/lib/ui/PcapReplayer/$(Configuration)/PcapReplayUI.lib
						optimized ${PROJECT_SOURCE_DIR}/lib/WpdPack/Lib/x64/wpcap.lib
						optimized ${PROJECT_SOURCE_DIR}/lib/WpdPack/Lib/x64/Packet.lib
						optimized ${PROJECT_BINARY_DIR}/lib/SystemService/$(Configuration)/SystemService.lib
						optimized ${PROJECT_BINARY_DIR}/lib/ui/PcapReplayer/$(Configuration)/PcapReplayUI.lib)
target_link_libraries (PcapReplayer ${Boost_LIBRARIES})
target_link_libraries (PcapReplayer Qt5::Widgets Qt5::Core Qt5::Gui ${Qt5Core_QTMAIN_LIBRARIES})
# target_link_libraries (PcapReplayer optimized ${MyProjectLib_Release_Link})

# Creates a folder "executables" and adds target 
# project (PcapReplayer.vcproj) under it
set_property(TARGET PcapReplayer PROPERTY FOLDER "executables")


# Adds logic to INSTALL.vcproj to copy PcapReplayer.exe to destination directory
install (TARGETS PcapReplayer
		 RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
		 LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/bin
		 ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/bin)
