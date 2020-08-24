# - Try to find Autotester library
#
# The following variables are optionally searched for defaults
#  AUTOTESTER_ROOT_DIR:            Base directory where all autotester components are found
#
# The following are set after configuration is done:
#  AUTOTESTER_FOUND
#  AUTOTESTER_INCLUDE_DIRS
#  AUTOTESTER_LIBRARIES
#  AUTOTESTER_LIBRARYRARY_DIRS

include(FindPackageHandleStandardArgs)

#find_path(AUTOTESTER_INCLUDE_DIR autotester.h
#    PATHS ${AUTOTESTER_LIBRARYDIR}/include)

find_library(AUTOTESTER_LIBRARY_DEBUG autotester 
    PATHS ${AUTOTESTER_ROOT}/debug
    PATH_SUFFIXES
        lib
        lib64)

find_library(AUTOTESTER_LIBRARY_RELEASE autotester 
    PATHS ${AUTOTESTER_ROOT}/release
    PATH_SUFFIXES
        lib
        lib64)

SET(AUTOTESTER_LIBRARY
    debug ${AUTOTESTER_LIBRARY_DEBUG}
        optimized ${AUTOTESTER_LIBRARY_RELEASE}
          )
find_package_handle_standard_args(AUTOTESTER DEFAULT_MSG
    # AUTOTESTER_INCLUDE_DIR
    AUTOTESTER_LIBRARY)

if(AUTOTESTER_FOUND)
    #set(AUTOTESTER_INCLUDE_DIRS ${AUTOTESTER_INCLUDE_DIR})

    set(AUTOTESTER_LIBRARIES ${AUTOTESTER_LIBRARY})
endif()

