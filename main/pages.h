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
<p>Go to Dashboard to see live logs</p>\
<a href='/dashboard'><button class='btn'>Open Dashboard Page</button></a>\
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
// Replace DASHBOARD_PAGE in pages.h with this version (no emoji/special chars):

#define DASHBOARD_PAGE "\
<!DOCTYPE html>\
<html>\
<head>\
  <meta charset=\"UTF-8\">\
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
  <title>ESP32 Firewall Dashboard</title>\
  <style>\
    * { margin: 0; padding: 0; box-sizing: border-box; }\
    body { \
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; \
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); \
      color: #c9d1d9; \
      padding: 20px; \
      min-height: 100vh; \
    }\
    .container { \
      max-width: 1200px; \
      margin: 0 auto; \
    }\
    h1 { \
      color: white; \
      text-align: center; \
      margin-bottom: 30px; \
      font-size: 2.5em; \
      text-shadow: 2px 2px 4px rgba(0,0,0,0.3); \
    }\
    .card { \
      background: rgba(22, 27, 34, 0.95); \
      border-radius: 15px; \
      padding: 25px; \
      margin-bottom: 25px; \
      box-shadow: 0 8px 32px rgba(0,0,0,0.4); \
      backdrop-filter: blur(10px); \
      border: 1px solid rgba(255,255,255,0.1); \
    }\
    h2 { \
      color: #58a6ff; \
      margin-bottom: 15px; \
      font-size: 1.5em; \
      border-bottom: 2px solid #58a6ff; \
      padding-bottom: 10px; \
    }\
    .stats { \
      display: flex; \
      gap: 15px; \
      margin-bottom: 15px; \
      flex-wrap: wrap; \
    }\
    .stat-box { \
      flex: 1; \
      min-width: 150px; \
      background: rgba(88, 166, 255, 0.1); \
      padding: 15px; \
      border-radius: 10px; \
      border-left: 4px solid #58a6ff; \
    }\
    .stat-label { \
      color: #8b949e; \
      font-size: 0.9em; \
      margin-bottom: 5px; \
    }\
    .stat-value { \
      color: #58a6ff; \
      font-size: 1.8em; \
      font-weight: bold; \
    }\
    pre { \
      background: #000; \
      color: #0f0; \
      padding: 15px; \
      border-radius: 10px; \
      height: 300px; \
      overflow-y: auto; \
      font-size: 13px; \
      font-family: 'Courier New', monospace; \
      line-height: 1.5; \
      border: 1px solid #333; \
    }\
    pre::-webkit-scrollbar { \
      width: 8px; \
    }\
    pre::-webkit-scrollbar-track { \
      background: #1a1a1a; \
    }\
    pre::-webkit-scrollbar-thumb { \
      background: #0f0; \
      border-radius: 4px; \
    }\
    table { \
      width: 100%; \
      border-collapse: collapse; \
      margin-top: 15px; \
    }\
    th, td { \
      border: 1px solid #30363d; \
      padding: 12px; \
      text-align: center; \
    }\
    th { \
      background: rgba(88, 166, 255, 0.2); \
      color: #58a6ff; \
      font-weight: 600; \
      text-transform: uppercase; \
      font-size: 0.85em; \
      letter-spacing: 1px; \
    }\
    tr:hover { \
      background: rgba(88, 166, 255, 0.05); \
    }\
    .btn { \
      background: linear-gradient(135deg, #238636 0%, #2ea043 100%); \
      color: white; \
      border: none; \
      padding: 8px 20px; \
      border-radius: 6px; \
      cursor: pointer; \
      margin: 2px; \
      font-weight: 600; \
      transition: all 0.3s ease; \
      box-shadow: 0 2px 8px rgba(35, 134, 54, 0.3); \
    }\
    .btn:hover { \
      background: linear-gradient(135deg, #2ea043 0%, #238636 100%); \
      transform: translateY(-2px); \
      box-shadow: 0 4px 12px rgba(35, 134, 54, 0.5); \
    }\
    .btn-block { \
      background: linear-gradient(135deg, #da3633 0%, #f85149 100%); \
      box-shadow: 0 2px 8px rgba(218, 54, 51, 0.3); \
    }\
    .btn-block:hover { \
      background: linear-gradient(135deg, #f85149 0%, #da3633 100%); \
      box-shadow: 0 4px 12px rgba(218, 54, 51, 0.5); \
    }\
    .status-badge { \
      display: inline-block; \
      padding: 5px 12px; \
      border-radius: 20px; \
      font-size: 0.85em; \
      font-weight: bold; \
      text-transform: uppercase; \
    }\
    .status-pending { \
      background: rgba(187, 128, 9, 0.2); \
      color: #f0ad4e; \
      border: 1px solid #f0ad4e; \
    }\
    .status-allowed { \
      background: rgba(35, 134, 54, 0.2); \
      color: #3fb950; \
      border: 1px solid #3fb950; \
    }\
    .status-blocked { \
      background: rgba(218, 54, 51, 0.2); \
      color: #f85149; \
      border: 1px solid #f85149; \
    }\
    .empty-state { \
      text-align: center; \
      padding: 40px; \
      color: #8b949e; \
      font-style: italic; \
    }\
    .refresh-indicator { \
      display: inline-block; \
      width: 8px; \
      height: 8px; \
      border-radius: 50%; \
      background: #3fb950; \
      margin-left: 10px; \
      animation: pulse 2s infinite; \
    }\
    @keyframes pulse { \
      0%, 100% { opacity: 1; } \
      50% { opacity: 0.3; } \
    }\
    .back-btn { \
      display: inline-block; \
      background: rgba(88, 166, 255, 0.2); \
      color: #58a6ff; \
      padding: 10px 20px; \
      border-radius: 8px; \
      text-decoration: none; \
      margin-bottom: 20px; \
      transition: all 0.3s ease; \
      border: 1px solid #58a6ff; \
    }\
    .back-btn:hover { \
      background: rgba(88, 166, 255, 0.3); \
      transform: translateX(-5px); \
    }\
    .add-domain-form { \
      display: flex; \
      gap: 10px; \
      margin-bottom: 15px; \
      flex-wrap: wrap; \
    }\
    .add-domain-form input { \
      flex: 1; \
      min-width: 200px; \
      padding: 10px 15px; \
      background: rgba(22, 27, 34, 0.8); \
      border: 1px solid #30363d; \
      border-radius: 6px; \
      color: #c9d1d9; \
      font-size: 14px; \
    }\
    .add-domain-form input:focus { \
      outline: none; \
      border-color: #58a6ff; \
      box-shadow: 0 0 0 2px rgba(88, 166, 255, 0.2); \
    }\
    .add-domain-form input::placeholder { \
      color: #8b949e; \
    }\
    .domain-list { \
      max-height: 300px; \
      overflow-y: auto; \
    }\
    .domain-item { \
      display: flex; \
      justify-content: space-between; \
      align-items: center; \
      padding: 12px 15px; \
      margin-bottom: 8px; \
      background: rgba(88, 166, 255, 0.05); \
      border-radius: 6px; \
      border-left: 3px solid #f85149; \
    }\
    .domain-name { \
      font-family: 'Courier New', monospace; \
      color: #c9d1d9; \
      font-weight: 500; \
    }\
  </style>\
</head>\
<body>\
  <div class=\"container\">\
    <a href=\"/\" class=\"back-btn\">&larr; Back to Config</a>\
    <h1>ESP32 Firewall Dashboard</h1>\
    \
    <div class=\"card\">\
      <h2>Statistics</h2>\
      <div class=\"stats\">\
        <div class=\"stat-box\">\
          <div class=\"stat-label\">Total Devices</div>\
          <div class=\"stat-value\" id=\"totalDevices\">0</div>\
        </div>\
        <div class=\"stat-box\">\
          <div class=\"stat-label\">Allowed</div>\
          <div class=\"stat-value\" id=\"allowedDevices\" style=\"color: #3fb950;\">0</div>\
        </div>\
        <div class=\"stat-box\">\
          <div class=\"stat-label\">Blocked</div>\
          <div class=\"stat-value\" id=\"blockedDevices\" style=\"color: #f85149;\">0</div>\
        </div>\
        <div class=\"stat-box\">\
          <div class=\"stat-label\">Blocked Domains</div>\
          <div class=\"stat-value\" id=\"blockedDomainsCount\" style=\"color: #f85149;\">0</div>\
        </div>\
      </div>\
    </div>\
    \
    <div class=\"card\">\
      <h2>Domain Blocking<span class=\"refresh-indicator\"></span></h2>\
      <div class=\"add-domain-form\">\
        <input type=\"text\" id=\"domainInput\" placeholder=\"Enter domain to block (e.g., youtube.com or *.facebook.com)\">\
        <button class=\"btn\" onclick=\"addDomain()\">Add Domain</button>\
      </div>\
      <div class=\"domain-list\" id=\"domainList\">\
        <div class=\"empty-state\">No domains blocked yet</div>\
      </div>\
    </div>\
    \
    <div class=\"card\">\
      <h2>Live Logs<span class=\"refresh-indicator\"></span></h2>\
      <pre id=\"logBox\">Loading logs...</pre>\
    </div>\
    \
    <div class=\"card\">\
      <h2>Connected Devices<span class=\"refresh-indicator\"></span></h2>\
      <table id=\"mac-table\">\
        <thead>\
          <tr>\
            <th>MAC Address</th>\
            <th>Status</th>\
            <th>Actions</th>\
          </tr>\
        </thead>\
        <tbody>\
          <tr>\
            <td colspan=\"3\" class=\"empty-state\">Loading devices...</td>\
          </tr>\
        </tbody>\
      </table>\
    </div>\
  </div>\
  \
  <script>\
    async function fetchLogs() {\
      try {\
        const res = await fetch('/logs');\
        const text = await res.text();\
        const logBox = document.getElementById('logBox');\
        logBox.textContent = text || 'No logs yet...';\
        logBox.scrollTop = logBox.scrollHeight;\
      } catch (err) { \
        console.error('Error fetching logs:', err); \
      }\
    }\
    \
    async function fetchDevices(){\
      try {\
        const resp = await fetch('/devices');\
        const devices = await resp.json();\
        const tbody = document.querySelector('#mac-table tbody');\
        tbody.innerHTML = '';\
        \
        if(devices.length === 0) {\
          tbody.innerHTML = '<tr><td colspan=\"3\" class=\"empty-state\">No devices connected yet</td></tr>';\
          updateStats(0, 0, 0, 0);\
          return;\
        }\
        \
        let allowed = 0, blocked = 0, pending = 0;\
        \
        devices.forEach(dev => {\
          const tr = document.createElement('tr');\
          \
          const macTd = document.createElement('td');\
          macTd.innerText = dev.mac;\
          macTd.style.fontFamily = 'monospace';\
          macTd.style.fontWeight = 'bold';\
          tr.appendChild(macTd);\
          \
          const statusTd = document.createElement('td');\
          const badge = document.createElement('span');\
          badge.className = 'status-badge status-' + dev.status.toLowerCase();\
          badge.innerText = dev.status;\
          statusTd.appendChild(badge);\
          tr.appendChild(statusTd);\
          \
          if(dev.status === 'ALLOWED') allowed++;\
          else if(dev.status === 'BLOCKED') blocked++;\
          else if(dev.status === 'PENDING') pending++;\
          \
          const actionTd = document.createElement('td');\
          \
          if(dev.status !== 'ALLOWED'){\
            const allowBtn = document.createElement('button');\
            allowBtn.innerText = 'Allow';\
            allowBtn.className = 'btn';\
            allowBtn.onclick = () => updateMac(dev.mac, 'ALLOWED');\
            actionTd.appendChild(allowBtn);\
          }\
          \
          if(dev.status !== 'BLOCKED'){\
            const blockBtn = document.createElement('button');\
            blockBtn.innerText = 'Block';\
            blockBtn.className = 'btn btn-block';\
            blockBtn.onclick = () => {\
              if(confirm('Block device ' + dev.mac + '?')){\
                updateMac(dev.mac, 'BLOCKED');\
              }\
            };\
            actionTd.appendChild(blockBtn);\
          }\
          \
          tr.appendChild(actionTd);\
          tbody.appendChild(tr);\
        });\
        \
        updateStats(devices.length, allowed, blocked, pending);\
      } catch(e){ \
        console.error('Error fetching devices:', e); \
      }\
    }\
    \
    async function fetchBlockedDomains() {\
      try {\
        const resp = await fetch('/blocked_domains');\
        const domains = await resp.json();\
        const domainList = document.getElementById('domainList');\
        const countElem = document.getElementById('blockedDomainsCount');\
        \
        countElem.innerText = domains.length;\
        \
        if(domains.length === 0) {\
          domainList.innerHTML = '<div class=\"empty-state\">No domains blocked yet</div>';\
          return;\
        }\
        \
        domainList.innerHTML = '';\
        domains.forEach(item => {\
          const div = document.createElement('div');\
          div.className = 'domain-item';\
          \
          const span = document.createElement('span');\
          span.className = 'domain-name';\
          span.innerText = item.domain;\
          \
          const removeBtn = document.createElement('button');\
          removeBtn.className = 'btn btn-block';\
          removeBtn.innerText = 'Remove';\
          removeBtn.onclick = () => removeDomain(item.domain);\
          \
          div.appendChild(span);\
          div.appendChild(removeBtn);\
          domainList.appendChild(div);\
        });\
      } catch(e) {\
        console.error('Error fetching domains:', e);\
      }\
    }\
    \
    async function addDomain() {\
      const input = document.getElementById('domainInput');\
      const domain = input.value.trim();\
      \
      if(!domain) {\
        alert('Please enter a domain');\
        return;\
      }\
      \
      try {\
        const resp = await fetch('/add_domain', {\
          method: 'POST',\
          headers: { 'Content-Type': 'application/json' },\
          body: JSON.stringify({ domain: domain })\
        });\
        \
        if(resp.ok) {\
          input.value = '';\
          fetchBlockedDomains();\
          fetchLogs();\
        } else {\
          alert('Failed to add domain');\
        }\
      } catch(e) {\
        console.error('Error adding domain:', e);\
        alert('Error adding domain');\
      }\
    }\
    \
    async function removeDomain(domain) {\
      if(!confirm('Remove ' + domain + ' from blocked list?')) {\
        return;\
      }\
      \
      try {\
        const resp = await fetch('/remove_domain', {\
          method: 'POST',\
          headers: { 'Content-Type': 'application/json' },\
          body: JSON.stringify({ domain: domain })\
        });\
        \
        if(resp.ok) {\
          fetchBlockedDomains();\
          fetchLogs();\
        } else {\
          alert('Failed to remove domain');\
        }\
      } catch(e) {\
        console.error('Error removing domain:', e);\
        alert('Error removing domain');\
      }\
    }\
    \
    function updateStats(total, allowed, blocked, pending) {\
      document.getElementById('totalDevices').innerText = total;\
      document.getElementById('allowedDevices').innerText = allowed;\
      document.getElementById('blockedDevices').innerText = blocked;\
    }\
    \
    async function updateMac(mac, status){\
      try {\
        const response = await fetch('/update_mac', {\
          method: 'POST',\
          headers: { 'Content-Type': 'application/json' },\
          body: JSON.stringify({ mac: mac, status: status })\
        });\
        \
        if(response.ok) {\
          fetchDevices();\
          fetchLogs();\
        } else {\
          alert('Failed to update device status');\
        }\
      } catch(e) {\
        console.error('Error updating MAC:', e);\
      }\
    }\
    \
    document.getElementById('domainInput').addEventListener('keypress', function(e) {\
      if(e.key === 'Enter') {\
        addDomain();\
      }\
    });\
    \
    setInterval(fetchLogs, 2000);\
    setInterval(fetchDevices, 3000);\
    setInterval(fetchBlockedDomains, 5000);\
    \
    fetchLogs();\
    fetchDevices();\
    fetchBlockedDomains();\
  </script>\
</body>\
</html>"