# INI Modding Reference — Zero Hour

> This project is not affiliated with or endorsed by Electronic Arts.
> Command & Conquer is a trademark of Electronic Arts.
> You must own the original game to use this software.

**Task:** P4-03
**Date:** 2026-03-23
**Audience:** Modders who know the game but not C++

---

## Overview

Zero Hour's game data lives in `.ini` text files. Nearly every unit, building,
weapon, upgrade, and visual effect is defined here — no recompilation needed.
INI files use a simple block format:

```ini
TypeName BlockName
  Field1 = Value
  Field2 = Value
End
```

Multiple INI files are merged at startup. If two files define the same block
name with the same type, the later file's values overwrite the earlier one
(mode: `INI_LOAD_OVERWRITE`). This means a mod file that ships a `Weapon`
block with the same name as a base game weapon will replace it.

---

## INI Block Types

| Block Keyword | What it Defines |
|---------------|-----------------|
| `Object`      | Unit or building template |
| `Weapon`      | Weapon stats and projectile behaviour |
| `Armor`       | Damage multipliers (defence profile) |
| `CommandButton` | A single button in a command panel |
| `CommandSet`  | A 18-slot grid of CommandButtons |
| `SpecialPower` | Special ability (timing, requirements) |
| `Upgrade`     | Research upgrade (cost, time, effect) |
| `Locomotor`   | Movement physics profile |
| `FXList`      | Visual effect chain (particles, sounds) |
| `ObjectCreationList` | What spawns when an event fires |
| `ParticleSystem` | Particle effect definition |
| `GameData`    | Global engine settings (GlobalData INI) |
| `MappedImage` | Texture atlas entry for UI |
| `MultiplayerSettings` | Lobby/game mode settings |
| `PlayerTemplate` | Faction definition |

---

## Object — Unit/Building Template

**Source:** `GameEngine/Source/Common/Thing/ThingTemplate.cpp` (~254 fields)

An `Object` block defines a unit or building. It is the largest INI type.

### Required / Commonly Used Fields

| Field | Type | Example | Notes |
|-------|------|---------|-------|
| `DisplayName` | String key | `OBJECT:MyTank` | Key into `.csf` string file |
| `Side` | String | `America` | Faction (America, China, GLA, civilian, ...) |
| `Buildable` | Bool | `Yes` | Whether players can build this |
| `BuildCost` | Int | `800` | Cost in credits |
| `BuildTime` | Real | `15.0` | Build time in seconds |
| `RefundValue` | Int | `640` | Credits returned on sell/cancel (usually 80%) |
| `KindOf` | Flags | see below | Category flags — affects targeting, selection |
| `ArmorSet` | List | see below | Which armor profile to use in which conditions |
| `WeaponSet` | List | see below | Which weapon(s) to use and when |
| `VisionRange` | Real | `350.0` | How far this unit can see |
| `ShroudClearingRange` | Real | `350.0` | How much shroud it reveals |
| `Scale` | Real | `1.0` | World-space scale multiplier |
| `Geometry` | Enum | `CYLINDER`, `BOX` | Collision shape type |
| `GeometryMajorRadius` | Real | `13.0` | Collision footprint radius / half-width |
| `GeometryMinorRadius` | Real | `13.0` | Half-depth (BOX only) |
| `GeometryHeight` | Real | `18.0` | Height for collision and targeting |
| `CommandSet` | String | `TankCommandSet` | Name of CommandSet block to use |
| `Prerequisites` | Block | see below | What must be built first |
| `ExperienceValue` | Int list | `100 150 200` | XP awarded on kill (3 vet levels) |
| `SkillPointValue` | Int list | `100 150 200` | XP this unit earns when killing |
| `ExperienceRequired`| Int list | `0 200 500` | XP thresholds for vet/elite/heroic |
| `IsTrainable` | Bool | `Yes` | Whether this unit can gain veterancy |
| `TransportSlotCount`| Int | `2` | How many transport slots it occupies |
| `SelectPortrait` | String | `SUVehicle_Paladin` | Portrait image name |
| `ButtonImage` | String | `SUVehicle_Paladin` | Build-queue button image |

#### KindOf Flags (common subset)

Pipe-separate multiple flags: `KindOf = VEHICLE TANK CAN_ATTACK`

| Flag | Meaning |
|------|---------|
| `VEHICLE` | Is a vehicle |
| `TANK` | Is a tank (affects crush ability) |
| `INFANTRY` | Is an infantry unit |
| `STRUCTURE` | Is a building |
| `AIRCRAFT` | Flies |
| `HARVESTER` | Is a resource harvester |
| `CAN_ATTACK` | Has weapons and can engage |
| `GROUND_VEHICLE` | Ground-only movement |
| `TRACKED` | Moves on tracks (vs wheels) |
| `SCORE` | Counts in the score/stats |
| `HERO` | Is a hero unit |
| `COMMANDCENTER` | Is a command centre |
| `SUPPLY_SOURCE` | Is a supply depot |
| `BRIDGE` | Is a bridge |

#### ArmorSet Block

```ini
ArmorSet
  Conditions = None
  Armor      = TankArmor
  DamageFX   = TankDamageFX
End
```

`Conditions` can be `None` (always), `WEAPONSET_VETERAN`, `WEAPONSET_ELITE`, or `WEAPONSET_HERO`.

#### WeaponSet Block

```ini
WeaponSet
  Conditions = None
  Weapon     = PRIMARY TankCannon
  AutoChooseSources = DEFAULT
End
```

`Weapon` slots: `PRIMARY`, `SECONDARY`, `TERTIARY`

#### Prerequisites Block

```ini
Prerequisites
  Object       AMERICASUPPLYAIRFIELD_PLACEHOLDER
  Object       AMERICABATTLELAB_PLACEHOLDER
End
```

### Module Fields

Zero Hour's object system is modular. The key module fields are:

| Field | Example | Purpose |
|-------|---------|---------|
| `Behavior` | `W3DDefaultDraw W3DDefaultDrawModuleData` | Visual draw module |
| `Draw` | `W3DTankDraw TankDrawData` | Specialised draw module |
| `Locomotor` | `SET_NORMAL TankLocomotor` | Movement style |
| `Body` | `ActiveBody TankBodyData` | HP/armour processing |
| `ClientUpdate` | `BeaconClientUpdate` | Client-side effects |

Module blocks follow the field name and are closed with `End`:

```ini
Body = ActiveBody ModuleTag_Body
  MaxHealth = 600.0
  InitialHealth = 600.0
End
```

### Minimal Working Object Example

```ini
Object MyCustomTank
  ; Identity
  DisplayName      = OBJECT:MyCustomTank
  Side             = America
  Buildable        = Yes
  BuildCost        = 900
  BuildTime        = 18.0
  RefundValue      = 720
  KindOf           = VEHICLE TANK GROUND_VEHICLE TRACKED CAN_ATTACK SCORE

  ; Build prerequisites
  Prerequisites
    Object         AMERICAWARFACTORY_PLACEHOLDER
  End

  ; Combat
  ArmorSet
    Conditions     = None
    Armor          = TankArmor
    DamageFX       = TankDamageFX
  End
  WeaponSet
    Conditions     = None
    Weapon         = PRIMARY MyCustomCannon
  End

  VisionRange            = 350.0
  ShroudClearingRange    = 350.0

  ; Visuals
  Scale            = 1.0
  Geometry         = CYLINDER
  GeometryMajorRadius  = 13.0
  GeometryMinorRadius  = 13.0
  GeometryHeight       = 18.0

  SelectPortrait   = SUVehicle_Crusader
  ButtonImage      = SUVehicle_Crusader
  CommandSet       = AmericaTankCommandSet

  ; Stats
  ExperienceValue  = 100 150 200
  SkillPointValue  = 100 150 200
  ExperienceRequired = 0 200 500
  IsTrainable      = Yes

  ; Modules — reference existing ones to avoid redefining draw/body
  Body = ActiveBody ModuleTag_Body
    MaxHealth      = 600.0
    InitialHealth  = 600.0
  End

  ; Audio
  VoiceSelect      = TankVoiceSelect
  VoiceMove        = TankVoiceMove
  VoiceAttack      = TankVoiceAttack
End
```

---

## Weapon — Weapon Template

**Source:** `GameEngine/Source/GameLogic/Object/Weapon.cpp` (~80+ fields)

| Field | Type | Example | Notes |
|-------|------|---------|-------|
| `PrimaryDamage` | Real | `60.0` | Damage per hit at centre |
| `PrimaryDamageRadius` | Real | `0.0` | Splash radius (0 = no splash) |
| `SecondaryDamage` | Real | `10.0` | Splash damage at edge |
| `SecondaryDamageRadius` | Real | `15.0` | Outer radius of splash |
| `AttackRange` | Real | `200.0` | Maximum attack range |
| `MinimumAttackRange` | Real | `0.0` | Minimum range (blind spot) |
| `DamageType` | Enum | `EXPLOSION` | Type of damage dealt |
| `DeathType` | Enum | `NORMAL` | How killed units die |
| `WeaponSpeed` | Real | `120.0` | Projectile speed (0 = hitscan) |
| `ProjectileObject` | String | `AmericaTankShell` | Projectile Object name |
| `FireOCL` | String | `TankFireOCL` | ObjectCreationList on fire |
| `ProjectileDetonationFX` | String | `TankHitFX` | FX on impact |
| `ClipSize` | Int | `1` | Shots before reload |
| `ClipReloadTime` | Duration | `1500` | Reload time in ms |
| `DelayBetweenShots` | Duration | `500` | Delay between shots in clip |
| `FireSound` | AudioEvent | `TankFireSound` | Sound on fire |
| `FireFX` | String | `TankMuzzleFlash` | FXList on fire |
| `ScatterRadius` | Real | `5.0` | Accuracy spread |
| `AntiAirborneVehicle` | Bool | `No` | Can target aircraft |
| `AntiGround` | Bool | `Yes` | Can target ground |

**DamageType values:** `EXPLOSION`, `BULLET`, `FLAME`, `LASER`, `POISON`, `RADIATION`,
`EMP`, `CRUSH`, `SNIPER`, `BIOTERROR`, `STEALTHJET`, `TOPPLING`, `SUBDUED`

### Minimal Weapon Example

```ini
Weapon MyCustomCannon
  PrimaryDamage              = 60.0
  PrimaryDamageRadius        = 0.0
  AttackRange                = 250.0
  DamageType                 = EXPLOSION
  DeathType                  = NORMAL
  WeaponSpeed                = 100.0
  ProjectileObject           = AmericaTankShell
  ClipSize                   = 1
  ClipReloadTime             = 1500
  DelayBetweenShots          = 500
  FireSound                  = TankFireSound
  AntiGround                 = Yes
  AntiAirborneVehicle        = No
End
```

---

## Armor — Damage Multipliers

**Source:** `GameEngine/Source/GameLogic/Object/Armor.cpp`

Armor defines how much of each damage type gets through. Values < 1.0 reduce
damage; values > 1.0 amplify it; 0.0 = immune.

```ini
Armor TankArmor
  Armor = BULLET         0.5
  Armor = EXPLOSION      0.75
  Armor = FLAME          1.5
  Armor = EMP            1.0
  Armor = LASER          0.5
  Armor = POISON         0.0    ; tanks immune to poison
  Armor = CRUSH          0.0    ; immune to crush
End
```

---

## CommandButton — UI Button

**Source:** `GameEngine/Source/GameClient/GUI/ControlBar/ControlBar.cpp`

| Field | Type | Notes |
|-------|------|-------|
| `Command` | Enum | What the button does (see below) |
| `Object` | String | Object to build (for `BUILD_OBJECT`) |
| `Upgrade` | String | Upgrade to research |
| `SpecialPower` | String | Special power to activate |
| `TextLabel` | String key | Tooltip text |
| `ButtonImage` | String | Image asset name |
| `ButtonBorderType` | Enum | `NONE`, `BUILD`, `UPGRADE`, `ACTION`, `SYSTEM` |
| `RadiusCursorType` | Enum | Targeting cursor style |
| `Science` | String | Required science/upgrade |

**Command enum values:** `BUILD_OBJECT`, `SELL`, `CANCEL_BUILD`, `STOP`,
`ATTACK_MOVE`, `GUARD`, `GUARD_WITHOUT_PURSUIT`, `EVACUATE`, `EXECUTE_RAILED_TRANSPORT`,
`CONVERT_TO_CARBOMB`, `SPECIAL_POWER`, `SPECIAL_POWER_FROM_SHORTCUT`,
`PURCHASE_SCIENCE`, `HACK_INTERNET`, `TOGGLE_OVERCHARGE`, `PLACE_BEACON`

```ini
CommandButton Command_BuildMyTank
  Command          = BUILD_OBJECT
  Object           = MyCustomTank
  TextLabel        = CONTROLBAR:ToolTipBuildCrusader
  ButtonImage      = SUVehicle_Crusader
  ButtonBorderType = BUILD
End
```

---

## CommandSet — Button Grid

A grid of up to 18 slots (3 rows × 6 columns). Slot numbers start at 1.

```ini
CommandSet MyTankCommandSet
  1  = Command_AttackMove
  2  = Command_Guard
  3  = Command_Stop
  13 = Command_SetRallyPoint
  14 = Command_Sell
End
```

---

## SpecialPower — Special Ability

**Source:** `GameEngine/Source/Common/RTS/SpecialPower.cpp`

| Field | Type | Notes |
|-------|------|-------|
| `ReloadTime` | Duration (ms) | Cooldown between uses |
| `RequiredScience` | String | Science needed to enable |
| `PublicTimer` | Bool | Show countdown to enemies |
| `ShortcutPower` | Bool | Show in shortcut bar |
| `Enum` | Enum | Ability type identifier (engine-side) |
| `RadiusCursorRadius` | Real | Targeting cursor radius |
| `ViewObjectDuration` | Duration | How long scout effect lasts |

---

## Upgrade — Research Upgrade

**Source:** `GameEngine/Source/Common/System/Upgrade.cpp`

| Field | Type | Notes |
|-------|------|-------|
| `DisplayName` | String key | Tooltip name |
| `Type` | Enum | `PLAYER` or `OBJECT` |
| `BuildTime` | Real | Research time in seconds |
| `BuildCost` | Int | Cost in credits |
| `ButtonImage` | String | Image for research button |
| `ResearchSound` | AudioEvent | Sound when completed |
| `AcademyClassify` | Enum | Category for General's skill tree |

---

## Common Patterns

### Referencing an existing unit to inherit settings

Zero Hour has no `extends` keyword — create a new `Object` block with your own
name and copy the fields you need. The INI parser does not support inheritance.

### Overriding a base game unit

Simply define an `Object` block with the **exact same name** as an existing
object in your mod INI file. Your values will overwrite the originals at load
time. Only the fields you write are replaced; fields you omit keep their base
game value **only if** you are doing a CREATE_OVERRIDES load. For OVERWRITE
loads, unspecified fields reset to parser defaults — always include all required
fields when overriding.

### Unknown or unclear fields

Fields marked below with `[BEHAVIOR UNCLEAR — needs playtesting]` exist in the
parse table but their exact in-game effect was not verified during this audit.

| Field | Object Type | Note |
|-------|------------|------|
| `BuildCompletion` | Object | `[BEHAVIOR UNCLEAR — needs playtesting]` |
| `AutoChooseSources` | Object/WeaponSet | `[BEHAVIOR UNCLEAR — needs playtesting]` |
| `SelectPortrait` vs `ButtonImage` | Object | Portrait = in-game info panel; Button = build queue |
| `AcademyClassify` | Upgrade | Categorises into General's Points skill tree |

---

## Loading Order

At startup, INI files are loaded in this order (approximately):

1. `Data/INI/Default/` — base game default values
2. `Data/INI/` — base game override values
3. Mod files (loaded by the ModManager, P4-05, in priority order)

A mod file that ships any INI block with the same name as an existing block
will replace that block's fields.

---

## Further Reading

- `docs/ARCHITECTURE.md` — how INI data flows through the engine
- `GeneralsMD/Code/GameEngine/Source/Common/Thing/ThingTemplate.cpp` — full Object field list
- `GeneralsMD/Code/GameEngine/Source/GameLogic/Object/Weapon.cpp` — full Weapon field list
- `GeneralsMD/Code/GameEngine/Source/GameLogic/Object/Armor.cpp` — full Armor field list
