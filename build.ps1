$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
Push-Location -LiteralPath $root
try {
    New-Item -ItemType Directory -Force -Path 'build' | Out-Null
    $compiler = 'C:\msys64\ucrt64\bin\g++.exe'
    foreach ($name in @('task4_1', 'task4_2', 'task4_3', 'task4_4', 'task5_1', 'task5_2', 'task5_3', 'task5_4', 'task5_5')) {
        & $compiler -std=c++17 -O2 -Wall -Wextra -pedantic "src/$name.cpp" -o "build/$name.exe"
        if ($LASTEXITCODE -ne 0) { throw "Build failed: $name" }
        Write-Host "Built build/$name.exe"
    }
} finally {
    Pop-Location
}
