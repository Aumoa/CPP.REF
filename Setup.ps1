Push-Location Registry::HKEY_CLASSES_ROOT
	if (Test-Path ".sproject") {
		Remove-Item -Path ".sproject" -Recurse -Force
	}

	Write-Output "Register '.sproject' types to 'Registry::HKEY_CLASSES_ROOT\.sproject'..."

	New-Item -Path ".sproject" > $null
	Push-Location ".sproject"
		New-Item -Path "shell" > $null
		Push-Location "shell"
			New-Item -Path "BuildTool" > $null
			Set-Item -Path "BuildTool" -Value "Generate CPP.REF Solution..."
			Push-Location "BuildTool"
				New-Item -Path "command" > $null
				Set-Item -Path "command" -Value "$PSScriptRoot\Binaries\BuildTool\BuildTool.exe %1"
			Pop-Location
		Pop-Location
	Pop-Location
	Write-Output "Done."

Pop-Location