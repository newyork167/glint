param ($ConanfilePath, $BuildDir); 

if ($BuildDir -eq $null) {
	Write-Host "Must specify build directory if not using CMakeLists!"
	return;
}

Write-Host "Writing conan build files described at $ConanfilePath to $BuildDir";

$project_dir = Get-Location;

if($ConanfilePath -eq $null) {
	$ConanfilePath = "..";
}

if ($BuildDir -ne $null) {
	#$DebugBuildDir = "$project_dir\out\build\x64-Debug (default)";
	conan install $ConanfilePath -s build_type=Debug --no-imports -if="$BuildDir"
	conan imports -if="$BuildDir" -imf "$ConanfilePath/Glint/bindings" $ConanfilePath
}