#define CONFIG_PAGE "<!DOCTYPE html>\
<html lang='en'>\
<head>\
<meta charset='UTF-8'>\
<meta name='viewport' content='width=device-width, initial-scale=1.0'>\
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
  max-width: 480px;\
  background: #fff;\
  padding: 25px;\
  border-radius: 15px;\
  box-shadow: 0 8px 25px rgba(0, 0, 0, 0.2);\
  animation: fadeIn 0.8s ease;\
}\
h1 {\
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
h2 {\
  color: #1e3c72;\
  margin-bottom: 10px;\
  text-align: center;\
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
  from {opacity: 0; transform: translateY(-10px);}\
  to {opacity: 1; transform: translateY(0);}\
}\
</style>\
</head>\
<body>\
<div class='container'>\
<h1>ESP32 NAT Router Config</h1>\
<script>\
if (window.location.search.substr(1) != '') {\
  document.body.innerHTML = '<h1>ESP32 NAT Router Config</h1><p>The new settings have been sent to the device.<br/>The page will refresh soon automatically...</p>';\
  setTimeout(function(){location.href='/'}, 8000);\
}\
</script>\
\
<div class='card'>\
<h2>AP Settings (New Network)</h2>\
<form action='' method='GET'>\
<label>SSID</label>\
<input type='text' name='ap_ssid' value='%s' placeholder='SSID of the new network'>\
<label>Password</label>\
<input type='text' name='ap_password' value='%s' placeholder='Password (min 8 chars)'>\
<button type='submit' class='btn'>Set</button>\
<p class='note'>Leave password empty for open network</p>\
</form>\
</div>\
\
<div class='card'>\
<h2>STA Settings (Uplink WiFi)</h2>\
<form action='' method='GET'>\
<label>SSID</label>\
<input type='text' name='ssid' value='%s' placeholder='SSID of existing network'>\
<label>Password</label>\
<input type='password' name='password' value='%s' placeholder='Password of existing network'>\
<label>Enterprise username</label>\
<input type='text' name='ent_username' value='%s' placeholder='WPA2 Enterprise username'>\
<label>Enterprise identity</label>\
<input type='text' name='ent_identity' value='%s' placeholder='WPA2 Enterprise identity'>\
<button type='submit' class='btn'>Connect</button>\
</form>\
</div>\
\
<div class='card'>\
<h2>Static IP Settings</h2>\
<form action='' method='GET'>\
<label>Static IP</label>\
<input type='text' name='staticip' value='%s'>\
<label>Subnet Mask</label>\
<input type='text' name='subnetmask' value='%s'>\
<label>Gateway</label>\
<input type='text' name='gateway' value='%s'>\
<button type='submit' class='btn'>Apply</button>\
<p class='note'>Leave blank for DHCP</p>\
</form>\
</div>\
\
<div class='card'>\
<h2>Device Management</h2>\
<form action='' method='GET'>\
<button type='submit' name='reset' value='Reboot' class='btn' style='background:#e72e00;'>Reboot Device</button>\
</form>\
</div>\
\
<div class='card' style='text-align:center;'>\
<h2>Firewall</h2>\
<p>Go to the Firewall Page to manage block/allow rules.</p>\
<a href='/firewall'><button class='btn'>Open Firewall Page</button></a>\
</div>\
\
<p class='note'>© 2025 ESP32 NAT Router</p>\
</div>\
</body>\
</html>"

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




#define FIREWALL_PAGE "<!DOCTYPE HTML>\
<html>\
  <head>\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
    <link href=\"https://fonts.googleapis.com/css2?family=Poppins:wght@400;600&display=swap\" rel=\"stylesheet\">\
    <style>\
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
      .btn-danger {\
        background: #e63946;\
      }\
      .btn-danger:hover {\
        background: #c92a36;\
      }\
      ul {\
        list-style-type: none;\
        padding-left: 0;\
        margin-top: 10px;\
      }\
      ul li {\
        background: #f1f3f8;\
        padding: 8px 10px;\
        margin: 6px 0;\
        border-radius: 6px;\
        color: #333;\
        font-size: 14px;\
      }\
      @keyframes fadeIn {\
        from { opacity: 0; transform: translateY(-10px); }\
        to { opacity: 1; transform: translateY(0); }\
      }\
    </style>\
  </head>\
  <body>\
    <div class=\"container\">\
      <h2>ESP32 Firewall Control Panel</h2>\
      <div class=\"card\">\
        <h3>Add IP/Domain</h3>\
        <form action=\"/add_ip\" method=\"POST\">\
          <label>Domain/IP:</label>\
          <input type=\"text\" name=\"ipfield\" placeholder=\"Enter domain or IP\" required>\
          <button class=\"btn\" type=\"submit\">Add IP</button>\
        </form>\
      </div>\
      <div class=\"card\">\
        <h3>Remove IP/Domain</h3>\
        <form action=\"/remove_ip\" method=\"POST\">\
          <label>Domain/IP:</label>\
          <input type=\"text\" name=\"ipfield\" placeholder=\"Enter domain or IP\" required>\
          <button class=\"btn btn-danger\" type=\"submit\">Remove IP</button>\
        </form>\
      </div>\
      <div class=\"card\">\
        <h3>Inspect Traffic</h3>\
        <p>View live packet information and connection details in real time.</p>\
        <form action=\"/inspect_packet\" method=\"GET\">\
            <button class=\"btn\" type=\"submit\" onclick=\"window.location.href='/dashboard'\">Inspect</button>\
        </form>\
      </div>\
      <div class=\"card\">\
        <h3>Current Firewall Rules</h3>\
        <ul>\
          <!-- Dynamically insert list of IPs/Domains here -->\
        </ul>\
      </div>\
    </div>\
  </body>\
</html>"


#define DASHBOARD_PAGE "\
<!DOCTYPE html>\
<html>\
<head>\
  <meta charset=\"UTF-8\">\
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
  <title>ESP32 Firewall Dashboard</title>\
  <style>\
    body { font-family: Arial, sans-serif; background-color: #0d1117; color: #c9d1d9; margin: 0; padding: 20px; }\
    h2 { color: #58a6ff; }\
    .card { background-color: #161b22; border-radius: 10px; padding: 20px; margin-bottom: 20px; box-shadow: 0 0 10px rgba(0,0,0,0.3); }\
    .btn { background-color: #238636; color: white; border: none; padding: 10px 20px; border-radius: 5px; cursor: pointer; }\
    .btn:hover { background-color: #2ea043; }\
    pre { background: #000; color: #0f0; padding: 10px; border-radius: 8px; height: 300px; overflow-y: scroll; font-size: 13px; }\
  </style>\
</head>\
<body>\
  <h2>ESP32 Firewall Dashboard</h2>\
  <div class=\"card\">\
    <h3>Live Logs</h3>\
    <pre id=\"logBox\">Loading logs...</pre>\
  </div>\
  <script>\
    async function fetchLogs() {\
      try {\
        const res = await fetch('/logs');\
        const text = await res.text();\
        const logBox = document.getElementById('logBox');\
        logBox.textContent = text;\
        logBox.scrollTop = logBox.scrollHeight;\
      } catch (err) {\
        console.error('Error fetching logs:', err);\
      }\
    }\
    setInterval(fetchLogs, 2000);\
    fetchLogs();\
  </script>\
</body>\
</html>"
