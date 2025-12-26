# DXC Shader Compiler

DXC is a command-line tool that compiles HLSL shader files using the DirectX Shader Compiler (DXC). It supports parallel compilation of multiple shaders and is designed to be called by AylaBuildTool during the build process.

## Features

- **Batch Compilation**: Compile multiple shaders from a single input file
- **Multithreaded**: Uses the Task system for parallel compilation
- **Dependency Tracking**: Generates dependency files for build system integration
- **Error Reporting**: Detailed compilation errors and warnings with file references
- **Directory Creation**: Automatically creates output and dependency directories

## Usage

```bash
DXC <compilation-list-file>
```

### Compilation List Format

The input file should contain one shader compilation task per line in the following format:

```
Source/Shader1.hlsl -o Intermediate/Shader1.cso -d Intermediate/Shader1.def
Source/Shader2.hlsl -o Intermediate/Shader2.cso -d Intermediate/Shader2.def
```

Each line specifies:
- **Source file**: Path to the HLSL shader source file
- **-o <output>**: Path where the compiled shader bytecode (.cso) will be written
- **-d <deps>**: Path where the dependency file (.def) will be written

### Example

Create a file `shaders.txt`:
```
Shaders/VertexShader.hlsl -o Output/VS.cso -d Output/VS.def
Shaders/PixelShader.hlsl -o Output/PS.cso -d Output/PS.def
Shaders/ComputeShader.hlsl -o Output/CS.cso -d Output/CS.def
```

Run the compiler:
```bash
DXC shaders.txt
```

## Requirements

### Windows
- DirectX Shader Compiler (DXC) SDK
- `dxcompiler.dll` must be available in the system PATH or in the same directory as the DXC executable

### Linux/macOS
- Not directly supported (DXC is a DirectX tool)
- Consider using alternative shader compilers or cross-compilation tools

## Build Configuration

The DXC module is configured in `DXC.Module.cs`:
- Module Type: Console application
- Dependencies: Core module
- Additional Libraries: dxcompiler.lib (Windows)

## Implementation Details

### Compilation Flow

1. **Parse Input**: Read and parse the compilation list file
2. **Initialize DXC**: Create IDxcCompiler3 and IDxcUtils instances
3. **Parallel Compilation**: Use Task<>::Run to compile shaders concurrently
4. **Result Aggregation**: Wait for all tasks and report success/failure counts
5. **Error Handling**: Log errors and return appropriate exit code

### Shader Compilation Options

Currently configured with:
- Entry point: `main`
- Target profile: `ps_6_0` (Shader Model 6.0, Pixel Shader)
- Optimization: `-O3` (maximum optimization)
- Debug info: `-Zi` (for better error messages)

**Note**: In a production implementation, shader types (vs, ps, cs, etc.) and entry points should be configurable per-shader or auto-detected.

### Multithreading

The compiler uses the Ayla Task system (`Task<bool>::Run`) to execute shader compilations in parallel on the thread pool. This significantly reduces build times when compiling multiple shaders.

### Dependency Files

Dependency files (.def) contain a list of files that the shader depends on (including itself and any `#include` files). This allows the build system to determine when shaders need recompilation.

**Current Implementation**: Only writes the source file itself as a dependency. A full implementation would parse `#include` directives from the compiled shader's metadata.

## Error Codes

- **0**: All shaders compiled successfully
- **1**: One or more shaders failed to compile, or invalid arguments

## Future Enhancements

1. **Configurable Shader Types**: Parse shader type from filename or add to command-line
2. **Entry Point Configuration**: Allow specifying entry points per-shader
3. **Include Dependency Parsing**: Full include dependency tracking using DXC reflection
4. **SPIRV Support**: Add `-spirv` flag for Vulkan compatibility (when needed)
5. **Preprocessor Defines**: Support for `-D` defines
6. **Profile Detection**: Auto-detect shader model from source or configuration
7. **Incremental Compilation**: Skip shaders with up-to-date outputs
8. **Statistics**: Report compilation times and bytecode sizes

## Integration with AylaBuildTool

When AylaBuildTool builds shader modules, it should:
1. Generate a shader compilation list file
2. Invoke DXC with the list file path
3. Check the exit code for compilation success
4. Parse dependency files for build graph integration
