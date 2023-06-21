# Default when RIOTBASE is not set and is executed from the RIOT directory
RIOTBOARD ?= $(or $(RIOTBASE),$(CURDIR))/boards
BOARDSDIRS ?= $(EXTERNAL_BOARD_DIRS) $(RIOTBOARD)

# List all boards in a directory
# By default, all directories in board directory except 'common'
#   use 'wildcard */.' to only list directories
# 获取指定目录下除common外的文件夹
_get_boards_in_directory = $(filter-out common,$(patsubst $1/%/.,%,$(wildcard $1/*/.)))

# Use `:=` so that it is evaluated before BOARDSDIRS gets eventually changed
# 获取boards目录下除common文件夹外的其他所有文件夹
ALLBOARDS := $(sort \
                    $(foreach dir,\
                              $(BOARDSDIRS),\
                              $(call _get_boards_in_directory,$(dir))))

# Set the default value from `BOARDS`
BOARDS ?= $(ALLBOARDS) # boards文件夹下除common外的所有文件夹

.PHONY: info-boards info-emulated-boards
info-boards:# 打印boards下的所有文件夹(不包括common文件夹)
	@echo $(BOARDS)

# 虚拟板子相关信息
# 可被工具处理后打包到image文件中的文件
EMULATED_BOARDS_RENODE := $(wildcard $(BOARDSDIRS)/*/dist/board.resc)
EMULATED_BOARDS_QEMU := microbit # microbit板

#所有虚拟板名称
EMULATED_BOARDS := \
  $(EMULATED_BOARDS_QEMU) \
  $(foreach board_path,$(EMULATED_BOARDS_RENODE),$(subst $(strip $(BOARDSDIRS)/),,$(subst /dist/board.resc,,$(board_path))))
  #

info-emulated-boards:
	@echo $(sort $(EMULATED_BOARDS))
