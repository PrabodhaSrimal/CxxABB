## User functions

define print_current_makefile
$(info [INFO] Current Makefile is $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))  )
endef

define test_and_create_dir
@$(TESTDIR) $1 || $(MKDIR) $1
endef

define make_vpath
vpath %.cpp $1/$2
endef
