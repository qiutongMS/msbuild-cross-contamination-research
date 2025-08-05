# Test Results Documentation

This document records the actual build outputs and behaviors observed when testing different MSBuild preprocessor definition configurations.

## Test Environment

- **MSBuild Version**: Microsoft Visual Studio 2022 MSBuild
- **Platform**: Windows 10/11
- **Configuration**: Debug|Win32
- **Project**: TestProject.vcxproj with hierarchical target imports

## Test Case 1: All Targets Using PreprocessorDefinitions

### Configuration
```xml
<!-- FirstTarget.targets -->
<ClCompile Include="FirstModule.cpp">
  <PreprocessorDefinitions>MODULE_ONE=1;FIRST_TARGET=1;%(PreprocessorDefinitions)</PreprocessorDefinitions>
</ClCompile>

<!-- SecondTarget.targets -->
<ClCompile Include="SecondModule.cpp">
  <PreprocessorDefinitions>MODULE_TWO=1;SECOND_TARGET=1;%(PreprocessorDefinitions)</PreprocessorDefinitions>
</ClCompile>

<!-- ThirdTarget.targets -->
<ClCompile Include="ThirdModule.cpp">
  <!-- No specific preprocessor definitions - acts as a "detector" to show global state -->
</ClCompile>
```

### Build Output
```
warning MSB8027: Two or more files with the name of SecondModule.cpp will produce outputs to the same location
```

### Compilation Commands (from .tlog files)
```bash
# FirstModule.cpp (compiled once)
/D MODULE_ONE=1 /D FIRST_TARGET=1 /D GLOBAL_DEFINE=1 /D _UNICODE /D UNICODE

# SecondModule.cpp (compiled multiple times)
# First compilation:
/D MODULE_TWO=1 /D SECOND_TARGET=1 /D GLOBAL_DEFINE=1 /D _UNICODE /D UNICODE
# Second compilation:
/D MODULE_TWO=1 /D SECOND_TARGET=1 /D MODULE_ONE=1 /D FIRST_TARGET=1 /D GLOBAL_DEFINE=1 /D _UNICODE /D UNICODE

# ThirdModule.cpp (compiled once)
/D GLOBAL_DEFINE=1 /D _UNICODE /D UNICODE
```

### Runtime Output
```
First Module Function Called:
  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is defined (value: 1)
  - FIRST_TARGET is defined (value: 1)
  - MODULE_TWO is NOT defined
  - SECOND_TARGET is NOT defined
  - MODULE_THREE is NOT defined
  - THIRD_TARGET is NOT defined
  - Compiled by: FirstTarget.targets

Second Module Function Called:
  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is defined (value: 1)  ← Unexpected!
  - FIRST_TARGET is defined (value: 1)  ← Unexpected!
  - MODULE_TWO is defined (value: 1)
  - SECOND_TARGET is defined (value: 1)
  - MODULE_THREE is NOT defined
  - THIRD_TARGET is NOT defined
  - Compiled by: SecondTarget.targets

Third Module Function Called:
  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is NOT defined
  - FIRST_TARGET is NOT defined
  - MODULE_TWO is NOT defined
  - SECOND_TARGET is NOT defined
  - MODULE_THREE is NOT defined
  - THIRD_TARGET is NOT defined
  - Compiled by: ThirdTarget.targets
```



**Result**: Complete cross-contamination. 

---

## Test Case 2: All Targets Using AdditionalOptions

### Configuration
```xml
<!-- FirstTarget.targets -->
<ClCompile Include="FirstModule.cpp">
  <AdditionalOptions>/D MODULE_ONE=1 /D FIRST_TARGET=1 %(AdditionalOptions)</AdditionalOptions>
</ClCompile>

<!-- SecondTarget.targets -->
<ClCompile Include="SecondModule.cpp">
  <AdditionalOptions>/D MODULE_TWO=1 /D SECOND_TARGET=1 %(AdditionalOptions)</AdditionalOptions>
</ClCompile>

<!-- ThirdTarget.targets -->
<ClCompile Include="ThirdModule.cpp">
  <!-- No specific preprocessor definitions - acts as a "detector" to show global state -->
</ClCompile>
```

### Build Output
```
warning MSB8027: Two or more files with the name of SecondModule.cpp will produce outputs to the same location
```

### Runtime Output
**Result**: Same cross-contamination as Test Case 1. The global state sharing problem persists with AdditionalOptions.

---

## Test Case 3: Mixed Approach (Fragile Success)

### Configuration
```xml
<!-- FirstTarget.targets -->
<ClCompile Include="FirstModule.cpp">
  <AdditionalOptions>/D MODULE_ONE=1 /D FIRST_TARGET=1 %(AdditionalOptions)</AdditionalOptions>
</ClCompile>

<!-- SecondTarget.targets -->
<ClCompile Include="SecondModule.cpp">
  <PreprocessorDefinitions>MODULE_TWO=1;SECOND_TARGET=1;%(PreprocessorDefinitions)</PreprocessorDefinitions>
</ClCompile>

<!-- ThirdTarget.targets (disabled) -->
<!-- <ClCompile Include="ThirdModule.cpp"> ... </ClCompile> -->
```

### Build Output
```
Build succeeded.
0 Warning(s)
0 Error(s)
```

### Runtime Output
```
First Module Function Called:
  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is defined (value: 1)
  - FIRST_TARGET is defined (value: 1)
  - MODULE_TWO is NOT defined
  - SECOND_TARGET is NOT defined
  - MODULE_THREE is NOT defined
  - THIRD_TARGET is NOT defined
  - Compiled by: FirstTarget.targets
Second Module Function Called:
  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is NOT defined
  - FIRST_TARGET is NOT defined
  - MODULE_TWO is defined (value: 1)
  - SECOND_TARGET is defined (value: 1)
  - MODULE_THREE is NOT defined
  - THIRD_TARGET is NOT defined
  - Compiled by: SecondTarget.targets
Third Module Function Called:
  - GLOBAL_DEFINE is defined (value: 1)
  - GLOBAL_ADDITIONAL is defined (value: 1)
  - MODULE_ONE is NOT defined
  - FIRST_TARGET is NOT defined
  - MODULE_TWO is NOT defined
  - SECOND_TARGET is NOT defined
  - MODULE_THREE is NOT defined
  - THIRD_TARGET is NOT defined
  - Compiled by: ThirdTarget.targets
```

**Result**: Appears to work! But this is the "fragile success" scenario - it only works because ThirdTarget does not append any value.

---

---

## Key Observations

1. **%(PreprocessorDefinitions) and %(AdditionalOptions) behave identically** - both cause global state contamination
2. **MSB8027 warnings consistently appear** when multiple targets use inheritance patterns
3. **"Fragile success" scenarios** only work in controlled conditions with limited participants
4. **True isolation requires giving up inheritance** - you can't have both
5. **Order matters** - later targets inherit accumulated definitions from earlier targets
6. **No warning doesn't mean no contamination** - runtime behavior reveals the truth

## Conclusion

The test results confirm that MSBuild's item metadata inheritance system is fundamentally incompatible with file-level isolation goals. Any solution that appears to work is likely fragile and will break when additional targets or third-party code joins the build.
