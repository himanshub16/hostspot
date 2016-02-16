regsvr32 hnetcfg.dll
$m = New-Object -ComObject HNetCfg.HNetShare
$c = $m.EnumEveryConnection |? { $m.NetConnectionProps.Invoke($_).Name -eq "Ethernet" }
$config = $m.INetSharingConfigurationForINetConnection.Invoke($c)
$config.EnableSharing(0)
netsh winhttp import proxy source=ie
netsh wlan set hostednetwork mode=allow ssid=himanshu key=priyanshu keyUsage=persistent
netsh wlan start hostednetwork
$var = "going"
Write-Host "I won't take much of System's resources. So you can sit back and watch me work"
Write-Host "Type 'q' anytime you want to stop the hotspot"
DO { $var = Read-Host } While ($var.CompareTo("q"))
netsh wlan stop hostednetwork
$config.DisableSharing()
