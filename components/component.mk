#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)
EXTRA_COMPONENT_DIRS = $(IDF_PATH)/examples/common_components/protocol_examples_common
EXTRA_COMPONENT_DIRS = $(PROJECT_PATH)/components
EXTRA_COMPONENT_DIRS = $(PROJECT_PATH)/


COMPONENT_DIRS = $(PROJECT_PATH)/components
COMPONENT_DIRS = $(PROJECT_PATH)/main
include $(PROJECT_PATH)/main

#$(PROJECT_PATH)/components
#$(PROJECT_PATH)/main