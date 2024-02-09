################################
# Common settings
cmake_minimum_required(VERSION 3.27)

SET(TARGET bootloader)

#IF(NOT MIMIC_HEIMAN)
#    SET(MIMIC_HEIMAN 0)
#ELSE()
#    SET(TARGET ${TARGET}_heiman)
#ENDIF()

ADD_DEFINITIONS(
#    -DEND_DEVICE=1
    -DMCU_CORE_${TELINK_PLATFORM}=1
    -DMCU_CORE_8258
    -DMCU_STARTUP_8258
    -DMCU_STARTUP_${TELINK_PLATFORM}=1
    -D__PROJECT_TL_BOOT_LOADER__=1
#    -DMIMIC_HEIMAN=${MIMIC_HEIMAN}
)

################################
# Zigbee Library

SET(ZIGBEE_SRC
    ${SDK_PREFIX}/platform/boot/link_cfg.S
    ${SDK_PREFIX}/platform/boot/${TELINK_PLATFORM}/cstartup_${TELINK_PLATFORM}.S
    ${SDK_PREFIX}/platform/services/b85m/irq_handler.c
    ${SDK_PREFIX}/platform/tc32/div_mod.S
    ${SDK_PREFIX}/platform/chip_${TELINK_PLATFORM}/flash.c
    ${SDK_PREFIX}/platform/chip_${TELINK_PLATFORM}/adc.c
    ${SDK_PREFIX}/platform/chip_${TELINK_PLATFORM}/flash/flash_mid13325e.c

 

    ${SDK_PREFIX}/proj/common/list.c
    ${SDK_PREFIX}/proj/common/mempool.c
    ${SDK_PREFIX}/proj/common/string.c
    ${SDK_PREFIX}/proj/common/tlPrintf.c
    ${SDK_PREFIX}/proj/common/utility.c
    ${SDK_PREFIX}/proj/drivers/drv_adc.c
    ${SDK_PREFIX}/proj/drivers/drv_flash.c
    ${SDK_PREFIX}/proj/drivers/drv_calibration.c
    ${SDK_PREFIX}/proj/drivers/drv_gpio.c
    ${SDK_PREFIX}/proj/drivers/drv_hw.c
    ${SDK_PREFIX}/proj/drivers/drv_i2c.c
    ${SDK_PREFIX}/proj/drivers/drv_keyboard.c
    ${SDK_PREFIX}/proj/drivers/drv_nv.c
    ${SDK_PREFIX}/proj/drivers/drv_pm.c
    ${SDK_PREFIX}/proj/drivers/drv_putchar.c
    ${SDK_PREFIX}/proj/drivers/drv_pwm.c
    ${SDK_PREFIX}/proj/drivers/drv_spi.c
    ${SDK_PREFIX}/proj/drivers/drv_timer.c
    ${SDK_PREFIX}/proj/drivers/drv_uart.c
    ${SDK_PREFIX}/proj/os/ev.c
    ${SDK_PREFIX}/proj/os/ev_buffer.c
    ${SDK_PREFIX}/proj/os/ev_poll.c
    ${SDK_PREFIX}/proj/os/ev_queue.c
    ${SDK_PREFIX}/proj/os/ev_timer.c


)


################################
# Telink Zigbee device

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}


    ${SDK_PREFIX}/proj
    ${SDK_PREFIX}/proj/common
    ${SDK_PREFIX}/platform

    # Zigbee sources reference zcl_options.h
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/bootLoader
)

LINK_DIRECTORIES(
#	${SDK_PREFIX}/zigbee/lib/tc32
	${SDK_PREFIX}/platform/lib
)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${SDK_PREFIX}/platform/boot/${TELINK_PLATFORM}/boot_${TELINK_PLATFORM}.link")

file( GLOB SOURCES1 *.c *.cpp *.h *.S common/*.c common/*.h )

SET (SOURCES  ${SOURCES1} ${ZIGBEE_SRC})

ADD_EXECUTABLE(${TARGET} ${SOURCES})
TARGET_LINK_LIBRARIES(${TARGET}
        -Wl,--start-group

	#ZigBee

#	zb_router
	drivers_${TELINK_PLATFORM}
#        soft-fp
	timestamp

        -Wl,--end-group
)

ADD_BIN_TARGET(${TARGET} ${PROJECT_SOURCE_DIR}/tools)
ADD_OTA_TARGET(${TARGET} ${PROJECT_SOURCE_DIR}/tools)
