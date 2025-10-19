# Runtime Automouse Timeout - Feature Summary

## What Was Created

This feature adds the ability to adjust the PMW3610 automouse layer timeout at runtime, allowing users to customize the trackball behavior without recompiling firmware.

## Files Created/Modified

### Core Implementation Files

| File | Purpose | Lines |
|------|---------|-------|
| `config/pmw3610_automouse_timeout.c` | Behavior implementation in C | ~140 |
| `config/pmw3610_automouse_timeout.dtsi` | Device tree binding definition | ~18 |
| `config/CMakeLists.txt` | Build configuration | ~5 |

### Documentation Files

| File | Purpose | Lines |
|------|---------|-------|
| `config/AUTOMOUSE_TIMEOUT_BEHAVIOR.md` | Complete user documentation | ~300 |
| `config/automouse_timeout_examples.keymap` | Usage examples and patterns | ~230 |
| `config/QUICK_REFERENCE.md` | Quick reference card | ~100 |
| `config/IMPLEMENTATION_SUMMARY.md` | Technical implementation details | ~400 |
| `CHANGELOG.md` | Version history and changes | ~200 |
| `FEATURE_SUMMARY.md` | This file | ~150 |

### Modified Files

| File | Changes |
|------|---------|
| `config/keyball44.keymap` | Added include directive and example bindings |
| `README.md` | Added feature documentation section |

## Total Impact

- **New Files**: 9
- **Modified Files**: 2
- **Total Lines Added**: ~1,550+
- **Code**: ~165 lines
- **Documentation**: ~1,385 lines

## Feature Capabilities

### Basic Operations

✅ Set absolute timeout values (100-5000ms)  
✅ Increment timeout by configurable step (default 100ms)  
✅ Decrement timeout by configurable step  
✅ Reset to Kconfig default value  
✅ Automatic bounds checking  
✅ Immediate effect (no reboot needed)  

### Advanced Features

✅ Configurable min/max bounds per behavior instance  
✅ Configurable step size per instance  
✅ Multiple behavior instances with different settings  
✅ Integration with macros  
✅ Integration with hold-tap behaviors  
✅ Integration with combos  
✅ Thread-safe operation  
✅ Comprehensive error handling  
✅ Debug logging support  

## Usage Examples Provided

### 1. Direct Key Bindings
Simple key presses to control timeout

### 2. Combo Bindings
Quick access without dedicated keys

### 3. Macro Integration
Combine timeout changes with layer switches

### 4. Hold-Tap Integration
Dual-purpose keys (tap for key, hold for timeout adjust)

### 5. Context-Aware Switching
Auto-adjust timeout when entering specific layers

### 6. Multi-Instance Configurations
Fine and coarse adjustment behaviors

### 7. Preset Values
Common timeout values for different use cases

## API Reference

### Behavior Binding

```c
&amto <param>
```

### Parameters

| Value | Action |
|-------|--------|
| `> 0` | Set absolute timeout in ms |
| `0` | Query (no change) |
| `-1` | Decrease by step |
| `-2` | Increase by step |
| `-3` | Reset to default |

### Configuration Properties

```c
min-timeout-ms = <100>;    // Minimum allowed (ms)
max-timeout-ms = <5000>;   // Maximum allowed (ms)
step-ms = <100>;           // Increment/decrement step (ms)
```

## Dependencies

### From Driver

Required API functions from `zmk-pmw3610-driver`:

```c
uint32_t pmw3610_get_automouse_timeout_ms(const struct device *dev);
int pmw3610_set_automouse_timeout_ms(const struct device *dev, uint32_t timeout_ms);
```

### Device Tree Requirements

```c
trackball: trackball@0 {
    compatible = "pixart,pmw3610";
    automouse-layer = <4>;  // Must be > 0
    ...
};
```

### Kconfig Requirements

```
CONFIG_PMW3610_AUTOMOUSE_TIMEOUT_MS=600  // Default value
```

## Example Integration

### Mouse Layer (keyball44.keymap)

```c
mouse_layer {
    label = "MOUSE";
    bindings = <
&trans     &kp N1      &kp N2      &kp N3        &kp N4           &kp N5          ...
&trans     &to 0       &to 0       &to 0         &to 0            &msc SCRL_UP    ...
&trans     &none       &none       &kp C_VOL_DN  &kp C_VOLUME_UP  &msc SCRL_DOWN  ...
&amto 1000 &amto (-2)  &amto (-1)  &trans        &kp LEFT_WIN                     ...
//  ^         ^           ^
//  Set 1s   Increase   Decrease
    >;
};
```

## Testing Checklist

- [x] Code compiles without errors
- [x] No linting issues
- [x] Documentation complete
- [x] Examples provided
- [x] Quick reference created
- [x] Technical details documented
- [x] Changelog updated
- [x] README updated
- [x] Error handling implemented
- [x] Bounds checking implemented
- [x] Thread safety considered
- [x] Memory footprint minimized
- [x] Zero-cost when inactive

## Documentation Structure

```
zmk-config-Keyball44/
├── README.md                                    # ✅ Updated with feature info
├── CHANGELOG.md                                 # ✅ New - version history
├── FEATURE_SUMMARY.md                           # ✅ New - this file
└── config/
    ├── pmw3610_automouse_timeout.c              # ✅ New - implementation
    ├── pmw3610_automouse_timeout.dtsi           # ✅ New - device tree
    ├── CMakeLists.txt                           # ✅ New - build config
    ├── keyball44.keymap                         # ✅ Modified - added bindings
    ├── AUTOMOUSE_TIMEOUT_BEHAVIOR.md            # ✅ New - full docs
    ├── automouse_timeout_examples.keymap        # ✅ New - examples
    ├── QUICK_REFERENCE.md                       # ✅ New - quick ref
    └── IMPLEMENTATION_SUMMARY.md                # ✅ New - tech details
```

## Quick Start Guide

### For End Users

1. **Use existing bindings** in mouse layer (layer 4):
   - Bottom-left: Set to 1 second
   - Second key: Increase timeout
   - Third key: Decrease timeout

2. **Customize** by editing `keyball44.keymap`

3. **Learn more** from `AUTOMOUSE_TIMEOUT_BEHAVIOR.md`

### For Developers

1. **Read** `IMPLEMENTATION_SUMMARY.md` for technical details

2. **Review** `pmw3610_automouse_timeout.c` for implementation

3. **Extend** by creating additional behavior instances

4. **Test** using the validation checklist

## Recommended Timeout Values

| Timeout | Use Case |
|---------|----------|
| 200ms | Gaming, fast navigation |
| 400ms | Precision work (CAD, design) |
| 600ms | General use (default) |
| 1000ms | Comfortable browsing |
| 1500ms | Relaxed use, media |
| 2000ms | Extended activities |

## Troubleshooting Resources

1. **Quick Reference**: `QUICK_REFERENCE.md`
2. **Full Documentation**: `AUTOMOUSE_TIMEOUT_BEHAVIOR.md` (Troubleshooting section)
3. **Implementation Details**: `IMPLEMENTATION_SUMMARY.md` (Debugging section)

## Support

For questions or issues:
1. Check the documentation files
2. Review examples in `automouse_timeout_examples.keymap`
3. Enable debug logging (see Implementation Summary)
4. Open an issue in the repository

## Future Roadmap

Potential enhancements:
- [ ] Display current timeout value (e.g., via OLED)
- [ ] Save/load preset profiles
- [ ] RGB indicator integration
- [ ] Automatic adjustment based on usage
- [ ] Support for multiple trackballs

## Credits

- **Driver API**: zmk-pmw3610-driver
- **ZMK Framework**: ZMK Contributors
- **Keyball44**: yangxing844, delock, Amos698

## License

MIT License - Same as ZMK firmware

---

**Status**: ✅ Complete and Ready for Use  
**Version**: 1.0  
**Date**: 2025-10-19

