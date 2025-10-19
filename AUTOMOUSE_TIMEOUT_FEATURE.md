# Auto Mouse Timeout Runtime Control Feature

## Summary

This feature adds runtime control for the PMW3610 trackball's auto mouse layer timeout. You can now change how long the mouse layer stays active after trackball movement by simply pressing a key, without rebuilding the firmware.

## What Was Implemented

### 1. Custom ZMK Behavior
A new custom behavior (`zmk,behavior-pmw-timeout`) was created that interfaces with the PMW3610 driver API to change the auto mouse timeout at runtime.

**Files Created:**
- `config/boards/shields/keyball_nano/behavior_pmw_timeout.c` - Implementation
- `config/boards/shields/keyball_nano/behavior_pmw_timeout.dtsi` - Device tree definitions
- `config/boards/shields/keyball_nano/dts/bindings/zmk,behavior-pmw-timeout.yaml` - Binding spec
- `config/boards/shields/keyball_nano/CMakeLists.txt` - Build configuration

### 2. Three Preset Timeout Options
Three behavior instances are pre-configured and ready to use:

| Behavior Name | Timeout Value | Description |
|---------------|---------------|-------------|
| `&pmw_timeout_1000` | 1000ms (1 second) | Quick timeout - layer deactivates quickly |
| `&pmw_timeout_2000` | 2000ms (2 seconds) | Medium timeout - **default** |
| `&pmw_timeout_5000` | 5000ms (5 seconds) | Long timeout - layer stays active longer |

### 3. Keymap Integration
The timeout controls have been added to **Layer 3 (Extended Layer)**, on the third row of the right side:

```
Layer 3 - Extended Layer (right side, row 3):
┌─────┬─────┬─────┬─────┬─────┬─────┐
│  _  │  &  │  *  │  (  │  )  │none │
├─────┼─────┼─────┼─────┼─────┼─────┤
│  -  │  =  │  [  │  ]  │ ;/2 │ '/3 │
├─────┼─────┼─────┼─────┼─────┼─────┤
│ 1s  │ 2s  │ 5s  │trans│  \  │none │  ← Auto mouse timeout controls
└─────┴─────┴─────┴─────┴─────┴─────┘
```

**To access:** Hold the key that activates Layer 3, then press one of the timeout control keys.

## How to Use

### Quick Start
1. Access Layer 3 (typically by holding a layer modifier key)
2. Press one of the timeout control keys:
   - **First key**: Set to 1 second timeout
   - **Second key**: Set to 2 second timeout  
   - **Third key**: Set to 5 second timeout
3. The new timeout takes effect immediately

### Example Scenario
Let's say you're doing detailed work with the trackball:

1. **Default**: Timeout is 2 seconds (configured in `keyball44_right.conf`)
2. **Switch to 5 seconds**: Hold Layer 3 key → Press the 5s timeout key
3. Now the mouse layer stays active for 5 seconds after you stop moving the trackball
4. **Switch back to 1 second**: Hold Layer 3 key → Press the 1s timeout key
5. Now the mouse layer deactivates after just 1 second

## Technical Details

### API Used
Based on the [PMW3610 driver PR #2](https://github.com/dangnm/zmk-pmw3610-driver/pull/2/files):

```c
// Set the auto mouse timeout
int pmw3610_set_automouse_timeout_ms(const struct device *dev, uint32_t timeout_ms);

// Get the current timeout
uint32_t pmw3610_get_automouse_timeout_ms(const struct device *dev);
```

### Device Reference
The behavior obtains the PMW3610 device using:
```c
const struct device *pmw_dev = DEVICE_DT_GET(DT_NODELABEL(trackball));
```

This works because the trackball is defined in `keyball44_right.overlay` with the label `trackball`:
```dts
trackball: trackball@0 {
    compatible = "pixart,pmw3610";
    automouse-layer = <4>;  // Mouse layer is layer 4
    scroll-layers = <5>;
    snipe-layers = <6>;
    ...
};
```

### Default Timeout
The default timeout on boot is configured in `keyball44_right.conf`:
```
CONFIG_PMW3610_AUTOMOUSE_TIMEOUT_MS=2000
```

## Customization

### Adding Different Timeout Values

Edit `config/boards/shields/keyball_nano/behavior_pmw_timeout.dtsi` and add:

```c
/ {
    behaviors {
        // Add your custom timeout
        pmw_timeout_3500: pmw_timeout_3500 {
            compatible = "zmk,behavior-pmw-timeout";
            #binding-cells = <0>;
            timeout-ms = <3500>;  // 3.5 seconds
        };
    };
};
```

Then use `&pmw_timeout_3500` in your keymap.

### Moving the Keys to a Different Layer

Edit `config/keyball44.keymap` and place the timeout behaviors where you prefer:

```c
your_layer {
    bindings = <
        // ... place anywhere ...
        &pmw_timeout_1000  &pmw_timeout_2000  &pmw_timeout_5000
        // ...
    >;
};
```

## Benefits

1. **No Firmware Rebuild**: Change timeout on the fly
2. **Quick Adaptation**: Adjust for different tasks (gaming, design work, general use)
3. **Experimentation**: Try different values to find what works best
4. **Context-Specific**: Use different timeouts for different scenarios

## Troubleshooting

### Keys Don't Work
- Ensure you're pressing them while on Layer 3
- Check that the firmware compiled successfully
- Verify the PMW3610 sensor is working (test trackball movement)

### No Visible Effect
- The timeout only affects how long the mouse layer stays active after movement stops
- Try moving the trackball and watching which layer is active
- Check the default timeout isn't the same as the one you're setting

### Build Errors
- Ensure the CMakeLists.txt is in the correct location
- Verify all file paths in the includes are correct
- Check that the device tree binding is properly formatted

## Files Modified

1. `config/keyball44.keymap` - Added include and key bindings
2. `config/boards/shields/keyball_nano/behavior_pmw_timeout.c` - Created
3. `config/boards/shields/keyball_nano/behavior_pmw_timeout.dtsi` - Created
4. `config/boards/shields/keyball_nano/dts/bindings/zmk,behavior-pmw-timeout.yaml` - Created
5. `config/boards/shields/keyball_nano/CMakeLists.txt` - Created

## Future Enhancements

Possible improvements:
- Add visual feedback (OLED display showing current timeout)
- Create a behavior that cycles through preset values
- Add behaviors for increment/decrement timeout
- Store selected timeout in settings (persist across reboots)

## Credits

Based on the auto mouse timeout runtime API added in [PR #2](https://github.com/dangnm/zmk-pmw3610-driver/pull/2) of the zmk-pmw3610-driver by [@dangnm](https://github.com/dangnm).

## License

MIT License (following ZMK licensing)

