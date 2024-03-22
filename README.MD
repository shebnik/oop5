# Organization of parallel information processing for multicore processors on the example of using the PThread library to calculate the product of two matrices

# Configuration
1. Configure vcpkg in VS Code - https://learn.microsoft.com/en-us/vcpkg/get_started/get-started-vscode?pivots=shell-powershell
2. Set `VCPKG_ROOT` PATH in CMakeUserPresets
3. Install `pthread`

    ```bash
    cd .\vcpkg\
    .\vcpkg install pthread:x64-windows
    ```