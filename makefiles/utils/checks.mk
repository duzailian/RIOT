# Utilities to produce errors inside Make
# Use this functions to produce friendlier error messages.

# Produce an error if the value is empty
#
# Parameters
#	value: a string which should not be empty
#	message: The error message to display.
# Returns:
#	the first argument, if it is not empty.
# 确定第一个参数是否为空字符串,若非空则返回该参数，否则报错
ensure_value = $(if $(1),$(1),$(error $(2)))
