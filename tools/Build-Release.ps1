param(
    [string]$Devenv = "${env:ProgramFiles(x86)}\Microsoft Visual Studio 9.0\Common7\IDE\devenv.com",
    [string]$OutputDirectory,
    [string]$BackgroundMusic
)

$ErrorActionPreference = 'Stop'
$repo = Split-Path $PSScriptRoot -Parent
if (-not $OutputDirectory) {
    $OutputDirectory = Join-Path $repo ('dist\build-' + (Get-Date -Format 'yyyyMMdd-HHmmss'))
}
$output = [IO.Path]::GetFullPath($OutputDirectory)
if (Test-Path -LiteralPath $output) {
    throw "Choose a new output directory: $output already exists."
}
foreach ($path in @($Devenv, "$env:XEDK\bin\win32\fxc.exe", "$env:XEDK\bin\win32\xuipkg.exe")) {
    if (-not (Test-Path -LiteralPath $path -PathType Leaf)) { throw "Missing build tool: $path" }
}
if ($BackgroundMusic -and -not (Test-Path -LiteralPath $BackgroundMusic -PathType Leaf)) {
    throw "Background music file not found: $BackgroundMusic"
}

New-Item -ItemType Directory -Path $output | Out-Null
$source = Join-Path $repo 'snes9x-1.51-src-d3d'
$project = Join-Path $source 'win32'
$package = Join-Path $output 'snes360-enhanced'
$media = Join-Path $package 'media'
$shaders = Join-Path $media 'Shaders'
New-Item -ItemType Directory -Path $shaders -Force | Out-Null
New-Item -ItemType Directory -Path (Join-Path $package 'Roms') | Out-Null
New-Item -ItemType Directory -Path (Join-Path $package 'previews') | Out-Null

Push-Location $repo
try {
    & $Devenv (Join-Path $project 'snes9xw.sln') /Rebuild 'Release|Xbox 360' /Out (Join-Path $output 'build.log')
    if ($LASTEXITCODE -ne 0) { throw 'The Xbox 360 release build failed. See build.log.' }
} finally { Pop-Location }

$xex = Join-Path $project 'Xbox 360\Release\Snes360.xex'
if (-not (Test-Path -LiteralPath $xex -PathType Leaf)) { throw "Missing build output: $xex" }
Copy-Item -LiteralPath $xex -Destination (Join-Path $package 'Snes360.xex')

& (Join-Path $PSScriptRoot 'Build-Skin.ps1') -OutputFile (Join-Path $media 'Snes360.xzp')
Push-Location $project
try {
    & "$env:XEDK\bin\win32\fxc.exe" '..\xbox\waveeffect.hlsl' /nologo /Tvs_3_0 /EShadeVertex /DEntryPoint_ShadeVertex "/Fo$shaders\ShadeXuiEffectVertex.xvu"
    if ($LASTEXITCODE -ne 0) { throw 'Vertex shader compilation failed.' }
    & "$env:XEDK\bin\win32\fxc.exe" '..\xbox\waveeffect.hlsl' /nologo /Tps_3_0 /EShadePixel /DEntryPoint_ShadePixel "/Fo$shaders\ShadeXuiEffectPixel.xpu"
    if ($LASTEXITCODE -ne 0) { throw 'Pixel shader compilation failed.' }
} finally { Pop-Location }

Copy-Item -LiteralPath (Join-Path $source 'xbox\XuiEffect_Texture.png') -Destination $media
Copy-Item -LiteralPath (Join-Path $source 'xbox\Skin\recovery.ttf') -Destination (Join-Path $media 'Snes360.ttf')
Copy-Item -LiteralPath (Join-Path $source 'xbox\Skin\saveicon.png') -Destination $media
Copy-Item -LiteralPath (Join-Path $source 'xbox\Skin\saveicon.png') -Destination $package
if ($BackgroundMusic) {
    $sounds = Join-Path $media 'sounds'
    New-Item -ItemType Directory -Path $sounds | Out-Null
    Copy-Item -LiteralPath $BackgroundMusic -Destination (Join-Path $sounds 'background.mp3')
}
Copy-Item -LiteralPath (Join-Path $repo 'README.md'), (Join-Path $repo 'LICENSE') -Destination $package
Copy-Item -LiteralPath (Join-Path $source 'doc\snes9x-license.txt') -Destination $package
Copy-Item -LiteralPath (Join-Path $source 'doc\GPL-2'), (Join-Path $source 'doc\LGPL-2.1') -Destination $package
Copy-Item -LiteralPath (Join-Path $repo 'docs\release-0.37.md') -Destination $package
Write-Output "Release files: $package"
Write-Output 'Inspect the XEX and package before creating a release archive.'
