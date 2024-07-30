# ----------------------------
# Makefile Options
# ----------------------------

NAME = MineCE
ICON = icon.png
DESCRIPTION = "Minecraft for the CE"
COMPRESSED = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
