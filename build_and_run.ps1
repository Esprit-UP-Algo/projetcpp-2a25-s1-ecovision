# build_and_run.ps1
# Automates cleaning, building and running the GE project on Windows (PowerShell)
# Usage: Open PowerShell in the project root and run: .\build_and_run.ps1

param(
    [switch]$UseMock  # if present, sets BREVO_USE_MOCK=1 so emails are not sent and codes are shown
)

# Load .env file if it exists (for local development secrets)
if (Test-Path .\.env) {
    Write-Host "Loading environment variables from .env file..."
    Get-Content .\.env | ForEach-Object {
        if ($_ -match '^\s*([^#][^=]+)=(.*)$') {
            $name = $matches[1].Trim()
            $value = $matches[2].Trim()
            # Remove surrounding quotes if present
            $value = $value -replace '^["'']|["'']$', ''
            Set-Item -Path "env:$name" -Value $value
            Write-Host "  Set $name"
        }
    }
}

Write-Host "Cleaning build folder..."
if (Test-Path .\build) { Remove-Item -Recurse -Force .\build }
else { Write-Host "No build folder found, skipping clean." }

if ($UseMock) {
    Write-Host "Enabling mock email mode (BREVO_USE_MOCK=1) for this session."
    $env:BREVO_USE_MOCK = '1'
} else {
    Write-Host "Mock mode not enabled. If you want mock mode, re-run with -UseMock."
}

# Check if Brevo credentials are set (either from .env or manually)
if ([string]::IsNullOrWhiteSpace($env:BREVO_API_KEY) -and [string]::IsNullOrWhiteSpace($env:BREVO_USE_MOCK)) {
    Write-Host ""
    Write-Host "WARNING: BREVO_API_KEY not set. To send real emails, you need to either:" -ForegroundColor Yellow
    Write-Host "  1. Create a .env file with BREVO_API_KEY and BREVO_SENDER_EMAIL" -ForegroundColor Yellow
    Write-Host "  2. Set environment variables manually before running this script" -ForegroundColor Yellow
    Write-Host "  3. Run with -UseMock flag for testing without sending emails" -ForegroundColor Yellow
    Write-Host ""
}

Write-Host "Running qmake..."
qmake GE.pro

Write-Host "Building (mingw32-make)..."
# Capture build output to build.log
mingw32-make 2>&1 | Tee-Object -FilePath build.log

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed. See build.log for details."
    exit $LASTEXITCODE
}

# Try to locate the generated executable
$exePaths = @(
    '.\build\Desktop_Qt_6_7_3_MinGW_64_bit-Debug\debug\GE.exe',
    '.\build\debug\GE.exe',
    '.\debug\GE.exe'
)

$exe = $exePaths | Where-Object { Test-Path $_ } | Select-Object -First 1
if (-not $exe) {
    Write-Host "Executable not found in expected locations. Search for GE.exe in build output..."
    $exe = Get-ChildItem -Path . -Filter GE.exe -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($exe) { $exe = $exe.FullName } else { $exe = $null }
}

if ($exe) {
    Write-Host "Launching: $exe"
    
    # Build environment variables string for the process
    $startInfo = New-Object System.Diagnostics.ProcessStartInfo
    $startInfo.FileName = $exe
    $startInfo.UseShellExecute = $false
    
    # Pass all current environment variables to the child process
    foreach ($key in [System.Environment]::GetEnvironmentVariables().Keys) {
        $value = [System.Environment]::GetEnvironmentVariable($key)
        $startInfo.EnvironmentVariables[$key] = $value
    }
    
    # Ensure our specific variables are set
    if ($env:BREVO_USE_MOCK) {
        $startInfo.EnvironmentVariables["BREVO_USE_MOCK"] = $env:BREVO_USE_MOCK
        Write-Host "  Passing BREVO_USE_MOCK=$($env:BREVO_USE_MOCK) to application"
    }
    if ($env:BREVO_API_KEY) {
        $startInfo.EnvironmentVariables["BREVO_API_KEY"] = $env:BREVO_API_KEY
        Write-Host "  Passing BREVO_API_KEY to application (length: $($env:BREVO_API_KEY.Length) chars)"
    }
    if ($env:BREVO_SENDER_EMAIL) {
        $startInfo.EnvironmentVariables["BREVO_SENDER_EMAIL"] = $env:BREVO_SENDER_EMAIL
        Write-Host "  Passing BREVO_SENDER_EMAIL=$($env:BREVO_SENDER_EMAIL) to application"
    }
    
    [System.Diagnostics.Process]::Start($startInfo) | Out-Null
} else {
    Write-Host "Could not find the built executable. Please open the project in Qt Creator and run it from there." 
}

Write-Host "Done. If build failed, open 'build.log' and paste the last lines here and I'll help debug."
