#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)


## set(CMAKE_SYSTEM_NAME Darwin)
## message(STATUS ${CMAKE_SYSTEM_NAME})
cmake_policy(VERSION 2.8)


project(G10X)


#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

  message(FATAL "    Geant4_USE_FILE: ${Geant4_USE_FILE} ")




#====================================
# ROOT_FOUND          If the ROOT is found
# ROOT_INCLUDE_DIR    PATH to the include directory
# ROOT_LIBRARIES      Most common libraries
# ROOT_LIBRARY_DIR    PATH to the library directory 
#====================================
find_program(ROOT_CONFIG_EXECUTABLE root-config PATHS $ENV{ROOTSYS}/bin)

#  message(FATAL "     $ENV{ROOTSYS}   ${ROOT_CONFIG_EXECUTABLE}")

if(NOT ROOT_CONFIG_EXECUTABLE)
  set(ROOT_FOUND FALSE)
else()    
  set(ROOT_FOUND TRUE)

##  execute_process(COMMAND ${ROOT_CONFIG_EXECUTABLE} ${VER}                   RESULT_VARIABLE res  OUTPUT_VARIABLE ROOT_VERSION     OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(COMMAND "root-config" "--version" 
WORKING_DIRECTORY "/usr/local/root/bin" RESULT_VARIABLE res OUTPUT_VARIABLE ROOT_VERSION     OUTPUT_STRIP_TRAILING_WHITESPACE)
  message(FATAL "    ROOT_VERSION:   ${res} <${ROOT_VERSION}>")

  execute_process(COMMAND ${ROOT_CONFIG_EXECUTABLE} --incdir                      OUTPUT_VARIABLE ROOT_INCLUDE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(ROOT_INCLUDE_DIR STREQUAL "")
    set(ROOT_INCLUDE_DIR ${ROOTSYS}/include)
  endif()

  execute_process(COMMAND ${ROOT_CONFIG_EXECUTABLE} --libdir                      OUTPUT_VARIABLE ROOT_LIBRARY_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(ROOT_LIBRARY_DIR STREQUAL "")
    set(ROOT_LIBRARY_DIR ${ROOTSYS}/lib)
  endif()

  execute_process(COMMAND ${ROOT_CONFIG_EXECUTABLE} --libs                        OUTPUT_VARIABLE ROOT_LIBRARIES   OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(ROOT_LIBRARIES STREQUAL "")
    message(WARNING "    Forced root libraries list ...")
    set(ROOT_LIBRARIES ${ROOT_LIBRARIES} -L/usr/local/root/lib  )
    set(ROOT_LIBRARIES ${ROOT_LIBRARIES} -lCore -lRIO -lNet -lHist -lGraf
                                         -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore
                                         -lThread -lpthread -Wl,-rpath,/usr/local/root/lib -stdlib=libc++ -lm -ldl )
  endif()
  set(ROOT_LIBRARIES ${ROOT_LIBRARIES} -lMinuit2  )


# Make variables changeble to the advanced user
# mark_as_advanced(ROOT_CONFIG_EXECUTABLE)

#  if(NOT ROOT_FIND_QUIETLY)
#    message(WARNING "    Found ROOT ${ROOT_VERSION} in ${ROOTSYS}")
#  endif()
endif()

MESSAGE(STATUS "    ROOTSYS          = ${ROOTSYS} ")
message(STATUS "    ROOT_VERSION     = ${ROOT_VERSION} ")
message(STATUS "    ROOT_INCLUDE_DIR = ${ROOT_INCLUDE_DIR} ")
message(STATUS "    ROOT_LIBRARY_DIR = ${ROOT_LIBRARY_DIR} ")
message(STATUS "    ROOT_LIBRARIES   = ${ROOT_LIBRARIES} ")
message(STATUS "    ...\n ")

include_directories(${ROOT_INCLUDE_DIR})
#====================================






#====================================
# Home_INCLUDE_DIR    PATH to the include directory or ""
# Home_LIBRARIES      list of all used libraries
# Home_LIBRARY_DIR    PATH to the library directory 

set(Home_INCLUDE_DIR $ENV{HOME}/include)
set(Home_LIBRARY_DIR $ENV{HOME}/lib)
set(Home_LIBRARIES -L${Home_LIBRARY_DIR} )
##--set(Home_LIBRARIES ${Home_LIBRARIES} )
set(Home_LIBRARIES ${Home_LIBRARIES} -lConfiguration -lVertexGenerators -lInteractionGenerators -lEdmFunc
-lEdmCommon -lEdmGeantTEvent )

message(STATUS "    Home_INCLUDE_DIR = ${Home_INCLUDE_DIR} ")
message(STATUS "    Home_LIBRARY_DIR = ${Home_LIBRARY_DIR} ")
message(STATUS "    Home_LIBRARIES   = ${Home_LIBRARIES}")
message(STATUS "    ...\n ")

include_directories(${Home_INCLUDE_DIR})
#====================================






#====================================
# External_INCLUDE_DIR    PATH to the include directory or ""
# External_LIBRARIES      libraries listed with '-l' prefix 
# External_LIBRARY_DIR    PATH to the library directory 

set(External_INCLUDE_DIR "")
set(External_LIBRARY_DIR "")
set(External_LIBRARIES )
set(External_LIBRARIES ${External_LIBRARIES} -lxerces-c )

message(STATUS "    External_INCLUDE_DIR = ${External_INCLUDE_DIR} ")
message(STATUS "    External_LIBRARY_DIR = ${External_LIBRARY_DIR} ")
message(STATUS "    External_LIBRARIES   = ${External_LIBRARIES} ")
message(STATUS "    ...\n ")

include_directories(${External_INCLUDE_DIR})
#====================================






#====================================
#include(${Geant4_cmake_Modules}/Geant4OptionalComponents.cmake)
#include(${Geant4_cmake_Modules}/Geant4InstallData.cmake)
#include(${Geant4_cmake_Modules}/Geant4ToolchainBackwardCompatibility.cmake)
add_definitions(${Geant4_DEFINITIONS})
#====================================










#-------------------------------finish
# Add the executable, and link it to the Geant4 libraries
#-------------------------------end


#------------------------------- tmp printout ...
message(STATUS "      Geant4_VERSION = " ${Geant4_VERSION})
message(STATUS "         DESTINATION = " ${DESTINATION})
message(STATUS "    Geant4_LIBRARIES = " ${Geant4_LIBRARIES})
message(STATUS "             headers = " ${headers})
message(STATUS "             sources = " ${sources})
message(STATUS "  External_LIBRARIES = " ${External_LIBRARIES})
#------------------------------- end




#------------------------------- library module generation ...
execute_process(COMMAND "make" "--directory=${PROJECT_SOURCE_DIR}/modules" RESULT_VARIABLE res OUTPUT_STRIP_TRAILING_WHITESPACE)
message(FATAL "    ${PROJECT_SOURCE_DIR}/modules make (done) res: ${res}\n")
#-------------------------------







#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
#### add_executable(mainG10X mainG10X.cc ${sources} ${headers})
#### target_link_libraries(mainG10X ${Geant4_LIBRARIES})

add_executable(main${PROJECT_NAME} main${PROJECT_NAME}.cc ${sources} ${headers})
###add_executable(main${PROJECT_NAME} MACOSX_BUNDLE  main${PROJECT_NAME}.cc ${sources} ${headers})

target_link_libraries(main${PROJECT_NAME} ${Geant4_LIBRARIES} ${Home_LIBRARIES} ${ROOT_LIBRARIES} ${External_LIBRARIES})
#----------------------------------------------------------------------------



# Copy all scripts to the build directory, i.e. the directory in which we
# build G10X. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(G10X_SCRIPTS
	config.dat
	EdmGeantParticleDB.dat
  icons.mac
  gui.mac
  run.png
  init.mac 
  init_vis.mac 
  run2.mac 
  vis.mac
  )

# copy needed files from source to build ... GM
foreach(_script ${G10X_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Add program to the project targets (this avoids the need of typing the program name after make)
#
add_custom_target(G10X DEPENDS mainG10X)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS mainG10X DESTINATION bin)

# e-o-f
