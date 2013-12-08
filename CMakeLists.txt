
#-----------------------------------------------------------------------
# CPM configuration
#-----------------------------------------------------------------------
set(CPM_MODULE_NAME mongodbc)
set(CPM_LIB_TARGET_NAME ${CPM_MODULE_NAME})

if ((DEFINED CPM_DIR) AND (DEFINED CPM_UNIQUE_ID) AND (DEFINED CPM_TARGET_NAME))
  set(CPM_LIB_TARGET_NAME ${CPM_TARGET_NAME})
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CPM_DIR})
  include(CPM)
else()
  set(CPM_DIR "${CMAKE_CURRENT_BINARY_DIR}/cpm-packages" CACHE TYPE STRING)
  find_package(Git)
  if(NOT GIT_FOUND)
    message(FATAL_ERROR "CPM requires Git.")
  endif()
  if (NOT EXISTS ${CPM_DIR}/CPM.cmake)
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" clone https://github.com/iauns/cpm ${CPM_DIR}
      RESULT_VARIABLE error_code
      OUTPUT_VARIABLE head_sha)
    if(error_code)
      message(FATAL_ERROR "CPM failed to get the hash for HEAD")
    endif()
  endif()
  include(${CPM_DIR}/CPM.cmake)
endif()

# All externals *must* define this.
CPM_ForceOnlyOneModuleVersion()

CPM_InitModule(${CPM_MODULE_NAME})

#------------------------------------------------------------------------------
# Mongo c driver
#------------------------------------------------------------------------------

set(MONGO_EXTRA_BUILD_COMMAND "")
if ((DEFINED CPM_MOD_MONGODB_USE_32BIT) AND (CPM_MOD_MONGODB_USE_32BIT))
  set(MONGO_EXTRA_BUILD_COMMAND "--m32")
endif()

# TODO: Build a find package for 'scons'.
set(MONGO_EP_TARGET_NAME MongoDB_C_Driver_EP)
ExternalProject_Add(${MONGO_EP_TARGET_NAME}
  GIT_REPOSITORY "https://github.com/mongodb/mongo-c-driver-legacy.git"
  GIT_TAG "v0.8.1"
  PATCH_COMMAND ""
  INSTALL_DIR ""
  INSTALL_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND "scons" ${MONGO_EXTRA_BUILD_COMMAND}
  BUILD_IN_SOURCE 1
)

set(MONGO_DB_LIBRARY ${CPM_LIB_TARGET_NAME})
add_library(${MONGO_DB_LIBRARY} SHARED IMPORTED GLOBAL)
add_dependencies(${MONGO_DB_LIBRARY} ${MONGO_EP_TARGET_NAME})

# Grab the source directory, that is where all of the library files
# will reside. We will staically link against them.
ExternalProject_Get_Property(${MONGO_EP_TARGET_NAME} SOURCE_DIR)

# Generate a library path and set the imported location for mongoc.
set_target_properties(${MONGO_DB_LIBRARY}
  PROPERTIES
  IMPORTED_LOCATION ${SOURCE_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}mongoc${CMAKE_STATIC_LIBRARY_SUFFIX}
  )

# Expose the MONGO_HAVE_STDINT to the consuming module (will only expose in the
# parent module). And also expose an additional include directory to the parent
# module only.
CPM_ExportAdditionalDefinition("-DMONGO_HAVE_STDINT")
CPM_ExportAdditionalIncludeDir("${SOURCE_DIR}/src")

set(MONGO_DB_BSON_LIBRARY "${CPM_LIB_TARGET_NAME}_BSON")
add_library(${MONGO_DB_BSON_LIBRARY} SHARED IMPORTED GLOBAL)
add_dependencies(${MONGO_DB_BSON_LIBRARY} ${MONGO_EP_TARGET_NAME})
set_target_properties(${MONGO_DB_BSON_LIBRARY}
  PROPERTIES
  IMPORTED_LOCATION ${SOURCE_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}bson${CMAKE_STATIC_LIBRARY_SUFFIX}
  )
CPM_ExportAdditionalLibraryTarget(${MONGO_DB_BSON_LIBRARY})

