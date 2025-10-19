/*
 * Copyright (c) 2025 ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_pmw3610_automouse_timeout

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>

#include <zmk/behavior.h>
#include <zmk/keymap.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

// Forward declare the driver API functions
uint32_t pmw3610_get_automouse_timeout_ms(const struct device *dev);
int pmw3610_set_automouse_timeout_ms(const struct device *dev, uint32_t timeout_ms);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

struct behavior_pmw3610_automouse_timeout_config {
    uint32_t min_timeout_ms;
    uint32_t max_timeout_ms;
    uint32_t step_ms;
};

struct behavior_pmw3610_automouse_timeout_data {
    // No runtime data needed
};

// Get the PMW3610 device reference
static const struct device *get_pmw3610_device(void) {
    // Try to get device by label "trackball" first
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(trackball));
    if (device_is_ready(dev)) {
        return dev;
    }
    return NULL;
}

static int behavior_pmw3610_automouse_timeout_init(const struct device *dev) {
    return 0;
}

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    const struct behavior_pmw3610_automouse_timeout_config *cfg = dev->config;

    const struct device *pmw_dev = get_pmw3610_device();
    if (!pmw_dev) {
        LOG_ERR("PMW3610 device not found");
        return -ENODEV;
    }

    int32_t param = binding->param1;
    uint32_t current_timeout = pmw3610_get_automouse_timeout_ms(pmw_dev);
    uint32_t new_timeout = current_timeout;

    // param1 interpretation:
    // > 0: Set absolute timeout value in milliseconds
    // 0: No change (can be used for query)
    // -1: Decrease timeout by step
    // -2: Increase timeout by step
    // -3: Reset to default (from config)

    if (param > 0) {
        // Set absolute value
        new_timeout = (uint32_t)param;
    } else if (param == -1) {
        // Decrease
        if (current_timeout > cfg->step_ms && current_timeout - cfg->step_ms >= cfg->min_timeout_ms) {
            new_timeout = current_timeout - cfg->step_ms;
        } else {
            new_timeout = cfg->min_timeout_ms;
        }
    } else if (param == -2) {
        // Increase
        if (current_timeout + cfg->step_ms <= cfg->max_timeout_ms) {
            new_timeout = current_timeout + cfg->step_ms;
        } else {
            new_timeout = cfg->max_timeout_ms;
        }
    } else if (param == -3) {
        // Reset to default from Kconfig
        #if defined(CONFIG_PMW3610_AUTOMOUSE_TIMEOUT_MS)
        new_timeout = CONFIG_PMW3610_AUTOMOUSE_TIMEOUT_MS;
        #else
        new_timeout = 600; // fallback default
        #endif
    }

    // Clamp to min/max
    if (new_timeout < cfg->min_timeout_ms) {
        new_timeout = cfg->min_timeout_ms;
    }
    if (new_timeout > cfg->max_timeout_ms) {
        new_timeout = cfg->max_timeout_ms;
    }

    // Only set if changed
    if (new_timeout != current_timeout) {
        int ret = pmw3610_set_automouse_timeout_ms(pmw_dev, new_timeout);
        if (ret < 0) {
            LOG_ERR("Failed to set automouse timeout: %d", ret);
            return ret;
        }
        LOG_INF("Automouse timeout changed: %u ms -> %u ms", current_timeout, new_timeout);
    } else {
        LOG_INF("Automouse timeout unchanged: %u ms", current_timeout);
    }

    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                       struct zmk_behavior_binding_event event) {
    // Nothing to do on release
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_pmw3610_automouse_timeout_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
};

#define PMW3610_AUTOMOUSE_TIMEOUT_INST(n)                                                          \
    static struct behavior_pmw3610_automouse_timeout_data behavior_pmw3610_automouse_timeout_data_##n = {}; \
    static struct behavior_pmw3610_automouse_timeout_config behavior_pmw3610_automouse_timeout_config_##n = { \
        .min_timeout_ms = DT_INST_PROP_OR(n, min_timeout_ms, 100),                                 \
        .max_timeout_ms = DT_INST_PROP_OR(n, max_timeout_ms, 5000),                                \
        .step_ms = DT_INST_PROP_OR(n, step_ms, 100),                                               \
    };                                                                                              \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_pmw3610_automouse_timeout_init, NULL,                      \
                            &behavior_pmw3610_automouse_timeout_data_##n,                          \
                            &behavior_pmw3610_automouse_timeout_config_##n, POST_KERNEL,           \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                                   \
                            &behavior_pmw3610_automouse_timeout_driver_api);

DT_INST_FOREACH_STATUS_OKAY(PMW3610_AUTOMOUSE_TIMEOUT_INST)

#endif

