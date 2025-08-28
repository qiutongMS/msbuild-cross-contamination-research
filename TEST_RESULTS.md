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

**Result**: **This appears to work, but is extremely fragile and impractical!** While each target achieves isolation by using different property mechanisms, this approach has critical limitations:
- **Limited scalability**: Only works with exactly 2 targets (PreprocessorDefinitions + AdditionalOptions)
- **No third option**: There's no third property mechanism available for additional targets
- **Real-world impracticality**: Most projects need more than 2 custom compilation targets
- **Breaks immediately**: Adding any third target forces reuse of a property, causing contamination

---

---

## Key Observations

1. **Cross-contamination occurs when multiple targets use the same property inheritance** - both PreprocessorDefinitions and AdditionalOptions create separate accumulation chains
2. **MSB8027 warnings appear when multiple targets use the same inheritance mechanism** indicating duplicate file compilation
3. **Mixed approach is a dead-end solution** - only works with exactly 2 targets, fails with 3 or more
4. **Property scarcity is the real limitation** - only PreprocessorDefinitions and AdditionalOptions are available for inheritance
5. **Order matters within the same property chain** - later targets inherit accumulated definitions from earlier targets using the same property
6. **No warning doesn't mean scalable solution** - clean build with 2 targets doesn't solve the fundamental N>2 problem

## Conclusion

The test results reveal the true nature of MSBuild's item metadata inheritance system:

**Corrected Understanding:**
- **Per-property accumulation chains**: Each property (PreprocessorDefinitions, AdditionalOptions) maintains its own inheritance chain
- **Property scarcity problem**: Only 2 properties are available for compiler definitions, limiting solution to 2 targets maximum
- **Multi-user property contamination**: When multiple targets use the same property with inheritance (`%(PropertyName)`), they contaminate each other
- **Mixed-property illusion**: While using different properties avoids contamination, it's not a scalable solution

**Practical Reality:**
- Mixed approach only works for projects with exactly 2 custom compilation targets
- Any real-world project with 3+ targets will force property reuse and trigger contamination
- The "solution" is actually a demonstration of the fundamental limitation
- Property inheritance in MSBuild is inherently unscalable for multiple targets
