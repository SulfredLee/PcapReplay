set(MY_PRODUCT_NUMBER 3)
set(MY_PRODUCT_VERSION 5)
set(MY_BUILD_NUMBER 49)
set (boostRootPath "C:\\boost\\boost_1_59_0")

find_package (Qt5Widgets)

set (PcapReplayUI_path ${PROJECT_SOURCE_DIR}/lib/ui/PcapReplayer)
file(GLOB PcapReplayUI_src_proxy "${PcapReplayUI_path}/*.cpp")
file(GLOB PcapReplayUI_hdr_proxy "${PcapReplayUI_path}/*.h")
file(GLOB PcapReplayUI_ui_proxy "${PcapReplayUI_path}/*.ui")
set (PcapReplayUI_src ${PcapReplayUI_src_proxy})
set (PcapReplayUI_hdr ${PcapReplayUI_hdr_proxy})
set (PcapReplayUI_ui  ${PcapReplayUI_ui_proxy})
set (PcapReplayUI_rcc ${PcapReplayUI_path}/resource.qrc)

qt5_wrap_cpp(PcapReplayUI_hdr_moc ${PcapReplayUI_hdr})
qt5_wrap_ui (PcapReplayUI_ui_moc  ${PcapReplayUI_ui})
qt5_add_resources(PcapReplayUI_rcc_moc ${PcapReplayUI_rcc})

include_directories (${PROJECT_BINARY_DIR}/lib/ui/PcapReplayer
					${PROJECT_SOURCE_DIR}/app/PcapReplayer
					${PROJECT_SOURCE_DIR}/app/PcapReplayer/ConditionChecker
					${PROJECT_SOURCE_DIR}/app/PcapReplayer/PcapReader
					${boostRootPath}
					${PROJECT_SOURCE_DIR}/lib/WpdPack/Include
					${PROJECT_SOURCE_DIR}/lib/SystemService)

add_library (PcapReplayUI
    ${PcapReplayUI_src}
	${PcapReplayUI_hdr}
    ${PcapReplayUI_hdr_moc}
    ${PcapReplayUI_ui_moc}
	${PcapReplayUI_rcc_moc}
)
target_link_libraries (PcapReplayUI Qt5::Widgets)
set_target_properties(PcapReplayUI
    PROPERTIES 
	VERSION ${MY_PRODUCT_NUMBER}.${MY_PRODUCT_VERSION}.${MY_BUILD_NUMBER}
    SOVERSION ${MY_PRODUCT_NUMBER})
# Creates a folder "libraries" and adds target project (PcapReplayUI.vcproj) under it
set_property(TARGET PcapReplayUI PROPERTY FOLDER "libraries")

# add_library (PcapReplayUIDLL SHARED
#	${PcapReplayUI_src}
#	${PcapReplayUI_hdr}
#	${PcapReplayUI_hdr_moc}
#	${PcapReplayUI_ui_moc}
# )
# target_link_libraries (PcapReplayUIDLL Qt5::Widgets)
# Creates a folder "libraries" and adds target project (PcapReplayUIDLL.vcproj) under it
#set_property(TARGET PcapReplayUIDLL PROPERTY FOLDER "libraries")

# Adds logic to INSTALL.vcproj to copy PcapReplayUI.lib to destination directory
# install (TARGETS PcapReplayUI PcapReplayUIDLL
install (TARGETS PcapReplayUI
		 RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
		 LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/bin
		 ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/bin)