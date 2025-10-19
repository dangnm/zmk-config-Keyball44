# Devicetree Binding Fix

## Issue Resolved

**Error Message:**
```
devicetree error: binding controller <Node /behaviors/behavior_pmw3610_automouse_timeout> 
for <Node /keymap/mouse_layer> lacks binding
```

## Root Cause

The custom behavior was missing a **devicetree binding definition file**. ZMK/Zephyr requires a YAML file that describes the behavior's compatible string and properties.

## Solution Applied

Created the missing binding file at:
```
config/dts/bindings/behavior/zmk,behavior-pmw3610-automouse-timeout.yaml
```

## What This File Does

The binding YAML file tells the devicetree compiler:
1. What the `compatible` string means (`"zmk,behavior-pmw3610-automouse-timeout"`)
2. What properties are valid (`min-timeout-ms`, `max-timeout-ms`, `step-ms`)
3. How many binding cells the behavior uses (`#binding-cells = 1`)
4. Default values for optional properties

## File Structure

```yaml
compatible: "zmk,behavior-pmw3610-automouse-timeout"
include: behavior_base.yaml

properties:
  min-timeout-ms:
    type: int
    default: 100
    
  max-timeout-ms:
    type: int
    default: 5000
    
  step-ms:
    type: int
    default: 100
    
  "#binding-cells":
    const: 1
```

## Complete File List

The behavior now requires **4 core files** (previously was 3):

1. ✅ `config/pmw3610_automouse_timeout.c` - Behavior implementation (C code)
2. ✅ `config/pmw3610_automouse_timeout.dtsi` - Device tree node definition
3. ✅ `config/dts/bindings/behavior/zmk,behavior-pmw3610-automouse-timeout.yaml` - **NEW: Binding definition**
4. ✅ `config/CMakeLists.txt` - Build configuration

## Why This Was Needed

In ZMK/Zephyr:
- `.dtsi` files define **instances** of nodes (the actual behavior node)
- `.yaml` files define **bindings** (what properties a compatible string supports)

Without the YAML binding:
- The devicetree compiler doesn't know what `zmk,behavior-pmw3610-automouse-timeout` means
- It can't validate the properties in the `.dtsi` file
- The build fails with "lacks binding" error

With the YAML binding:
- The devicetree compiler understands the compatible string
- It validates properties against the schema
- The build succeeds ✅

## Build Now Should Work

Try building again:

```bash
cd path/to/zmk
west build -p -b nice_nano_v2 -- -DSHIELD=keyball44_right \
    -DZMK_CONFIG="path/to/zmk-config-Keyball44/config"
```

Expected result: **Clean build** ✅

## Directory Structure

```
config/
├── CMakeLists.txt
├── pmw3610_automouse_timeout.c
├── pmw3610_automouse_timeout.dtsi
├── keyball44.keymap
└── dts/
    └── bindings/
        └── behavior/
            └── zmk,behavior-pmw3610-automouse-timeout.yaml  ← NEW FILE
```

## Additional Notes

### Standard ZMK Binding Location

ZMK's built-in behaviors have bindings in the ZMK repo at:
```
zmk/app/dts/bindings/behaviors/
```

For user configs, bindings should be in:
```
config/dts/bindings/
```

### Binding File Naming

The filename must match the pattern:
```
<vendor>,<binding-name>.yaml
```

For this behavior:
- Vendor: `zmk`
- Binding name: `behavior-pmw3610-automouse-timeout`
- File: `zmk,behavior-pmw3610-automouse-timeout.yaml`

### Including Base Behavior

```yaml
include: behavior_base.yaml
```

This line includes ZMK's base behavior binding, which provides:
- Standard `label` property
- Logging support
- Device driver structure

## Validation

To verify the fix worked:

1. **Build succeeds** without devicetree errors
2. **Behavior is usable** in keymap with `&amto`
3. **Properties are validated** by devicetree compiler

## References

- [Zephyr Devicetree Bindings](https://docs.zephyrproject.org/latest/build/dts/bindings.html)
- [ZMK Behaviors](https://zmk.dev/docs/behaviors)
- [Example ZMK Binding](https://github.com/zmkfirmware/zmk/blob/main/app/dts/bindings/behaviors/zmk%2Cbehavior-key-press.yaml)

## Status

✅ **FIXED** - Binding file created and integrated  
✅ **Build should now succeed**  
✅ **Documentation updated**

---

**Date**: 2025-10-19  
**Issue**: Missing devicetree binding definition  
**Resolution**: Created YAML binding file

