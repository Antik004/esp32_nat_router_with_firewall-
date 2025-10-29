#define CONFIG_PAGE "<html>\
<head></head>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
<style>\
body {\
font-family: apercu-pro, -apple-system, system-ui, BlinkMacSystemFont, 'Helvetica Neue', sans-serif;\
padding: 1em;\
line-height: 2em;\
font-weight: 100;\
}\
\
td {\
font-weight: 100;\
min-height: 24px;\
}\
\
td:first-child { \
text-align: right;\
min-width: 100px;\
padding-right: 10px;\
}\
\
h1 {\
font-size: 1.5em;\
font-weight: 200;\
}\
\
h2 {\
font-size: 1.2em;\
font-weight: 200;\
margin-left: 5px;\
}\
\
input {\
border: 1px solid rgb(196, 196, 196);\
color: rgb(76, 76, 76);\
width: 240px;\
border-radius: 3px;\
height: 40px;\
margin: 3px 0px;\
padding: 0px 14px;\
}\
\
input:focus {\
border:1px solid black;\
outline: none !important;\
box-shadow: 0 0 10px #719ECE;\
}\
\
#config {\
width:400px; \
margin:0 auto;\
}\
\
.ok-button {\
background-color: #0078e7;\
color: #fff;\
}\
\
.red-button {\
background-color: #e72e00;\
color: #fff;\
}\
</style>\
<body>\
<div id='config'>\
<h1>ESP32 NAT Router Config</h1>\
<script>\
if (window.location.search.substr(1) != '')\
{\
document.getElementById('config').display = 'none';\
document.body.innerHTML ='<h1>ESP32 NAT Router Config</h1>The new settings have been sent to the device.<br/>The page will refresh soon automatically...';\
setTimeout(\"location.href = '/'\",10000);\
}\
</script>\
<h2>AP Settings (the new network)</h2>\
<form action='' method='GET'>\
<table>\
<tr>\
<td>SSID</td>\
<td><input type='text' name='ap_ssid' value='%s' placeholder='SSID of the new network'/></td>\
</tr>\
<tr>\
<td>Password</td>\
<td><input type='text' name='ap_password' value='%s' placeholder='Password of the new network'/></td>\
</tr>\
<tr>\
<td></td>\
<td><input type='submit' value='Set' class='ok-button'/></td>\
</tr>\
</table>\
<small>\
<i>Password </i>less than 8 chars = open<br />\
</small>\
</form>\
\
<h2>STA Settings (uplink WiFi network)</h2>\
<form action='' method='GET'>\
<table>\
<tr>\
<td>SSID</td>\
<td><input type='text' name='ssid' value='%s' placeholder='SSID of existing network'/></td>\
</tr>\
<tr>\
<td>Password</td>\
<td><input type='text' name='password' value='%s' placeholder='Password of existing network'/></td>\
</tr>\
<tr>\
<td colspan='2'>WPA2 Enterprise settings. Leave blank for regular</td>\
</tr>\
<tr>\
<td>Enterprise username</td>\
<td><input type='text' name='ent_username' value='%s' placeholder='WPA2 Enterprise username'/></td>\
</tr>\
<tr>\
<td>Enterprise identity</td>\
<td><input type='text' name='ent_identity' value='%s' placeholder='WPA2 Enterprise identity'/></td>\
</tr>\
<tr>\
<td></td>\
<td><input type='submit' value='Connect' class='ok-button'/></td>\
</tr>\
\
</table>\
</form>\
\
<h2>STA Static IP Settings</h2>\
<form action='' method='GET'>\
<table>\
<tr>\
<td>Static IP</td>\
<td><input type='text' name='staticip' value='%s'/></td>\
</tr>\
<tr>\
<td>Subnet Mask</td>\
<td><input type='text' name='subnetmask' value='%s'/></td>\
</tr>\
<tr>\
<td>Gateway</td>\
<td><input type='text' name='gateway' value='%s'/></td>\
</tr>\
<tr>\
<td></td>\
<td><input type='submit' value='Connect' class='ok-button'/></td>\
</tr>\
\
</table>\
<small>\
<i>Leave it in blank if you want your ESP32 to get an IP using DHCP</i>\
</small>\
</form>\
\
<h2>Device Management</h2>\
<form action='' method='GET'>\
<table>\
<tr>\
<td>Device</td>\
<td><input type='submit' name='reset' value='Reboot' class='red-button'/></td>\
</tr>\
</table>\
</form>\
</div>\
\
<p>\
    Already configured? Go to the <a href=\"/firewall\">Firewall Page</a>\
</p>\
\
</body>\
</html>\
"



#define LOCK_PAGE "<html>\
<head></head>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
<style>\
body {\
font-family: apercu-pro, -apple-system, system-ui, BlinkMacSystemFont, 'Helvetica Neue', sans-serif;\
padding: 1em;\
line-height: 2em;\
font-weight: 100;\
}\
\
td {\
font-weight: 100;\
min-height: 24px;\
}\
\
td:first-child { \
text-align: right;\
min-width: 100px;\
padding-right: 10px;\
}\
\
h1 {\
font-size: 1.5em;\
font-weight: 200;\
}\
\
h2 {\
font-size: 1.2em;\
font-weight: 200;\
margin-left: 5px;\
}\
\
input {\
border: 1px solid rgb(196, 196, 196);\
color: rgb(76, 76, 76);\
width: 240px;\
border-radius: 3px;\
height: 40px;\
margin: 3px 0px;\
padding: 0px 14px;\
}\
\
input:focus {\
border:1px solid black;\
outline: none !important;\
box-shadow: 0 0 10px #719ECE;\
}\
\
#config {\
width:400px; \
margin:0 auto;\
}\
\
.ok-button {\
background-color: #0078e7;\
color: #fff;\
}\
\
.red-button {\
background-color: #e72e00;\
color: #fff;\
}\
</style>\
<body>\
<div id='config'>\
<h1>ESP32 NAT Router Config</h1>\
<script>\
if (window.location.search.substr(1) != '')\
{\
document.getElementById('config').display = 'none';\
document.body.innerHTML ='<h1>ESP32 NAT Router Config</h1>The new settings have been sent to the device.<br/>The page will refresh soon automatically...';\
setTimeout(\"location.href = '/'\",1000);\
}\
</script>\
<h2>Config Locked</h2>\
<form autocomplete='off' action='' method='GET'>\
<table>\
<tr>\
<td>Password:</td>\
<td><input type='password' name='unlock_password'/></td>\
</tr>\
<tr>\
<td></td>\
<td><input type='submit' value='Unlock' class='red-button'/></td>\
</tr>\
\
</table>\
<small>\
<i>Default: STA password to unlock<br />\
</small>\
</form>\
</div>\
</body>\
</html>\
"




#define FIREWALL_PAGE "<html>\
<head></head>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
<style>\
body { font-family: sans-serif; padding:1em; line-height:2em;}\
table {margin-top:1em;}\
.red-button {background-color:#e72e00;color:#fff;border:none;padding:6px 12px;border-radius:3px;}\
</style>\
<body>\
<h1>Firewall - Blocked IPs</h1>\
<form action='' method='GET'>\
<table>\
<tr>\
<td>IP to block</td>\
<td><input type='text' name='block_ip' placeholder='192.168.4.x'/></td>\
<td><input type='submit' value='Block' class='red-button'/></td>\
</tr>\
</table>\
</form>\
</body>\
</html>"

#define DASHBOARD_PAGE "<html>\
<head>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
<title>ESP32 Live Dashboard</title>\
<style>\
body { font-family: Arial, sans-serif; padding: 1em; background: #f4f4f9; }\
h1 { text-align: center; color: #333; }\
.card { background: white; padding: 1em; margin: 1em auto; border-radius: 10px; max-width: 600px; box-shadow: 0 2px 6px rgba(0,0,0,0.1); }\
table { width: 100%; border-collapse: collapse; }\
th, td { border-bottom: 1px solid #ccc; padding: 8px; text-align: left; }\
.refresh { text-align: center; margin-top: 10px; color: #777; font-size: 0.9em; }\
</style>\
<script>\
function fetchData() {\
  fetch('/api/dashboard')\
    .then(response => response.json())\
    .then(data => {\
      document.getElementById('clients').innerHTML = data.clients.join('<br>');\
      document.getElementById('blocked').innerHTML = data.blocked.join('<br>');\
      document.getElementById('traffic').innerHTML = data.traffic.join('<br>');\
      document.getElementById('lastUpdate').innerText = new Date().toLocaleTimeString();\
    });\
}\
setInterval(fetchData, 3000);\
window.onload = fetchData;\
</script>\
</head>\
<body>\
<h1>ESP32 Live Dashboard</h1>\
<div class='card'>\
  <h2>Connected Clients</h2>\
  <div id='clients'>Loading...</div>\
</div>\
<div class='card'>\
  <h2>Recently Blocked Domains</h2>\
  <div id='blocked'>Loading...</div>\
</div>\
<div class='card'>\
  <h2>Recent Traffic Logs</h2>\
  <div id='traffic'>Loading...</div>\
</div>\
<p class='refresh'>Last updated at: <span id='lastUpdate'>--:--:--</span></p>\
</body>\
</html>"
