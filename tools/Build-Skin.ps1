param([Parameter(Mandatory = $true)][string]$OutputFile)

$ErrorActionPreference = 'Stop'
$repo = Split-Path $PSScriptRoot -Parent
$output = [IO.Path]::GetFullPath($OutputFile)
if (Test-Path -LiteralPath $output) { throw "Choose a new archive path: $output already exists." }
$packager = Join-Path $env:XEDK 'bin\win32\xuipkg.exe'
if (-not (Test-Path -LiteralPath $packager -PathType Leaf)) { throw "Missing XDK packager: $packager" }

$project = Join-Path $repo 'snes9x-1.51-src-d3d\win32'
$skin = Join-Path $repo 'snes9x-1.51-src-d3d\xbox\Skin'
$files = @(Get-ChildItem -LiteralPath $skin -File | Where-Object {
    # Compile each scene from its source rather than also including an older XUR.
    $_.Extension -ne '.xur' -or -not (Test-Path -LiteralPath (Join-Path $skin ($_.BaseName + '.xui')))
} | ForEach-Object { '..\Xbox\Skin\' + $_.Name })
$files += @(Get-ChildItem -LiteralPath (Join-Path $skin 'Graphics') -File | ForEach-Object {
    '..\Xbox\Skin\Graphics\' + $_.Name
})
Push-Location $project
try {
    & $packager /nologo /d /o $output @files
    if ($LASTEXITCODE -ne 0) { throw 'XUI packaging failed.' }
} finally { Pop-Location }
