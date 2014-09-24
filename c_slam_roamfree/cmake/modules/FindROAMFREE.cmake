# - ROAMFREE library and header paths

# Note:
#
# At the moment ROAMFREE does not have a proper project-config file. 
# The user has to set two envirorment variables ROAMFREE_BUILD_PATH and ROAMFREE_SRC_PATH
# Please be patient until we fix this.

SET(LIB_ROAMESTIMATION LIB_ROAMESTIMATION-NOTFOUND)
SET(LIB_ROAMIMU LIB_ROAMIMU-NOTFOUND)

FIND_LIBRARY(LIB_ROAMESTIMATION NAMES ROAMestimation ROAMestimation_d PATHS $ENV{ROAMFREE_BUILD_PATH}/lib)
FIND_LIBRARY(LIB_ROAMIMU NAMES ROAMimu ROAMimu_d PATHS $ENV{ROAMFREE_BUILD_PATH}/lib)

SET(ROAMFREE_LIBRARIES ${LIB_ROAMESTIMATION} ${LIB_ROAMIMU})

SET(ROAMFREE_INCLUDE_DIR $ENV{ROAMFREE_SRC_PATH}/ROAMFREE $ENV{ROAMFREE_SRC_PATH}/g2o $ENV{ROAMFREE_BUILD_PATH}/g2o)

SET(ROAMFREE_FOUND TRUE) 