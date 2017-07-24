# CMakeLists.txt
set(MY_PRODUCT_NUMBER 3)
set(MY_PRODUCT_VERSION 5)
set(MY_BUILD_NUMBER 49)

set (QTRootPath "C:\\Qt\\Qt5.6.0\\5.6\\msvc2013_64")
set (boostRootPath "C:\\boost\\boost_1_59_0")

file (GLOB MyProjectBin_src
      "${PROJECT_SOURCE_DIR}/app/PcapReplayer/*.cpp"
      "${PROJECT_SOURCE_DIR}/app/PcapReplayer/*.h"
	  "${PROJECT_SOURCE_DIR}/app/PcapReplayer/ConditionChecker/*.cpp"
	  "${PROJECT_SOURCE_DIR}/app/PcapReplayer/ConditionChecker/*.h"
	  "${PROJECT_SOURCE_DIR}/app/PcapReplayer/PcapReader/*.cpp"
	  "${PROJECT_SOURCE_DIR}/app/PcapReplayer/PcapReader/*.h")

# include_directories (${PROJECT_SOURCE_DIR}/lib/ui/PcapReplayer)
include_directories (${PROJECT_SOURCE_DIR}/lib/WpdPack/Include
					${boostRootPath}
					${PROJECT_SOURCE_DIR}/lib/SystemService
					${PROJECT_SOURCE_DIR}/app/PcapReplayer
					${PROJECT_SOURCE_DIR}/app/PcapReplayer/ConditionChecker
					${PROJECT_SOURCE_DIR}/app/PcapReplayer/PcapReader
					${PROJECT_SOURCE_DIR}/lib/ui/PcapReplayer
					${QTRootPath}/include
					${QTRootPath}/include/QtWidgets
					${QTRootPath}/include/QtGui
					${QTRootPath}/include/include/QtCore
					${QTRootPath}/mkspecs/win32-msvc2013)

# Set Properties->General->Configuration Type to Application(.exe)
# Creates PcapReplayer.exe with the listed sources (maindialog.cpp)
# Adds sources to the Solution Explorer
# add_executable(PcapReplayer WIN32 ${MyProjectBin_src})
add_executable(PcapReplayer ${MyProjectBin_src} ${VersionFilesOutputVariable})
# Handle version
set_target_properties(PcapReplayer
    PROPERTIES 
	VERSION ${MY_PRODUCT_NUMBER}.${MY_PRODUCT_VERSION}.${MY_BUILD_NUMBER}
    SOVERSION ${MY_PRODUCT_NUMBER})
add_dependencies(PcapReplayer SystemService PcapReplayUI)

# ${PROJECT_SOURCE_DIR}/lib/ui/PcapReplayer/resource.qrc


# Properties->Linker->Input->Additional Dependencies
#target_link_libraries (PcapReplayer ${MyProjectLib_link})
target_link_libraries (PcapReplayer 
						debug ${PROJECT_SOURCE_DIR}/lib/WpdPack/Lib/x64/wpcap.lib
						debug ${PROJECT_SOURCE_DIR}/lib/WpdPack/Lib/x64/Packet.lib
						debug ${boostRootPath}/lib64-msvc-12.0/libboost_log-vc120-mt-gd-1_59.lib
						debug ${boostRootPath}/lib64-msvc-12.0/libboost_system-vc120-mt-gd-1_59.lib
						debug ${boostRootPath}/lib64-msvc-12.0/libboost_filesystem-vc120-mt-gd-1_59.lib
						debug ${boostRootPath}/lib64-msvc-12.0/libboost_date_time-vc120-mt-gd-1_59.lib
						debug ${boostRootPath}/lib64-msvc-12.0/libboost_thread-vc120-mt-gd-1_59.lib
						debug ${boostRootPath}/lib64-msvc-12.0/libboost_log_setup-vc120-mt-gd-1_59.lib
						debug ${boostRootPath}/lib64-msvc-12.0/libboost_chrono-vc120-mt-gd-1_59.lib
						debug ${boostRootPath}/lib64-msvc-12.0/libboost_regex-vc120-mt-gd-1_59.lib
						debug ${PROJECT_BINARY_DIR}/lib/SystemService/$(Configuration)/SystemService.lib
						debug ${PROJECT_BINARY_DIR}/lib/ui/PcapReplayer/$(Configuration)/PcapReplayUI.lib
						debug ${QTRootPath}/lib/Qt5Cored.lib
						debug ${QTRootPath}/lib/Qt5Guid.lib
						debug ${QTRootPath}/lib/qtmaind.lib
						debug ${QTRootPath}/lib/Qt5Widgetsd.lib
						optimized ${PROJECT_SOURCE_DIR}/lib/WpdPack/Lib/x64/wpcap.lib
						optimized ${PROJECT_SOURCE_DIR}/lib/WpdPack/Lib/x64/Packet.lib
						optimized ${boostRootPath}/lib64-msvc-12.0/libboost_log-vc120-mt-1_59.lib
						optimized ${boostRootPath}/lib64-msvc-12.0/libboost_system-vc120-mt-1_59.lib
						optimized ${boostRootPath}/lib64-msvc-12.0/libboost_filesystem-vc120-mt-1_59.lib
						optimized ${boostRootPath}/lib64-msvc-12.0/libboost_date_time-vc120-mt-1_59.lib
						optimized ${boostRootPath}/lib64-msvc-12.0/libboost_thread-vc120-mt-1_59.lib
						optimized ${boostRootPath}/lib64-msvc-12.0/libboost_log_setup-vc120-mt-1_59.lib
						optimized ${boostRootPath}/lib64-msvc-12.0/libboost_chrono-vc120-mt-1_59.lib
						optimized ${boostRootPath}/lib64-msvc-12.0/libboost_regex-vc120-mt-1_59.lib
						optimized ${PROJECT_BINARY_DIR}/lib/SystemService/$(Configuration)/SystemService.lib
						optimized ${PROJECT_BINARY_DIR}/lib/ui/PcapReplayer/$(Configuration)/PcapReplayUI.lib
						optimized ${QTRootPath}/lib/Qt5Core.lib
						optimized ${QTRootPath}/lib/Qt5Gui.lib
						optimized ${QTRootPath}/lib/qtmain.lib
						optimized ${QTRootPath}/lib/Qt5Widgets.lib)
# target_link_libraries (PcapReplayer optimized ${MyProjectLib_Release_Link})

# Creates a folder "executables" and adds target 
# project (PcapReplayer.vcproj) under it
set_property(TARGET PcapReplayer PROPERTY FOLDER "executables")


# Adds logic to INSTALL.vcproj to copy PcapReplayer.exe to destination directory
install (TARGETS PcapReplayer
		 RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
		 LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/bin
		 ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/bin)
