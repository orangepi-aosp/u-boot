# Copyright 2022 Man Nguyen <nmman37@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

ANDROID_TOP := $(realpath $(TOP))

ifeq ($(UBOOT_DEFCONFIG),)
$(error UBOOT_DEFCONFIG is required)
endif

ifeq ($(UBOOT_CROSS_COMPILE_PREFIX),)
$(error UBOOT_CROSS_COMPILE_PREFIX is required)
endif

ifeq ($(UBOOT_EXTRA_BUILD_ENV),)
UBOOT_EXTRA_BUILD_ENV :=
endif

UBOOT_SRC := $(call my-dir)
UBOOT_OUT := $(TARGET_OUT_INTERMEDIATES)/UBOOT_OBJ
UBOOT_CONFIG := $(UBOOT_OUT)/.config
UBOOT_IMAGE := $(UBOOT_OUT)/u-boot.img

UBOOT_MAKE_CMD := \
	PATH=/usr/bin:$$PATH \
	ARCH=$(TARGET_ARCH) \
	CROSS_COMPILE=$(ANDROID_TOP)/$(UBOOT_CROSS_COMPILE_PREFIX) \
	$(UBOOT_EXTRA_BUILD_ENV) \
	$(MAKE) -C $(UBOOT_SRC) O=$(ANDROID_TOP)/$(UBOOT_OUT)

$(UBOOT_OUT):
	$(hide) mkdir -p $(UBOOT_OUT)

$(UBOOT_CONFIG): $(UBOOT_SRC)/Makefile $(UBOOT_OUT)
	$(hide) echo "Configuring U-Boot..."
	$(UBOOT_MAKE_CMD) $(UBOOT_DEFCONFIG)

$(UBOOT_IMAGE): $(UBOOT_CONFIG)
	$(hide) echo "Building U-Boot..."
	$(UBOOT_MAKE_CMD)

$(UBOOT_IDBLOADER): $(UBOOT_ITB_IMAGE)

.PHONY: uboot
uboot: $(UBOOT_IMAGE)

.PHONY: clean-uboot
clean-uboot:
	rm -rf $(UBOOT_OUT)
