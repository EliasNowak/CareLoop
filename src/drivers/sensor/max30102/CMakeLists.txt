# MAX30102 heart rate sensor driver
#
# Copyright (c) 2017, NXP
#
# SPDX-License-Identifier: Apache-2.0

zephyr_library()
zephyr_library_sources(max30102.c)
zephyr_library_link_libraries(drivers__sensor)
