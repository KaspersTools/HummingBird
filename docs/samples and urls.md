# ImGui Libraries and Tools

A curated list of ImGui-related libraries and tools. ImGui is a bloat-free graphical user interface library for C++. It's primarily used for making tools in professional software and game development.

## Libraries

### ImGuizmo
- **Description**: An extension of ImGui for manipulating objects in 3D.
- **GitHub**: [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)

### ImNode
- **Description**: A collection of node-based editors for ImGui.
- **GitHub Links**:
  - [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)
  - [ImGui Node Editor](https://github.com/thedmd/imgui-node-editor)
  - [ImNodes](https://github.com/Nelarius/imnodes)
  - [Defold ImNodes](https://github.com/paweljarosz/defold_imnodes/tree/master/imnodes)

### ImPlot
- **Description**: An advanced 2D plotting library for ImGui.
- **GitHub**: [ImPlot](https://github.com/epezent/implot)

### ImGui Color Text Edit
- **Description**: A color text editor module for ImGui.
- **GitHub**: [ImGui Color Text Edit](https://github.com/BalazsJako/ImGuiColorTextEdit)

### Osfx
- **Description**: A collection of ImGui extensions for different applications.
- **GitHub Links**:
  - [OfxImGui](https://github.com/jvcleave/ofxImGui)

### ImGui Builder
- **Description**: A tool for building ImGui interfaces.
- **GitHub**: [ImGuiBuilder](https://github.com/Code-Building/ImGuiBuilder)

### Tracy
- **Description**: A real-time, nanosecond resolution, remote telemetry, and frame profiler.
- **GitHub**: [Tracy View Options](https://github.com/wolfpld/tracy/blob/master/server/TracyView_Options.cpp)

## Game Engines with ImGui Integration

### Piccolo
- **GitHub**: [Piccolo](https://github.com/BoomingTech/Piccolo)

### Overload
- **GitHub**: [Overload](https://github.com/adriengivry/Overload)

### Rbfx
- **GitHub**: [Rbfx](https://github.com/rbfx/rbfx?tab=readme-ov-file)

### Lumos
- **GitHub**: [Lumos](https://github.com/jmorton06/Lumos?tab=readme-ov-file)

### CXEngine
- **GitHub**: [CXEngine](https://github.com/oceancx/CXEngine/tree/master)

### TerraForge3D
- [TerraForge3D UI](https://github.com/Jaysmito101/TerraForge3D/blob/gen3/Screenshots/UI/QB.png)


## Miscellaneous ImGui Extensions

### Desktop Sharing
- **GitHub**: [Desktop Sharing](https://github.com/PHZ76/DesktopSharing)

### Memory
- **GitHub**: [ImGui Club](https://github.com/ocornut/imgui_club)

### Networking
- **GitHub**: [Terminal](https://github.com/NSTerminal/terminal/tree/dev)

## Ideas and Examples

### LuaPad
- **GitHub**: [LuaPad](https://github.com/xhighway999/luapad)

### Debugger
- **GitHub**: [Tug](https://github.com/kyle-sylvestre/Tug)

### TerraForge3D
- **GitHub**: [TerraForge3D](https://github.com/Jaysmito101/TerraForge3D/)

## SPDLOG Usage Examples

### Sample Code for SPDLOG
```cpp
try {
  // Create basic file logger
  auto my_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");

  // Create a rotating logger
  auto file_logger = spdlog::rotating_logger_mt("file_logger", "myfilename", 1024 * 1024 * 5, 3);

  my_logger->info("This is a basic logger.");

  for (int i = 0; i < 1024 * 1024 * 5 * 4; i++)
    file_logger->info("This is a rotating logger: #{}", i);
} catch (const spdlog::spdlog_ex& ex) {
  std::cout << "Log initialization failed: " << ex.what() << std::endl;
}
```

### JSON
- **GitHub**: [Spdlog JSON Config](https://github.com/MuhaoSZ/spdlog-json-config)

### SQLite3 Sample
- **GitHub**: [Spdlog Setup](https://github.com/guangie88/spdlog_setup)

### SPDLOG

Setup Guide
- **GitHub Links**:
    - [Spdlog](https://github.com/gabime/spdlog/tree/v1.x)
    - [Spdlog Setup](https://github.com/guangie88/spdlog_setup)

---

Note: This is a brief overview of the mentioned libraries and tools. For detailed information and usage instructions, please refer to their respective GitHub repositories.