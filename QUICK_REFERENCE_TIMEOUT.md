# Auto Mouse Timeout - Quick Reference

## 🎯 Quick Access

**Location:** Layer 3 (Extended Layer) → Right side, Row 3

```
┌──────────────────────────────────────────────┐
│     Hold Layer 3 key, then press:            │
├──────────────────────────────────────────────┤
│  Position 7 (Caret ^)   → 1 second timeout  │
│  Position 8 (And &)     → 2 seconds timeout │
│  Position 9 (Asterisk *) → 5 seconds timeout │
└──────────────────────────────────────────────┘
```

## ⌨️ Key Positions on Layer 3

Right side, third row:
```
[Underscore] [Plus] [Trans] [Trans] [Backslash] [None]
    ↓          ↓       ↓
  [1s ⏱]    [2s ⏱]  [5s ⏱]
```

## 💡 Use Cases

| Timeout | Best For |
|---------|----------|
| **1s** 🏃 | Gaming, quick movements, want layer to deactivate fast |
| **2s** ⚖️ | General use (default) - balanced for most tasks |
| **5s** 🎨 | Design work, precision tasks, continuous trackball use |

## 🔧 Technical Info

- **Default on boot:** 2000ms (2 seconds)
- **Config location:** `keyball44_right.conf`
- **Changes:** Take effect immediately
- **Persistence:** Resets to default on power cycle

## 📝 How It Works

1. Move trackball → Mouse layer activates (Layer 4)
2. Stop moving → Timer starts
3. After timeout expires → Mouse layer deactivates
4. The timeout value is what these keys control

## 🔄 Changing the Timeout

```
Step 1: Access Layer 3
   └─→ Hold the key that activates Layer 3

Step 2: Press timeout key
   └─→ 1s / 2s / 5s (takes effect immediately)

Step 3: Test it
   └─→ Move trackball and watch layer indicator
```

## 📍 Layer Access Reminder

Check your keymap to see how to access Layer 3. Common methods:
- Hold `&lt 3 COLON` or `&lt 3 SQT` keys
- Press `&to 3` to toggle to Layer 3
- Use a layer tap or layer hold key

In the current keymap, Layer 3 is accessible from:
- **Layer 2 (L10 position):** `&lt 3 F10`
- **Layer 2 (L11 position):** `&lt 3 SQT`
- **Layer 2 (R11 position):** `&lt 3 SQT`

---

**Need more details?** See `AUTOMOUSE_TIMEOUT_FEATURE.md`

