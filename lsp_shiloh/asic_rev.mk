# ============================================================================
# Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

# Strictly incrementing numerical ASIC revision defines.

ASIC_REV_Z0 = 10
ASIC_REV_Z1 = 20
ASIC_REV_Z2 = 30
ASIC_REV_Z3 = 40
ASIC_REV_A0 = 50
ASIC_REV_A1 = 60
ASIC_REV_A2 = 70
ASIC_REV_A3 = 80
ASIC_REV_B0 = 90
ASIC_REV_B1 = 100
ASIC_REV_B2 = 110
ASIC_REV_B3 = 120
ASIC_REV_C0 = 130

ifeq ($(ASIC_REV), z0)
ASIC_REV_NUM = $(ASIC_REV_Z0)
endif

ifeq ($(ASIC_REV), z1)
ASIC_REV_NUM = $(ASIC_REV_Z1)
endif

ifeq ($(ASIC_REV), z2)
ASIC_REV_NUM = $(ASIC_REV_Z2)
endif

ifeq ($(ASIC_REV), z3)
ASIC_REV_NUM = $(ASIC_REV_Z3)
endif

ifeq ($(ASIC_REV), a0)
ASIC_REV_NUM = $(ASIC_REV_A0)
endif

ifeq ($(ASIC_REV), a1)
ASIC_REV_NUM = $(ASIC_REV_A1)
endif

ifeq ($(ASIC_REV), a2)
ASIC_REV_NUM = $(ASIC_REV_A2)
endif

ifeq ($(ASIC_REV), a3)
ASIC_REV_NUM = $(ASIC_REV_A3)
endif

ifeq ($(ASIC_REV), b0)
ASIC_REV_NUM = $(ASIC_REV_B0)
endif

ifeq ($(ASIC_REV), b1)
ASIC_REV_NUM = $(ASIC_REV_B1)
endif

ifeq ($(ASIC_REV), b2)
ASIC_REV_NUM = $(ASIC_REV_B2)
endif

ifeq ($(ASIC_REV), b3)
ASIC_REV_NUM = $(ASIC_REV_B3)
endif

ifeq ($(ASIC_REV), c0)
ASIC_REV_NUM = $(ASIC_REV_C0)
endif

ifndef ASIC_REV_NUM
$(error Unknown ASIC revision)
endif

ifeq ($(ASIC_REV_NUM), 0)
$(error Unknown ASIC revision)
endif

MACROS += ASIC_REV_Z0=$(ASIC_REV_Z0) \
          ASIC_REV_Z1=$(ASIC_REV_Z1) \
          ASIC_REV_Z2=$(ASIC_REV_Z2) \
          ASIC_REV_Z3=$(ASIC_REV_Z3) \
          ASIC_REV_A0=$(ASIC_REV_A0) \
          ASIC_REV_A1=$(ASIC_REV_A1) \
          ASIC_REV_A2=$(ASIC_REV_A2) \
          ASIC_REV_A3=$(ASIC_REV_A3) \
          ASIC_REV_B0=$(ASIC_REV_B0) \
          ASIC_REV_B1=$(ASIC_REV_B1) \
          ASIC_REV_B2=$(ASIC_REV_B2) \
          ASIC_REV_B3=$(ASIC_REV_B3) \
          ASIC_REV_C0=$(ASIC_REV_C0)


