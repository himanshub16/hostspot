#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <string>

using namespace std;

struct configs
{
    string connToBridge;
    string bridgeOver;// 0 for Ethernet and 1 for Wifi
    string ssid;
    string key;
};

configs* getData()
{
    configs *newConfig = new configs;
    int x;
    cout << "Enter interface to share (internet source) " << endl;
    cout << "1 : Ethernet\n2 : Wi-Fi \n";
    cin >> x;
    switch(x) {
        case 1 : cout << "Ethernet network is to be shared over WiFi" << endl;
                 newConfig->connToBridge = "Ethernet";
                 newConfig->bridgeOver = "Wi-Fi";
                 break;
        case 2 : cout << "WiFi is to be shared over Ethernet" << endl;
                 newConfig->connToBridge = "Wi-Fi";
                 newConfig->bridgeOver = "Ethernet";
                 break;
        default : cout << "Invalid input :("; exit(EXIT_FAILURE);
    }
    cout << "Enter SSID (name of hotspot) : " ; cin >> newConfig->ssid;
    do {
        cout << "Enter password (at least 8 characters) : "; cin >> newConfig->key;
    } while ((newConfig->key).length() < 8);
    return newConfig;
}


int main()
{
    cout << "blah blah blah" << endl;
    fstream file;
    file.open("script.ps1", ios::out);
    configs *conf;
    conf = getData();
    // Register the HNetCfg library (once)
    file << "regsvr32 hnetcfg.dll" << endl;
    // create a NetSharingManager object
    file << "$m = New-Object -ComObject HNetCfg.HNetShare" << endl;
    //find desired connection
    file << "$c = $m.EnumEveryConnection \|\? \{ $m.NetConnectionProps.Invoke($_).Name -eq \"" << conf->connToBridge << "\" \}" << endl;
    //get sharing information and enable sharing
    file << "$config = $m.INetSharingConfigurationForINetConnection.Invoke\($c\)" << endl;
    file << "$config.EnableSharing\(0\)" << endl;

    //disable sharing for the other device
    //file << "$p = $m.EnumEveryConnection \|\? \{ $m.NetConnectionProps.Invoke($_).Name -eq \"" << conf->bridgeOver << "\" \}" << endl;
    //get sharing information and enable sharing
    //file << "$tempconfig = $m.INetSharingConfigurationForINetConnection.Invoke\($p\)" << endl;
    //file << "$tempconfig.DisableSharing\(\)" << endl;

    //starting hostednetwork
    string netsh_arg;
    if (conf->bridgeOver == "Wi-Fi") netsh_arg = "wlan";
    else if (conf->bridgeOver == "Ethernet") netsh_arg = "lan";
    else exit(EXIT_FAILURE);

    cout << "Use proxy settings (y/n) ? ";
    char c; cin >> c;
    if (c == 'y' || c == 'Y') file << "netsh winhttp import proxy source=ie" << endl;
    else file << "netsh winhttp reset proxy" << endl;


    file << "netsh " << netsh_arg << " set hostednetwork mode=allow ssid=" << conf->ssid << " key=" << conf->key << " keyUsage=temporary"<< endl;
    file << "netsh " << netsh_arg << " start hostednetwork" << endl;

    file << "$var = \"going\"" << endl;
    //create a loop
    file << "Write-Host \"I won't take much of System\'s resources. So you can sit back and watch me work\"" << endl;
    file << "Write-Host \"Type 'q' anytime you want to stop the hotspot\"" << endl;
    file << "DO \{ $var = Read-Host \} While \($var.CompareTo\(\"q\"\)\)" << endl;

    file << "netsh " << netsh_arg << " stop hostednetwork" << endl;
    file << "$config.DisableSharing\(\)" << endl;
    //file << "set-executionpolicy unrestricted";

    file.close();

    cout << "Script is ready, you can run script.bat and enjoy.";
    return 0;
}
