# PMW3610 Auto Mouse Timeout Control

This custom behavior allows you to dynamically change the auto mouse layer timeout at runtime using keyboard keys.

## Overview

The PMW3610 driver supports an auto mouse layer feature that automatically activates a mouse layer when the trackball is moved. This behavior provides runtime control over how long the mouse layer stays active after trackball movement stops.

## Features

- **Runtime Configuration**: Change the timeout value without rebuilding firmware
- **Three Preset Options**: 1000ms (1 second), 2000ms (2 seconds), and 5000ms (5 seconds)
- **Easy to Use**: Simply press a key to apply the timeout

## Implementation

### Files Created

1. **behavior_pmw_timeout.c** - Custom ZMK behavior implementation
2. **behavior_pmw_timeout.dtsi** - Device tree definitions for the three timeout behaviors
3. **dts/bindings/zmk,behavior-pmw-timeout.yaml** - Device tree binding specification
4. **CMakeLists.txt** - Build configuration

### How It Works

The custom behavior uses the PMW3610 driver API functions:
- `pmw3610_get_automouse_timeout_ms(device)` - Get current timeout
- `pmw3610_set_automouse_timeout_ms(device, timeout_ms)` - Set new timeout

When you press a key mapped to one of the timeout behaviors, it immediately updates the auto mouse layer timeout value.

## Usage

### In Your Keymap

The timeout behaviors are already defined in `behavior_pmw_timeout.dtsi`:

- `&pmw_timeout_1000` - Set timeout to 1 second (1000ms)
- `&pmw_timeout_2000` - Set timeout to 2 seconds (2000ms)  
- `&pmw_timeout_5000` - Set timeout to 5 seconds (5000ms)

### Example Keymap Integration

```c
#include <boards/shields/keyball_nano/behavior_pmw_timeout.dtsi>

/ {
    keymap {
        compatible = "zmk,keymap";
        
        layer_name {
            bindings = <
                // ... other keys ...
                &pmw_timeout_1000  // Press to set 1 second timeout
                &pmw_timeout_2000  // Press to set 2 second timeout
                &pmw_timeout_5000  // Press to set 5 second timeout
                // ... other keys ...
            >;
        };
    };
};
```

### Current Implementation

In the provided keymap, the timeout controls are placed on Layer 3 (extended layer), third row:
- Position 7: 1000ms timeout
- Position 8: 2000ms timeout
- Position 9: 5000ms timeout

## Customization

### Adding Different Timeout Values

To add custom timeout values, edit `behavior_pmw_timeout.dtsi`:

```c
/ {
    behaviors {
        pmw_timeout_custom: pmw_timeout_custom {
            compatible = "zmk,behavior-pmw-timeout";
            #binding-cells = <0>;
            timeout-ms = <3000>;  // Your custom value in milliseconds
        };
    };
};
```

Then use `&pmw_timeout_custom` in your keymap.

### Modifying Existing Values

Simply change the `timeout-ms` property in the behavior definition.

## Default Timeout

The default timeout value is set via `CONFIG_PMW3610_AUTOMOUSE_TIMEOUT_MS` in your configuration file. The runtime control allows you to override this default temporarily.

## Troubleshooting

### Behavior Not Working

1. **Check device readiness**: Ensure the PMW3610 sensor is properly initialized
2. **Verify device tree label**: The trackball device must have the label `trackball` in your overlay
3. **Check logs**: Monitor ZMK logs for error messages

### Invalid Timeout Values

- Timeout values must be greater than 0
- Very small values (<100ms) may cause rapid layer switching
- Very large values (>10000ms) may feel unresponsive

## Technical Details

### Device Reference

The behavior obtains the PMW3610 device reference using:
```c
const struct device *pmw_dev = DEVICE_DT_GET(DT_NODELABEL(trackball));
```

This requires your device tree overlay to define the trackball with the label `trackball`, as shown in `keyball44_right.overlay`:

```dts
trackball: trackball@0 {
    compatible = "pixart,pmw3610";
    automouse-layer = <4>;
    // ... other properties ...
};
```

### API Reference

Based on the PMW3610 driver API ([PR #2](https://github.com/dangnm/zmk-pmw3610-driver/pull/2/files)):

```c
// Get current timeout value
uint32_t pmw3610_get_automouse_timeout_ms(const struct device *dev);

// Set new timeout value (returns 0 on success, -EINVAL if timeout_ms is 0)
int pmw3610_set_automouse_timeout_ms(const struct device *dev, uint32_t timeout_ms);
```

## License

This implementation follows the ZMK MIT license.

