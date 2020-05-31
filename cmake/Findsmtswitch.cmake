# FindSMT_SWITCH.cmake
# Finds the smt-switch library
#
# This will define the following variables
#
#   SMTSWITCH_FOUND
#   SMTSWITCH_INCLUDE_DIR
#   SMTSWITCH_LIBRARY
#
#   SMTSWITCH_BTOR_FOUND
#   SMTSWITCH_BTOR_LIBRARY
#   SMTSWITCH_CVC4_FOUND
#   SMTSWITCH_CVC4_LIBRARY
#   SMTSWITCH_MSAT_FOUND
#   SMTSWITCH_MSAT_LIBRARY
#   SMTSWITCH_YICES2_FOUND
#   SMTSWITCH_YICES2_LIBRARY
#
# and the following imported targets
#
#   smt-switch::smt-switch
# 

# XXX smt-switch needs to be built with static type
# XXX @smt-switch config time, set SMT_SWITCH_LIB_TYPE to STATIC

find_package(PkgConfig)
pkg_check_modules(PC_SMTSWITCH QUIET SMTSWITCH)

##
## core smt-switch
##
find_path(SMTSWITCH_INCLUDE_DIR
  NAMES "smt-switch/smt.h"
  HINTS ${PC_SMTSWITCH_INCLUDEDIR} ${PC_SMTSWITCH_INCLUDE_DIRS}
  PATH_SUFFIXES smt-switch
)

find_library(SMTSWITCH_LIBRARY 
  NAMES smt-switch
  HINTS ${PC_SMTSWITCH_LIBDIR} ${PC_SMTSWITCH_LIBRARY_DIRS}
  PATH_SUFFIXES smt-switch
)

mark_as_advanced(SMTSWITCH_FOUND SMTSWITCH_INCLUDE_DIR STM_SWITCH_LIBRARY)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(smtswitch DEFAULT_MSG 
  SMTSWITCH_INCLUDE_DIR 
  SMTSWITCH_LIBRARY
)

##
## individual solvers
##

# boolector
find_library(SMTSWITCH_BTOR_LIBRARY
  NAMES smt-switch-btor
  HINTS ${PC_SMTSWITCH_LIBDIR} ${PC_SMTSWITCH_LIBRARY_DIRS}
  PATH_SUFFIXES smt-switch
)

if(SMTSWITCH_BTOR_LIBRARY)
  set(SMTSWITCH_BTOR_FOUND TRUE)
else()
  set(SMTSWITCH_BTOR_FOUND FALSE)
endif()

mark_as_advanced(SMTSWITCH_BTOR_FOUND SMTSWITCH_BTOR_LIBRARY)

# cvc4
find_library(SMTSWITCH_CVC4_LIBRARY
  NAMES smt-switch-cvc4
  HINTS ${PC_SMTSWITCH_LIBDIR} ${PC_SMTSWITCH_LIBRARY_DIRS}
  PATH_SUFFIXES smt-switch
)

if(SMTSWITCH_CVC4_LIBRARY)
  set(SMTSWITCH_CVC4_FOUND TRUE)
else()
  set(SMTSWITCH_CVC4_FOUND FALSE)
endif()

mark_as_advanced(SMTSWITCH_CVC4_FOUND SMTSWITCH_CVC4_LIBRARY)

# math SAT
find_library(SMTSWITCH_MSAT_LIBRARY
  NAMES smt-switch-msat
  HINTS ${PC_SMTSWITCH_LIBDIR} ${PC_SMTSWITCH_LIBRARY_DIRS}
  PATH_SUFFIXES smt-switch
)

if(SMTSWITCH_MSAT_LIBRARY)
  set(SMTSWITCH_MSAT_FOUND TRUE)
else()
  set(SMTSWITCH_MSAT_FOUND FALSE)
endif()

mark_as_advanced(SMTSWITCH_MSAT_FOUND SMTSWITCH_MSAT_LIBRARY)

# yices2
find_library(SMTSWITCH_YICES2_LIBRARY
  NAMES smt-switch-yices2
  HINTS ${PC_SMTSWITCH_LIBDIR} ${PC_SMTSWITCH_LIBRARY_DIRS}
  PATH_SUFFIXES smt-switch
)

if(SMTSWITCH_YICES2_LIBRARY)
  set(SMTSWITCH_YICES2_FOUND TRUE)
else()
  set(SMTSWITCH_YICES2_FOUND FALSE)
endif()

mark_as_advanced(SMTSWITCH_YICES2_FOUND SMTSWITCH_YICES2_LIBRARY)

# create imported target smt-switch::smt-switch
if(SMTSWITCH_FOUND AND NOT TARGET smt-switch::smt-switch)

  add_library(smt-switch::smt-switch INTERFACE IMPORTED)

  set_target_properties(smt-switch::smt-switch PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${SMTSWITCH_INCLUDE_DIR}")

  set_target_properties(smt-switch::smt-switch PROPERTIES
    INTERFACE_LINK_LIBRARIES "${SMTSWITCH_LIBRARY}")

endif()

