param ($ShaderDir, $SymlinkToDir) 

function Test-ReparsePoint([string]$path) {
	$file = Get-Item $path -Force -ea SilentlyContinue
	return [bool]($file.Attributes -band [IO.FileAttributes]::ReparsePoint)
}

$ShaderDir = $ShaderDir -replace "/","\"
$SymlinkToDir = $SymlinkToDir -replace "/","\"

Write-Host "`"$SymlinkToDir`" `"$ShaderDir`""
cmd /c mklink /J `"$SymlinkToDir\Shaders`" `"$ShaderDir`"
