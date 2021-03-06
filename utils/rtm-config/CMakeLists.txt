cmake_minimum_required (VERSION 3.0.2)



set(INSTALL_INCLUDE_DIR include)
set(INSTALL_RTM_IDL_DIR ${INSTALL_RTM_INCLUDE_DIR}/rtm/idl)
set(INSTALL_DATA_DIR openrtm-${RTM_MAJOR_VERSION}.${RTM_MINOR_VERSION})
set(INSTALL_RTM_IDL_DIR ${INSTALL_RTM_INCLUDE_DIR}/rtm/idl)
set(INSTALL_RTC_DIR ${INSTALL_RTM_EXT_DIR}/rtc)
set(INSTALL_EC_DIR ${INSTALL_RTM_EXT_DIR}/ec)
set(INSTALL_RTM_DIR ${INSTALL_RTM_EXT_DIR}/rtm)
set(INSTALL_SVC_DIR ${INSTALL_RTM_EXT_DIR}/rtm)
set(PACKAGE OpenRTM-aist)
set(RTM_DESCRIPTION An OMG RTC compliant middleware for robots.)
set(RTM_LIBS ${RTM_PROJECT_NAME})
string(REPLACE ";" " " IDL_FLAGS "${IDLCOMMAND_FLAGS}")

set(TMPLIBS -lpthread ${RTM_LINKER_OPTION})
foreach(library ${ORB_LIBRARIES})
	set(TMPLIBS ${TMPLIBS} -l${library})
endforeach()
set(TMPLIBS ${TMPLIBS} -l${RTM_PROJECT_NAME} -lrtmCamera -lrtmManipulator)
string(REPLACE ";" " " LIBS "${TMPLIBS}")




configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rtm-config_cmake.in ${PROJECT_BINARY_DIR}/rtm-config @ONLY)

if(NOT WIN32)
	install(FILES ${PROJECT_BINARY_DIR}/rtm-config DESTINATION ${INSTALL_RTM_BIN_DIR} COMPONENT utils PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)
else()
	install(FILES ${PROJECT_BINARY_DIR}/rtm-config DESTINATION ${INSTALL_RTM_BIN_DIR} COMPONENT utils)
endif()


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/openrtm-aist_cmake.pc.in ${PROJECT_BINARY_DIR}/openrtm-aist.pc @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/openrtm-aist.pc DESTINATION ${INSTALL_RTM_LIB_DIR}/pkgconfig COMPONENT utils)
