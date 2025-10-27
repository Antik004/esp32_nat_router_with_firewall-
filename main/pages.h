#define CONFIG_PAGE "<!DOCTYPE html>\
<html lang='en'>\
<head>\
<meta charset='UTF-8' />\
<meta name='viewport' content='width=device-width, initial-scale=1.0' />\
<title>ESP32 NAT Router Config</title>\
<style>\
@import url('https://fonts.googleapis.com/css2?family=Poppins:wght@400;600&display=swap');\
body {\
  font-family: 'Poppins', sans-serif;\
  background: linear-gradient(135deg, #1e3c72, #2a5298);\
  color: #333;\
  display: flex;\
  justify-content: center;\
  align-items: flex-start;\
  min-height: 100vh;\
  margin: 0;\
  padding: 40px 0;\
}\
.container {\
  width: 90%;\
  max-width: 450px;\
  background: #fff;\
  padding: 25px;\
  border-radius: 15px;\
  box-shadow: 0 8px 25px rgba(0, 0, 0, 0.2);\
  animation: fadeIn 0.8s ease;\
}\
h2 {\
  text-align: center;\
  color: #2a5298;\
  margin-bottom: 20px;\
}\
.card {\
  background: #f7f9fc;\
  padding: 20px;\
  border-radius: 10px;\
  margin-bottom: 20px;\
  border-left: 5px solid #2a5298;\
  transition: transform 0.3s ease, box-shadow 0.3s ease;\
}\
.card:hover {\
  transform: scale(1.02);\
  box-shadow: 0 6px 15px rgba(0, 0, 0, 0.1);\
}\
h3 {\
  color: #1e3c72;\
  margin-bottom: 15px;\
}\
label {\
  display: block;\
  margin-top: 10px;\
  font-weight: 500;\
}\
input {\
  width: 100%;\
  padding: 10px;\
  margin-top: 5px;\
  border: 1px solid #ccc;\
  border-radius: 8px;\
  transition: 0.3s;\
  outline: none;\
}\
input:focus {\
  border-color: #2a5298;\
  box-shadow: 0 0 5px rgba(42, 82, 152, 0.4);\
}\
.btn {\
  display: block;\
  width: 100%;\
  background: #2a5298;\
  color: white;\
  border: none;\
  padding: 10px;\
  font-size: 16px;\
  border-radius: 8px;\
  cursor: pointer;\
  margin-top: 15px;\
  transition: 0.3s;\
}\
.btn:hover {\
  background: #1e3c72;\
}\
.note {\
  font-size: 12px;\
  color: #555;\
  margin-top: 8px;\
  text-align: center;\
}\
@keyframes fadeIn {\
  from { opacity: 0; transform: translateY(-10px);}\
  to { opacity: 1; transform: translateY(0);}\
}\
</style>\
</head>\
<body>\
  <div class='container'>\
    <h2>⚙️ ESP32 NAT Router Config</h2>\
    <div class='card'>\
      <h3>AP Settings (New Network)</h3>\
      <form id='apForm'>\
        <label for='apSsid'>SSID</label>\
        <input type='text' id='apSsid' placeholder='Enter AP SSID' />\
        <label for='apPassword'>Password</label>\
        <input type='password' id='apPassword' placeholder='Enter password (min 8 chars)' />\
        <button type='submit' class='btn'>Set</button>\
        <p class='note'>Password less than 8 chars = open</p>\
      </form>\
    </div>\
    <div class='card'>\
      <h3>STA Settings (Uplink WiFi Network)</h3>\
      <form id='staForm'>\
        <label for='staSsid'>SSID</label>\
        <input type='text' id='staSsid' placeholder='Existing WiFi SSID' />\
        <label for='staPassword'>Password</label>\
        <input type='password' id='staPassword' placeholder='Existing WiFi Password' />\
        <label for='enterpriseUsername'>Enterprise Username (optional)</label>\
        <input type='text' id='enterpriseUsername' placeholder='WPA2 Enterprise username' />\
        <label for='enterpriseIdentity'>Enterprise Identity (optional)</label>\
        <input type='text' id='enterpriseIdentity' placeholder='WPA2 Enterprise identity' />\
        <button type='submit' class='btn'>Connect</button>\
      </form>\
    </div>\
  </div>\
<script>\
document.getElementById('apForm').addEventListener('submit', (e) => {\
  e.preventDefault();\
  const ssid = document.getElementById('apSsid').value;\
  const password = document.getElementById('apPassword').value;\
  alert(`AP Configured:\\nSSID: ${ssid}\\nPassword: ${password || 'Open Network'}`);\
});\
document.getElementById('staForm').addEventListener('submit', (e) => {\
  e.preventDefault();\
  const ssid = document.getElementById('staSsid').value;\
  const password = document.getElementById('staPassword').value;\
  alert(`Connecting to:\\nSSID: ${ssid}\\nPassword: ${password ? '********' : 'No Password'}`);\
});\
</script>\
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
