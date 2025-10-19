# Auto Mouse Timeout Key Positions

## Visual Layout - Layer 3 (Extended Layer)

### Full Layer 3 Layout

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                         Layer 3 - Extended Layer                            │
└─────────────────────────────────────────────────────────────────────────────┘

Left Side:                          Right Side:
┌────┬────┬────┬────┬────┬────┐    ┌────┬────┬────┬────┬────┬────┐
│TO 0│  ! │  @ │  # │  $ │BTCL│    │  ^ │  & │  * │  ( │  ) │    │
├────┼────┼────┼────┼────┼────┤    ├────┼────┼────┼────┼────┼────┤
│TO 1│BTD0│BTD1│BTD2│BTS2│    │    │  - │  = │  [ │  ] │ ;/2│ '/3│
├────┼────┼────┼────┼────┼────┤    ├────┼────┼────┼────┼────┼────┤
│TO 2│BTS0│BTS1│BTS2│BTS3│BTS4│    │ 1s │ 2s │ 5s │    │  \ │    │
└────┴────┴────┼────┼────┼────┤    ├────┼────┼────┴────┴────┴────┘
               │TO 4│    │ DEL│    │    │    │
               └────┴────┴────┘    └────┴────┘
```

### Timeout Controls Highlighted

```
Right Side - Row 3 (Bottom Row):
┌──────────────────────────────────────────────────────┐
│  ┏━━━━┓  ┏━━━━┓  ┏━━━━┓                             │
│  ┃ 1s ┃  ┃ 2s ┃  ┃ 5s ┃  (TRANS)  (  \  )  (NONE)  │
│  ┗━━━━┛  ┗━━━━┛  ┗━━━━┛                             │
│     ▲       ▲       ▲                                │
│     │       │       │                                │
│  1000ms  2000ms  5000ms                              │
└──────────────────────────────────────────────────────┘
```

## Key Position Details

| Position | Physical Location | Behavior | Timeout | Notes |
|----------|-------------------|----------|---------|-------|
| R3-P1 | Right side, row 3, position 1 | `&pmw_timeout_1000` | 1000ms | Quick timeout |
| R3-P2 | Right side, row 3, position 2 | `&pmw_timeout_2000` | 2000ms | Default timeout |
| R3-P3 | Right side, row 3, position 3 | `&pmw_timeout_5000` | 5000ms | Long timeout |

### Matrix Coordinates

In the keymap matrix (Layer 3), these keys are at:
```
Row 3 (4th row), positions 7-9 from left in the bindings array:

&to 2  &bt BT_SEL 0  &bt BT_SEL 1  &bt BT_SEL 2  &bt BT_SEL 3  &bt BT_SEL 4    
       &pmw_timeout_1000  &pmw_timeout_2000  &pmw_timeout_5000  &trans  &kp NON_US_BACKSLASH  &none
       ↑ Position 7      ↑ Position 8        ↑ Position 9
```

## How to Access

### Method 1: From Layer 2 (Numbers/Shortcuts Layer)
1. Hold a key that has `&lt 3` (layer tap to 3)
   - Examples: Semicolon key (R11), Quote key, or F10 key
2. While holding, press one of the timeout keys on the right side

### Method 2: From Any Layer
1. Press a `&to 3` key to switch to Layer 3
2. Press the desired timeout key
3. Press `&to 0` to return to default layer

## Visual Memory Aid

```
Think of the three keys as:
┌─────┬─────┬─────┐
│ ⚡  │ ⚖️  │ 🕐  │  
│ 1s  │ 2s  │ 5s  │
│Fast │Norm │Slow │
└─────┴─────┴─────┘
```

- **⚡ Fast (1s):** Quick response, layer deactivates fast
- **⚖️ Normal (2s):** Balanced, the default setting
- **🕐 Slow (5s):** Patient, layer stays active longer

## In Context: When You're Using the Keyboard

### Scenario 1: Gaming (Need Fast Response)
```
You: "I need the mouse layer to go away quickly"
Action: Hold layer 3 key → Press leftmost timeout key (1s)
Result: Mouse layer now deactivates after 1 second
```

### Scenario 2: Design Work (Need Patience)
```
You: "I'm making fine adjustments, need more time"
Action: Hold layer 3 key → Press rightmost timeout key (5s)
Result: Mouse layer stays active for 5 seconds
```

### Scenario 3: Return to Normal
```
You: "Back to regular usage"
Action: Hold layer 3 key → Press middle timeout key (2s)
Result: Back to default 2 second timeout
```

## Physical Key Labels (Optional)

If you want to label your keys, consider:
```
Key 1: "1s" or "⚡" or "1"
Key 2: "2s" or "⚖" or "2"
Key 3: "5s" or "🕐" or "5"
```

## Testing Your Setup

1. **Find Layer 3**
   - Look for keys with `&lt 3` in the keymap
   - Try holding those keys
   - Check if other keys change behavior

2. **Locate the Timeout Keys**
   - On Layer 3, right side, bottom physical row
   - First three keys from the left on that side

3. **Test Each Timeout**
   - Press a timeout key
   - Move the trackball
   - Use a timer to measure how long the layer stays active
   - Should match the timeout you selected

## Quick Reference Card (Printable)

```
╔═══════════════════════════════════════════╗
║   Auto Mouse Timeout Control Keys        ║
╠═══════════════════════════════════════════╣
║  Location: Layer 3, Right Side, Row 3    ║
║                                           ║
║  ┌────┬────┬────┐                        ║
║  │ 1s │ 2s │ 5s │                        ║
║  └────┴────┴────┘                        ║
║                                           ║
║  1s = Quick (gaming, fast switching)     ║
║  2s = Normal (default, balanced)         ║
║  5s = Patient (design, continuous use)   ║
║                                           ║
║  Changes take effect immediately!        ║
╚═══════════════════════════════════════════╝
```

---

**Pro Tip:** Memorize the order: **Left to Right = Short to Long**

