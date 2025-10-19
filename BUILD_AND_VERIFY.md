# Build and Verification Guide

## Summary of Changes

This document lists all files created/modified for the auto mouse timeout runtime control feature.

## Files Created

### 1. Behavior Implementation
```
config/boards/shields/keyball_nano/behavior_pmw_timeout.c
```
- Custom ZMK behavior that calls PMW3610 driver API
- Handles key press to set timeout value
- Includes error handling and logging

### 2. Device Tree Include
```
config/boards/shields/keyball_nano/behavior_pmw_timeout.dtsi
```
- Defines three behavior instances:
  - `pmw_timeout_1000` (1 second)
  - `pmw_timeout_2000` (2 seconds)
  - `pmw_timeout_5000` (5 seconds)

### 3. Device Tree Binding
```
config/boards/shields/keyball_nano/dts/bindings/zmk,behavior-pmw-timeout.yaml
```
- Binding specification for the custom behavior
- Defines `timeout-ms` property

### 4. Build Configuration
```
config/boards/shields/keyball_nano/CMakeLists.txt
```
- Adds behavior_pmw_timeout.c to build
- Configures include directories
- Sets up DTS_ROOT for custom bindings

### 5. Documentation Files
```
AUTOMOUSE_TIMEOUT_FEATURE.md          - Comprehensive feature documentation
QUICK_REFERENCE_TIMEOUT.md            - Quick reference guide
config/boards/shields/keyball_nano/AUTO_MOUSE_TIMEOUT_README.md - Implementation details
BUILD_AND_VERIFY.md                   - This file
```

## Files Modified

### 1. Keymap
```
config/keyball44.keymap
```
**Changes:**
- Added include: `#include <boards/shields/keyball_nano/behavior_pmw_timeout.dtsi>`
- Modified Layer 3 (extended_layer) to add three timeout control keys
- Added comment explaining the timeout controls

### 2. README
```
README.md
```
**Changes:**
- Added Features section
- Added documentation links
- Added credit to driver PR

## Build Process

### Prerequisites
- ZMK build environment set up
- west configured with zmk-pmw3610-driver repository
- Repository already points to correct driver branch (auto_mouse_timeout_runtime)

### Verify Configuration

Check that `config/west.yml` contains:
```yaml
projects:
  - name: zmk-pmw3610-driver
    remote: dangnm
    revision: auto_mouse_timeout_runtime
```

✅ **Status:** Already configured correctly

### Build Commands

For GitHub Actions (recommended):
```bash
# Push to repository
git add .
git commit -m "Add auto mouse timeout runtime control"
git push
```

GitHub Actions will automatically build the firmware.

For local build:
```bash
cd zmk/app
west build -d build/keyball44_left -b nice_nano_v2 -- -DSHIELD=keyball44_left -DZMK_CONFIG=/path/to/zmk-config-Keyball44/config
west build -d build/keyball44_right -b nice_nano_v2 -- -DSHIELD=keyball44_right -DZMK_CONFIG=/path/to/zmk-config-Keyball44/config
```

## Verification Checklist

### Pre-Build Verification
- [ ] All new files are created in correct locations
- [ ] CMakeLists.txt exists in shield directory
- [ ] Device tree binding YAML is properly formatted
- [ ] Keymap includes the behavior dtsi file
- [ ] No syntax errors in C code

### Post-Build Verification
- [ ] Build completes without errors
- [ ] No warnings related to pmw_timeout behavior
- [ ] Firmware files generated successfully
- [ ] File sizes are reasonable (not bloated)

### Flash and Test
- [ ] Flash firmware to both halves
- [ ] Keyboard boots successfully
- [ ] Trackball works normally
- [ ] Can access Layer 3
- [ ] Timeout keys respond to presses
- [ ] Check logs for confirmation messages

## Expected Log Output

When you press a timeout key, you should see in the logs:
```
[00:XX:XX.XXX] <inf> zmk: Auto mouse timeout set to XXXX ms
```

If there's an error:
```
[00:XX:XX.XXX] <err> zmk: PMW3610 device not ready
```
or
```
[00:XX:XX.XXX] <err> zmk: Failed to set timeout: -EINVAL
```

## Testing the Feature

### Test Plan

1. **Initial State**
   - [ ] Boot keyboard
   - [ ] Move trackball
   - [ ] Verify mouse layer activates (Layer 4)
   - [ ] Stop moving
   - [ ] Time how long until layer deactivates (should be ~2s default)

2. **Set 1 Second Timeout**
   - [ ] Hold Layer 3 key
   - [ ] Press timeout key #1 (position 7 on right side)
   - [ ] Release Layer 3 key
   - [ ] Move trackball
   - [ ] Stop moving
   - [ ] Verify layer deactivates after ~1 second

3. **Set 5 Second Timeout**
   - [ ] Hold Layer 3 key
   - [ ] Press timeout key #3 (position 9 on right side)
   - [ ] Release Layer 3 key
   - [ ] Move trackball
   - [ ] Stop moving
   - [ ] Verify layer stays active for ~5 seconds

4. **Set 2 Second Timeout (Reset to Default)**
   - [ ] Hold Layer 3 key
   - [ ] Press timeout key #2 (position 8 on right side)
   - [ ] Release Layer 3 key
   - [ ] Move trackball
   - [ ] Stop moving
   - [ ] Verify layer deactivates after ~2 seconds

### Visual Indicators

Watch for layer changes via:
- OLED display (if showing current layer)
- LED indicators (if configured)
- Behavior of mouse movement keys
- ZMK Studio layer view (if enabled)

## Troubleshooting

### Build Errors

**Error:** `zmk,behavior-pmw-timeout` not found
- **Fix:** Check that binding YAML is in correct location
- **Fix:** Verify DTS_ROOT is set in CMakeLists.txt

**Error:** `pmw3610.h` not found
- **Fix:** Ensure zmk-pmw3610-driver is in west.yml
- **Fix:** Run `west update` to fetch dependencies
- **Fix:** Verify driver branch is `auto_mouse_timeout_runtime`

**Error:** Undefined reference to `pmw3610_set_automouse_timeout_ms`
- **Fix:** Ensure driver is version with runtime API (PR #2)
- **Fix:** Clean build and rebuild

**Error:** `DEVICE_DT_GET(DT_NODELABEL(trackball))` fails
- **Fix:** Verify trackball node exists in overlay
- **Fix:** Check node label is exactly "trackball"

### Runtime Issues

**Issue:** Keys don't respond
- **Check:** Are you on Layer 3?
- **Check:** Is trackball functioning normally?
- **Check:** Check firmware version and build logs

**Issue:** No visible effect
- **Check:** Time the actual timeout with a timer
- **Check:** Ensure auto mouse layer is configured (layer 4)
- **Check:** Verify automouse-layer property in overlay

**Issue:** Timeout doesn't persist after reboot
- **Expected:** This is by design
- **Reason:** Runtime control doesn't modify persistent config
- **Workaround:** Change `CONFIG_PMW3610_AUTOMOUSE_TIMEOUT_MS` in config file for permanent change

## File Structure Overview

```
zmk-config-Keyball44/
├── config/
│   ├── boards/
│   │   └── shields/
│   │       └── keyball_nano/
│   │           ├── behavior_pmw_timeout.c          ← New
│   │           ├── behavior_pmw_timeout.dtsi       ← New
│   │           ├── CMakeLists.txt                  ← New
│   │           ├── AUTO_MOUSE_TIMEOUT_README.md    ← New
│   │           ├── dts/
│   │           │   └── bindings/
│   │           │       └── zmk,behavior-pmw-timeout.yaml  ← New
│   │           ├── keyball44.dtsi
│   │           ├── keyball44_left.overlay
│   │           ├── keyball44_right.overlay         
│   │           ├── keyball44_right.conf
│   │           └── ... (other files)
│   ├── keyball44.keymap                            ← Modified
│   ├── keyball44.conf
│   └── west.yml
├── AUTOMOUSE_TIMEOUT_FEATURE.md                    ← New
├── QUICK_REFERENCE_TIMEOUT.md                      ← New
├── BUILD_AND_VERIFY.md                             ← New (this file)
├── README.md                                       ← Modified
└── ... (other files)
```

## Success Criteria

✅ **Feature is working correctly if:**
1. Firmware builds without errors
2. Keyboard functions normally
3. Timeout keys are responsive
4. Timeout changes take effect immediately
5. Different timeouts are clearly distinguishable
6. No regressions in other keyboard functions

## Additional Notes

- The timeout value resets to default (2000ms) on power cycle
- Changes are only in RAM, not persistent storage
- Feature only works when trackball is on right side
- Requires driver version with runtime API support

## Support

If you encounter issues:
1. Check the troubleshooting section above
2. Review the comprehensive documentation in `AUTOMOUSE_TIMEOUT_FEATURE.md`
3. Verify all files are in correct locations
4. Check ZMK and driver versions
5. Review build logs for specific errors

## References

- [ZMK Documentation](https://zmk.dev/)
- [PMW3610 Driver PR #2](https://github.com/dangnm/zmk-pmw3610-driver/pull/2)
- [Device Tree Bindings Guide](https://zmk.dev/docs/development/new-behavior)

