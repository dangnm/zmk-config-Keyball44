/*
 * Copyright (c) 2025 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_pmw_timeout

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>
#include <zmk/behavior.h>
#include <zmk/endpoints.h>

#include "pmw3610.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

struct behavior_pmw_timeout_config {
    uint32_t timeout_ms;
};

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    const struct behavior_pmw_timeout_config *cfg = dev->config;
    
    // Get the PMW3610 device reference
    const struct device *pmw_dev = DEVICE_DT_GET(DT_NODELABEL(trackball));
    
    if (!device_is_ready(pmw_dev)) {
        LOG_ERR("PMW3610 device not ready");
        return -ENODEV;
    }
    
    // Set the timeout
    int ret = pmw3610_set_automouse_timeout_ms(pmw_dev, cfg->timeout_ms);
    
    if (ret == 0) {
        LOG_INF("Auto mouse timeout set to %u ms", cfg->timeout_ms);
    } else {
        LOG_ERR("Failed to set timeout: %d", ret);
    }
    
    return ret;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                       struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_pmw_timeout_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
};

static int behavior_pmw_timeout_init(const struct device *dev) {
    return 0;
}

#define PMW_TIMEOUT_INST(n)                                                                        \
    static const struct behavior_pmw_timeout_config behavior_pmw_timeout_config_##n = {           \
        .timeout_ms = DT_INST_PROP(n, timeout_ms),                                                 \
    };                                                                                             \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_pmw_timeout_init, NULL, NULL,                             \
                            &behavior_pmw_timeout_config_##n, POST_KERNEL,                         \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_pmw_timeout_driver_api);

DT_INST_FOREACH_STATUS_OKAY(PMW_TIMEOUT_INST)

