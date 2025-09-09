# OriginalDemo

## Build Result

```
"C:\Users\qiutongshen\source\testBeforeTargets\OriginalDemo\testBeforeTargets.sln" (Rebuild target) (1) ->
"C:\Users\qiutongshen\source\testBeforeTargets\OriginalDemo\TestProject.vcxproj" (Rebuild target) (2) ->
(WarnCompileDuplicatedFilename target) ->
  C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Microsoft\VC\v170\Microsoft.CppBuild.targets(1129,5): warning MSB8027: Two or more files with the name of SecondModule.cpp will produce outputs to the same location. This can lead to an incorrect build result.  The files involved are SecondModule.cpp, SecondModule.cpp. [C:\Users\qiutongshen\source\testBeforeTargets\OriginalDemo\TestProject.vcxproj]

"C:\Users\qiutongshen\source\testBeforeTargets\OriginalDemo\testBeforeTargets.sln" (Rebuild target) (1) ->
"C:\Users\qiutongshen\source\testBeforeTargets\OriginalDemo\TestProject.vcxproj" (Rebuild target) (2) ->
(Link target) ->
  TestProject\x64\Release\SecondModule.obj : warning LNK4042: object specified more than once; extras ignored [C:\Users\qiutongshen\source\testBeforeTargets\OriginalDemo\TestProject.vcxproj]

    2 Warning(s)
    0 Error(s)
```

---

## Debug Result

### First Module Function Called

  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is defined (value: 1)
  - FIRST_TARGET is defined (value: 1)
  - MODULE_TWO is NOT defined
  - SECOND_TARGET is NOT defined
  - Compiled by: FirstTarget.targets

### Second Module Function Called

  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is defined (value: 1)
  - FIRST_TARGET is defined (value: 1)
  - MODULE_TWO is defined (value: 1)
  - SECOND_TARGET is defined (value: 1)
  - Compiled by: SecondTarget.targets


# Demo1(Qiutong's revert PR)

## Build Result

```
Build succeeded.
    0 Warning(s)
    0 Error(s)
```

---

## Debug Result

### First Module Function Called

  - GLOBAL_DEFINE is NOT defined
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is NOT defined
  - FIRST_TARGET is NOT defined
  - MODULE_TWO is NOT defined
  - SECOND_TARGET is NOT defined
  - Compiled by: FirstTarget.targets

### Second Module Function Called
  - GLOBAL_DEFINE is NOT defined
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is NOT defined
  - FIRST_TARGET is NOT defined
  - MODULE_TWO is NOT defined
  - SECOND_TARGET is NOT defined
  - Compiled by: SecondTarget.targets

## Even though the build has no warning, we lost the global define value!!.

# Demo2(Qiutong's sencond PR)

## Build Result

```
Build succeeded.
    0 Warning(s)
    0 Error(s)
```

---

## Debug Result

### First Module Function Called

  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is defined (value: 1)
  - FIRST_TARGET is defined (value: 1)
  - MODULE_TWO is NOT defined
  - SECOND_TARGET is NOT defined
  - Compiled by: FirstTarget.targets

### Second Module Function Called

  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is NOT defined
  - FIRST_TARGET is NOT defined
  - MODULE_TWO is defined (value: 1)
  - SECOND_TARGET is defined (value: 1)
  - Compiled by: SecondTarget.targets

### This appears to work, but is extremely limited and impractical! While avoiding contamination by using different properties, this approach has fatal scalability issues:
### Maximum 2 targets: Only PreprocessorDefinitions and AdditionalOptions are available
### No solution for 3+ targets: Any additional target must reuse a property, causing contamination
### Not a real solution: Demonstrates the fundamental limitation rather than solving it

# Demo3(set all properties before BeforeClCompiles)

## Build Result

```
Build succeeded.
    0 Warning(s)
    0 Error(s)
```

---

## Debug Result

### First Module Function Called

  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is defined (value: 1)
  - FIRST_TARGET is defined (value: 1)
  - MODULE_TWO is defined (value: 1)
  - SECOND_TARGET is defined (value: 1)
  - Compiled by: SecondTarget.targets

### Second Module Function Called

  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is defined (value: 1)
  - FIRST_TARGET is defined (value: 1)
  - MODULE_TWO is defined (value: 1)
  - SECOND_TARGET is defined (value: 1)
  - Compiled by: SecondTarget.targets

### Not sure if I tried correctly, and this may trigger warning C4651 and be treated as an error in WinUI


# Demo4(set all properties before ClCompiles, after $(BeforeClCompileTargets);$(ComputeCompileInputsTargets);MakeDirsForCl)

## Build Result

```
Build succeeded.
    0 Warning(s)
    0 Error(s)
```

---

## Debug Result

### First Module Function Called

  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is NOT defined
  - FIRST_TARGET is NOT defined
  - MODULE_TWO is NOT defined
  - SECOND_TARGET is NOT defined
  - Compiled by: FirstTarget.targets

### Second Module Function Called

  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is NOT defined
  - FIRST_TARGET is NOT defined
  - MODULE_TWO is NOT defined
  - SECOND_TARGET is NOT defined
  - Compiled by: SecondTarget.targets

# Demo5(set all properties before ClCompiles, after MakeDirsForCl)

## Build Result

```
Build succeeded.
    0 Warning(s)
    0 Error(s)
```

---

## Debug Result

### First Module Function Called

  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is defined (value: 1)
  - FIRST_TARGET is defined (value: 1)
  - MODULE_TWO is defined (value: 1)
  - SECOND_TARGET is defined (value: 1)
  - EXTRA_FIRST is defined (value: 1)
  - Compiled by: FirstTarget.targets

### Second Module Function Called

  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is defined (value: 1)
  - FIRST_TARGET is defined (value: 1)
  - MODULE_TWO is defined (value: 1)
  - SECOND_TARGET is defined (value: 1)
  - Compiled by: SecondTarget.targets

### Not sure if I tried correctly, and this may trigger warning C4651 and be treated as an error in WinUI