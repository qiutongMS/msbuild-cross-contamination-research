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

# OriginalDemo

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

