# Automouse Timeout - Quick Reference Card

## Basic Syntax

```c
&amto <parameter>
```

## Parameters

| Code | Action | Example |
|------|--------|---------|
| Positive number | Set absolute timeout (ms) | `&amto 1000` = 1 second |
| `-1` | Decrease by step | `&amto (-1)` = -100ms |
| `-2` | Increase by step | `&amto (-2)` = +100ms |
| `-3` | Reset to default | `&amto (-3)` = use Kconfig value |

## Common Presets

```c
&amto 200   // Ultra-responsive (gaming)
&amto 400   // Precision work
&amto 600   // Default balanced
&amto 1000  // Comfortable browsing
&amto 1500  // Relaxed use
&amto 2000  // Extended activity
```

## Configuration

Edit `pmw3610_automouse_timeout.dtsi`:

```c
amto: behavior_pmw3610_automouse_timeout {
    min-timeout-ms = <100>;    // Minimum: 100ms
    max-timeout-ms = <5000>;   // Maximum: 5 seconds  
    step-ms = <100>;           // Step: 100ms
};
```

## Example Layer

```c
mouse_layer {
    bindings = <
        // First key sets to 1 second
        &amto 1000  
        
        // Second key increases by 100ms
        &amto (-2)
        
        // Third key decreases by 100ms
        &amto (-1)
        
        // Other keys...
        ...
    >;
};
```

## Tips

- **Lower timeout** = Mouse layer deactivates faster = More responsive
- **Higher timeout** = Mouse layer stays active longer = More comfortable
- Changes take effect immediately, no reboot needed
- Watch serial console for confirmation messages
- Values automatically clamped to min/max range

## Troubleshooting

| Issue | Solution |
|-------|----------|
| Not working | Check `automouse-layer` is set in overlay |
| Can't go lower | Check `min-timeout-ms` setting |
| Can't go higher | Check `max-timeout-ms` setting |
| Build fails | Verify CMakeLists.txt is in config/ folder |

## Files

- **Implementation**: `config/pmw3610_automouse_timeout.c`
- **Device Tree**: `config/pmw3610_automouse_timeout.dtsi`
- **Build Config**: `config/CMakeLists.txt`
- **Documentation**: `config/AUTOMOUSE_TIMEOUT_BEHAVIOR.md`
- **Examples**: `config/automouse_timeout_examples.keymap`

---

**Remember**: The timeout value must be greater than 0!

