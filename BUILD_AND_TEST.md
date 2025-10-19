# Build and Test Guide

This guide will help you build and test the new runtime automouse timeout control feature.

## Prerequisites

- West workspace set up with ZMK
- Nice Nano v2 (or compatible board)
- Keyball44 hardware
- PMW3610 trackball sensor

## Build Instructions

### 1. Clean Build

```bash
# Navigate to the zmk directory
cd path/to/zmk

# Clean any previous builds
west build -t clean

# Build for right side (with trackball)
west build -p -b nice_nano_v2 -- -DSHIELD=keyball44_right -DZMK_CONFIG="path/to/zmk-config-Keyball44/config"

# Build for left side
west build -p -b nice_nano_v2 -- -DSHIELD=keyball44_left -DZMK_CONFIG="path/to/zmk-config-Keyball44/config"
```

### 2. Expected Build Output

✅ **Success indicators:**
```
-- west build: building application
[...]
Memory region         Used Size  Region Size  %age Used
           FLASH:      xxxxxx B       xxx KB     xx.xx%
             RAM:      xxxxxx B       xxx KB     xx.xx%
[...]
[100%] Built target zephyr
```

❌ **Common errors and solutions:**

| Error | Solution |
|-------|----------|
| `CMakeLists.txt not found` | Ensure CMakeLists.txt is in config/ directory |
| `Cannot find file pmw3610_automouse_timeout.dtsi` | Check include path in keymap |
| `undefined reference to pmw3610_get_automouse_timeout_ms` | Ensure driver is up to date |
| `device not found: trackball` | Check device tree overlay |

## Flashing

### UF2 Method (Recommended)

1. Put controller in bootloader mode (double-tap reset)
2. Copy `build/zephyr/zmk.uf2` to the USB drive
3. Wait for automatic disconnect
4. Repeat for other side

### Command Line

```bash
west flash
```

## Testing

### 1. Serial Console Setup

**Windows:**
```powershell
# Find COM port
Get-WmiObject Win32_SerialPort | Select Name,DeviceID

# Connect using PuTTY or:
mode COM3 BAUD=115200 PARITY=N DATA=8
```

**Linux/Mac:**
```bash
# Find device
ls /dev/tty*

# Connect
screen /dev/ttyACM0 115200
# or
minicom -D /dev/ttyACM0 -b 115200
```

### 2. Enable Debug Logging (Optional)

Add to `config/keyball44.conf`:
```
CONFIG_ZMK_LOG_LEVEL_DBG=y
CONFIG_LOG_MODE_IMMEDIATE=y
```

Rebuild and reflash.

### 3. Functional Tests

#### Test 1: Set Absolute Timeout

1. Navigate to mouse layer (layer 4)
2. Press bottom-left key (`&amto 1000`)
3. **Expected log:**
   ```
   <inf> zmk: Automouse timeout changed: 600 ms -> 1000 ms
   ```
4. Move trackball
5. Stop moving
6. **Expected:** Mouse layer stays active for ~1 second

✅ **Pass:** Layer deactivates after 1 second  
❌ **Fail:** Check logs for errors

#### Test 2: Increase Timeout

1. Press second key (`&amto (-2)`)
2. **Expected log:**
   ```
   <inf> zmk: Automouse timeout changed: 1000 ms -> 1100 ms
   ```
3. Move trackball, then stop
4. **Expected:** Layer stays active for ~1.1 seconds

✅ **Pass:** Timeout increased by 100ms  
❌ **Fail:** Check min/max bounds

#### Test 3: Decrease Timeout

1. Press third key (`&amto (-1)`)
2. **Expected log:**
   ```
   <inf> zmk: Automouse timeout changed: 1100 ms -> 1000 ms
   ```
3. Move trackball, then stop
4. **Expected:** Layer stays active for ~1 second

✅ **Pass:** Timeout decreased by 100ms  
❌ **Fail:** Check step configuration

#### Test 4: Bounds Checking

1. Press decrease key 20 times
2. **Expected:** Timeout stops at min_timeout_ms (100ms)
3. **Expected log:**
   ```
   <inf> zmk: Automouse timeout changed: 200 ms -> 100 ms
   <inf> zmk: Automouse timeout unchanged: 100 ms
   ```

✅ **Pass:** Cannot go below 100ms  
❌ **Fail:** Review bounds logic

#### Test 5: Reset to Default

1. Press `&amto (-3)` (or add this binding temporarily)
2. **Expected log:**
   ```
   <inf> zmk: Automouse timeout changed: xxx ms -> 600 ms
   ```

✅ **Pass:** Returns to Kconfig default (600ms)  
❌ **Fail:** Check CONFIG_PMW3610_AUTOMOUSE_TIMEOUT_MS

### 4. Integration Tests

#### Test 6: Layer Switching

1. Switch between layers
2. Adjust timeout in mouse layer
3. Switch away and back
4. **Expected:** Timeout setting persists

✅ **Pass:** Timeout maintained across layer changes  
❌ **Fail:** Check device reference

#### Test 7: Bluetooth Reconnection

1. Set custom timeout
2. Disconnect Bluetooth
3. Reconnect
4. **Expected:** Timeout resets to default

✅ **Pass:** Default restored after power cycle  
❌ **Fail:** This is expected behavior (no persistence across reboots)

#### Test 8: Split Keyboard Sync

1. Adjust timeout on right side (with trackball)
2. **Expected:** Only right side affected
3. Left side should function normally

✅ **Pass:** Independent operation  
❌ **Fail:** Check split configuration

## Performance Testing

### Memory Usage

Check build output:
```
Memory region         Used Size  Region Size  %age Used
           FLASH:      xxxxxx B       xxx KB     xx.xx%
```

Compare with previous build without feature:
- Expected increase: ~1-2KB flash
- RAM impact: Negligible

### Responsiveness

1. Rapidly press timeout adjustment keys
2. **Expected:** Immediate response, no lag
3. **Expected:** No missed keypresses

✅ **Pass:** Responsive operation  
❌ **Fail:** Check execution time

## Troubleshooting

### Build Issues

**Issue:** CMakeLists.txt not found
```bash
# Verify file location
ls -la config/CMakeLists.txt

# Should show:
# -rw-r--r-- ... config/CMakeLists.txt
```

**Issue:** Include file not found
```bash
# Verify dtsi file
ls -la config/pmw3610_automouse_timeout.dtsi

# Check include in keymap
grep "pmw3610_automouse_timeout.dtsi" config/keyball44.keymap
```

### Runtime Issues

**Issue:** Device not found

Check device tree overlay:
```bash
grep -A10 "trackball:" config/boards/shields/keyball_nano/keyball44_right.overlay
```

Should show:
```
trackball: trackball@0 {
    status = "okay";
    compatible = "pixart,pmw3610";
    automouse-layer = <4>;
    ...
};
```

**Issue:** No effect

1. Enable debug logging
2. Check automouse-layer is not 0
3. Verify timeout value is changing in logs
4. Test with extreme values (200ms vs 2000ms)

**Issue:** Build warnings

Review warnings carefully:
- Unused variable warnings: OK (may be conditional)
- Missing prototype warnings: Fix by checking includes
- Type mismatch warnings: Fix immediately

## Validation Checklist

### Pre-Flight

- [ ] All files present in config/ directory
- [ ] No build errors
- [ ] No build warnings (or understood/acceptable)
- [ ] Flash size within limits

### Basic Functionality

- [ ] Can set absolute timeout value
- [ ] Can increase timeout
- [ ] Can decrease timeout
- [ ] Can reset to default
- [ ] Bounds are enforced (min/max)
- [ ] Log messages appear correctly

### Advanced Functionality

- [ ] Multiple key presses work correctly
- [ ] Layer switching doesn't affect timeout
- [ ] Rapid key presses handled properly
- [ ] No memory leaks (stable over time)
- [ ] No performance degradation

### User Experience

- [ ] Behavior is intuitive
- [ ] Visual feedback (logs) is clear
- [ ] Documentation is accessible
- [ ] Examples are helpful

## Success Criteria

✅ **Ready for use when:**

1. ✅ Build completes without errors
2. ✅ All functional tests pass
3. ✅ No linter errors
4. ✅ Memory usage acceptable
5. ✅ Performance is responsive
6. ✅ Documentation is complete
7. ✅ Examples are provided

## Reporting Issues

If you encounter problems:

1. **Check documentation first:**
   - AUTOMOUSE_TIMEOUT_BEHAVIOR.md
   - IMPLEMENTATION_SUMMARY.md
   - QUICK_REFERENCE.md

2. **Gather information:**
   - Build output (with errors)
   - Serial console logs
   - Board and shield configuration
   - ZMK version and commit

3. **Create detailed report:**
   - What you expected
   - What actually happened
   - Steps to reproduce
   - Relevant logs and errors

## Next Steps

After successful testing:

1. **Customize bindings** to your preference
2. **Try advanced patterns** from examples
3. **Adjust configuration** (min/max/step) if needed
4. **Share feedback** with the community

## Quick Test Script

```bash
#!/bin/bash
# Quick validation script

echo "=== Keyball44 Automouse Timeout - Build Test ==="

echo "Checking files..."
[ -f config/CMakeLists.txt ] && echo "✅ CMakeLists.txt" || echo "❌ CMakeLists.txt"
[ -f config/pmw3610_automouse_timeout.c ] && echo "✅ .c file" || echo "❌ .c file"
[ -f config/pmw3610_automouse_timeout.dtsi ] && echo "✅ .dtsi file" || echo "❌ .dtsi file"

echo ""
echo "Building right side..."
west build -p -b nice_nano_v2 -- -DSHIELD=keyball44_right -DZMK_CONFIG="$(pwd)/config"

if [ $? -eq 0 ]; then
    echo "✅ Build successful!"
    echo ""
    echo "Flash size:"
    arm-none-eabi-size build/zephyr/zmk.elf
else
    echo "❌ Build failed!"
    exit 1
fi
```

---

**Status**: Ready for testing  
**Version**: 1.0  
**Last Updated**: 2025-10-19

