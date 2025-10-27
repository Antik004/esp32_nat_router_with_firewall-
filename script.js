// Handle AP Form Submission
document.getElementById("apForm").addEventListener("submit", (e) => {
  e.preventDefault();
  const ssid = document.getElementById("apSsid").value;
  const password = document.getElementById("apPassword").value;

  alert(`AP Configured:\nSSID: ${ssid}\nPassword: ${password || "Open Network"}`);
});

// Handle STA Form Submission
document.getElementById("staForm").addEventListener("submit", (e) => {
  e.preventDefault();
  const ssid = document.getElementById("staSsid").value;
  const password = document.getElementById("staPassword").value;

  alert(`Connecting to:\nSSID: ${ssid}\nPassword: ${password ? "********" : "No Password"}`);
});
