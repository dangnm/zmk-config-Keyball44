# PMW3610 Automouse Timeout Behavior

This custom ZMK behavior allows you to adjust the automouse layer timeout at runtime for keyboards using the PMW3610 trackball sensor.

## Overview

The automouse feature automatically activates a mouse layer when trackball movement is detected. The timeout determines how long the mouse layer stays active after the last movement. This behavior lets you change that timeout on-the-fly without recompiling your firmware.

## Installation

The behavior is already included in this configuration:

1. **pmw3610_automouse_timeout.c** - The behavior implementation
2. **pmw3610_automouse_timeout.dtsi** - Device tree definition
3. **CMakeLists.txt** - Build configuration

The behavior is included in `keyball44.keymap` via:
```c
#include "pmw3610_automouse_timeout.dtsi"
```

## Usage

The behavior is bound with the name `amto` (AutoMouse TimeOut) and takes one parameter:

```
&amto <parameter>
```

### Parameters

| Parameter | Action | Description |
|-----------|--------|-------------|
| Positive value (e.g., `500`, `1000`, `2000`) | Set absolute timeout | Sets the timeout to the specified value in milliseconds |
| `0` | Query | No change, can be used to log current value |
| `-1` | Decrease | Decreases timeout by step amount (default 100ms) |
| `-2` | Increase | Increases timeout by step amount (default 100ms) |
| `-3` | Reset | Resets to default value from `CONFIG_PMW3610_AUTOMOUSE_TIMEOUT_MS` |

### Configuration Options

In the `.dtsi` file, you can configure these properties:

```c
amto: behavior_pmw3610_automouse_timeout {
    compatible = "zmk,behavior-pmw3610-automouse-timeout";
    #binding-cells = <1>;
    display-name = "Auto Mouse Timeout";
    min-timeout-ms = <100>;      // Minimum allowed timeout
    max-timeout-ms = <5000>;     // Maximum allowed timeout
    step-ms = <100>;             // Step size for increment/decrement
};
```

## Examples

### Set Specific Timeout Values

```c
// Set timeout to 500ms
&amto 500

// Set timeout to 1 second
&amto 1000

// Set timeout to 2 seconds
&amto 2000
```

### Increment/Decrement Controls

```c
// Decrease timeout by 100ms (or configured step-ms)
&amto (-1)

// Increase timeout by 100ms (or configured step-ms)
&amto (-2)

// Reset to default from Kconfig
&amto (-3)
```

### Example Keymap Integration

Here's an example from the mouse layer:

```c
mouse_layer {
    label = "MOUSE";
    bindings = <
&trans     &kp N1      &kp N2      &kp N3        &kp N4           &kp N5                      &kp N6         &kp N7        &kp N8          &kp N9     &kp N0           &to 0
&trans     &to 0       &to 0       &to 0         &to 0            &msc SCRL_UP                &kp PAGE_UP    &mkp LCLK     &mkp MCLK       &mkp RCLK  &lt 2 SEMICOLON  &lt 3 SQT
&trans     &none       &none       &kp C_VOL_DN  &kp C_VOLUME_UP  &msc SCRL_DOWN              &kp PAGE_DOWN  &msc SCRL_UP  &msc SCRL_DOWN  &none      &none            &none
&amto 1000 &amto (-2)  &amto (-1)  &trans        &kp LEFT_WIN                         &trans  &trans                                                                   &none
    >;
};
```

In this example:
- Bottom-left key: Sets timeout to 1000ms (1 second)
- Second key: Increases timeout by 100ms
- Third key: Decreases timeout by 100ms

### Practical Use Cases

**Quick Access Setup:**
```c
// Bind to a combo or dedicated key for quick access
combo_timeout_short {
    bindings = <&amto 300>;  // Short timeout for precision work
    key-positions = <...>;
};

combo_timeout_long {
    bindings = <&amto 2000>;  // Long timeout for browsing
    key-positions = <...>;
};
```

**Layer-Specific Timeouts:**
```c
// In your work layer, automatically set a comfortable timeout
work_layer {
    bindings = <
        &amto 1500  // First key sets preferred timeout for this layer
        ...
    >;
};
```

## How It Works

1. The behavior looks up the PMW3610 device using the `trackball` label from your device tree
2. It calls the driver's runtime API functions:
   - `pmw3610_get_automouse_timeout_ms()` - Gets current timeout
   - `pmw3610_set_automouse_timeout_ms()` - Sets new timeout
3. Values are clamped to the configured min/max range
4. Changes are logged for debugging

## Troubleshooting

### Device Not Found
If you see "PMW3610 device not found" in logs:
- Ensure your trackball is defined with label `trackball` in the overlay
- Check that `automouse-layer` is configured in the trackball node

### Value Not Changing
- Check that the new value is within min/max bounds (default 100-5000ms)
- Verify that `automouse-layer` is set to a valid layer number (> 0)
- Look at logs to see if the API call succeeded

### Build Errors
- Ensure `CMakeLists.txt` is in the `config/` directory
- Verify the include path in your keymap matches the `.dtsi` filename
- Check that all three files are present (`.c`, `.dtsi`, `CMakeLists.txt`)

## Technical Details

### Driver Requirements

This behavior requires:
- PMW3610 driver with automouse support (the driver in `zmk-pmw3610-driver`)
- `automouse-layer` property set to a valid layer number in device tree
- `CONFIG_PMW3610_AUTOMOUSE_TIMEOUT_MS` defined in Kconfig

### Memory and Performance

- Minimal memory footprint (no dynamic allocation)
- Zero-cost when not invoked
- Timeout changes take effect immediately
- Thread-safe access to driver API

## API Reference

### Behavior Binding

```c
&amto <param1>
```

- **param1**: int32_t value controlling the timeout adjustment
  - `> 0`: Absolute timeout in milliseconds
  - `0`: No change (query current value)
  - `-1`: Decrease by step
  - `-2`: Increase by step  
  - `-3`: Reset to default

### Device Tree Properties

| Property | Type | Default | Description |
|----------|------|---------|-------------|
| `compatible` | string | Required | Must be `"zmk,behavior-pmw3610-automouse-timeout"` |
| `#binding-cells` | int | Required | Must be `<1>` |
| `min-timeout-ms` | int | `100` | Minimum timeout in milliseconds |
| `max-timeout-ms` | int | `5000` | Maximum timeout in milliseconds |
| `step-ms` | int | `100` | Increment/decrement step in milliseconds |

## See Also

- [PMW3610 Driver Documentation](../../zmk-pmw3610-driver/README.md)
- [ZMK Behaviors Documentation](https://zmk.dev/docs/behaviors)
- [Device Tree Configuration](https://zmk.dev/docs/config/system)

