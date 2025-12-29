# HLSL Shader Build Pipeline

This document describes the HLSL shader build pipeline integration in the CPP.REF engine.

## Overview

The shader build pipeline automatically compiles HLSL shader files (`.hlsl`) during the build process, generating both DirectX (`.cso`) and Vulkan (`.spv`) shader bytecode files. The system supports dependency tracking to skip compilation when shaders and their includes haven't changed.

## Features

- **Automatic Shader Discovery**: All `.hlsl` files in module directories are automatically discovered and compiled
- **Dual Output**: Generates both DirectX Shader Object (`.cso`) and SPIR-V (`.spv`) files for cross-platform support
- **Dependency Tracking**: Uses `.deps` files to track shader includes and only recompile when necessary
- **IDE Integration**: HLSL files appear in Visual Studio projects with proper IntelliSense support
- **Shader Type Detection**: Automatically detects shader types from comments or HLSL attributes

## Shader Type Annotation

To enable shader compilation, you must annotate your HLSL files with their shader type. There are two supported methods:

### Method 1: Comment-Based Annotation (Recommended)

Add a comment at the top of your shader file specifying the shader type:

```hlsl
// Shader Type: library
#include "Common.hlsli"

[shader("raygeneration")]
void main()
{
    // Shader code...
}
```

Supported shader type keywords in comments:
- `vertex`, `vs` → Vertex Shader
- `pixel`, `ps`, `fragment` → Pixel/Fragment Shader
- `compute`, `cs` → Compute Shader
- `library`, `lib`, `raytracing`, `rt` → Shader Library (for raytracing)

### Method 2: HLSL Attribute-Based Detection

The build system also detects shader types from HLSL shader attributes:

```hlsl
[shader("raygeneration")]
void main()
{
    // Automatically detected as library shader
}
```

Detected attributes include:
- `[shader("vertex")]` → Vertex Shader
- `[shader("pixel")]` → Pixel Shader
- `[shader("compute")]` → Compute Shader
- `[shader("raygeneration")]`, `[shader("closesthit")]`, `[shader("miss")]`, etc. → Library Shader

## Build Process

### 1. Shader Discovery

During build, the AylaBuildTool scans all module source directories for `.hlsl` files.

### 2. Shader Type Detection

For each shader file, the system:
1. Reads the first 20 lines of the file
2. Looks for shader type annotations in comments or attributes
3. Skips files without valid shader type annotations

### 3. Dependency Checking

The system checks if compilation is needed by:
1. Verifying if output files (`.cso` and `.spv`) exist
2. Reading the `.deps` file to check all dependencies
3. Comparing timestamps of dependencies against output files
4. Skipping compilation if all dependencies are older than outputs

### 4. Makefile Generation

For shaders that need compilation, the system generates a temporary makefile at:
```
Engine/Intermediate/<ModuleName>/<Platform>/<Config>/ShaderCompilation.txt
```

Format:
```
<source.hlsl> -t <type> -e main -o <output_base_path> -I <include_path>...
<source.hlsl> -t <type> -e main --vulkan -o <output_base_path> -I <include_path>...
```

### 5. DXC Invocation

The DXC compiler is invoked with the generated makefile to compile all shaders in parallel.

### 6. Output Files

Compiled shaders are written to:
```
Engine/Binaries/<Platform>/<Config>/Shaders/<ShaderName>.cso
Engine/Binaries/<Platform>/<Config>/Shaders/<ShaderName>.spv
```

Dependency files are written to:
```
Engine/Intermediate/<ModuleName>/<Platform>/<Config>/<ShaderName>.deps
```

## Project File Integration

### Visual Studio

HLSL files are automatically added to Visual Studio projects with the `FXCompile` item type, enabling:
- Syntax highlighting
- IntelliSense (if supported by the IDE)
- Proper file organization in solution explorer

Example project entry:
```xml
<ItemGroup>
  <FXCompile Include="Path/To/Shader.hlsl" />
  <ClInclude Include="Path/To/Common.hlsli" />
</ItemGroup>
```

### Visual Studio Code

(To be implemented)

## Example Shader Project

Here's an example of a raytracing shader project structure:

```
DXC/Private/
├── Common.hlsli              # Shared include file
├── RayGeneration.hlsl        # Ray generation shader
├── Hit.hlsl                  # Closest hit shader
└── Miss.hlsl                 # Miss shader
```

Each `.hlsl` file includes a shader type annotation:

```hlsl
// Shader Type: library
#include "Common.hlsli"

[shader("raygeneration")]
void main()
{
    // Ray generation logic
}
```

## Compiler Arguments

The shader compiler automatically configures appropriate arguments:

### Common Arguments
- `-E main` - Entry point (default)
- `-O3` - Maximum optimization
- `-Zi` - Debug information
- `-enable-16bit-types` - Enable 16-bit types
- `-I <path>` - Include directories (source directory + module include paths)

### DirectX-Specific
- `-T <profile>` - Shader profile (e.g., `lib_6_3`, `vs_6_0`, `ps_6_0`, `cs_6_0`)

### Vulkan-Specific (with --vulkan flag)
- `-spirv` - Generate SPIR-V output
- `-fspv-target-env=vulkan1.2` - Target Vulkan 1.2
- `-fspv-extension=SPV_KHR_ray_tracing` - Enable raytracing extension (for library shaders)

## Implementation Details

### Source Code Types

Two new source code types were added to `SourceCodeType` enum:
- `HLSLShader` - Shader source files (`.hlsl`)
- `HLSLInclude` - Shader include files (`.hlsli`)

### Build Integration

The `ShaderCompileTask` class handles shader compilation and is integrated into the build pipeline:

1. **Discovery Phase**: Scans for HLSL files during project scanning
2. **Build Phase**: Executes in parallel with C++ compilation
3. **Output Phase**: Places compiled shaders in the output directory alongside executables

### Dependency Format

Dependency files use a makefile-like format:

```makefile
output.cso: \
  source.hlsl \
  include1.hlsli \
  include2.hlsli
```

This format is compatible with standard build systems and enables incremental compilation.

## Limitations and Future Improvements

### Current Limitations
1. Shader type must be explicitly annotated (no automatic detection from usage)
2. Entry point is fixed to `main` (no custom entry points)
3. Shader model is fixed to 6.0 for standard shaders, 6.3 for libraries
4. All shaders are compiled with both DirectX and Vulkan targets (no selective compilation)

### Planned Improvements
1. Support for custom entry points via command-line annotations
2. Configurable shader models
3. Selective platform compilation (DirectX-only or Vulkan-only builds)
4. Shader variant support (permutations with different defines)
5. Shader hot-reloading for faster iteration
6. Compilation statistics (timing, bytecode sizes)

## Troubleshooting

### Shaders Not Compiling

**Problem**: Shaders are skipped during build

**Solutions**:
1. Verify shader type annotation is present at the top of the file
2. Check that the annotation uses supported keywords
3. Ensure the shader file has `.hlsl` extension (not `.hlsli`)

### Missing Include Files

**Problem**: Shader compilation fails with "file not found" errors

**Solutions**:
1. Verify include paths in `Module.cs` file
2. Check that included files are in the module's `Private` or `Public` directories
3. Use relative paths in `#include` directives

### DXC Not Found

**Problem**: Build warns that DXC compiler is not found

**Solutions**:
1. Build the DXC project first: `dotnet AylaBuildTool.dll build --target DXC`
2. Ensure VulkanSDK is installed (required for DXC compilation)
3. Check that the DXC executable is in `Engine/Binaries/dotnet/`

### Dependency Tracking Issues

**Problem**: Shaders not recompiling after include changes

**Solutions**:
1. Delete the `.deps` file to force recompilation
2. Use `--clean Rebuild` flag to rebuild all shaders
3. Check file timestamps (ensure system clock is correct)

## Related Documentation

- [DXC Shader Compiler](../Engine/Source/Programs/DXC/README.md)
- [Build System](BuildSystem.md)
- [Module System](Architecture.md#module-system)
