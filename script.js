// Handle AP Form
document.getElementById("apForm").addEventListener("submit", (e) => {
  e.preventDefault();
  const ssid = document.getElementById("apSsid").value;
  alert(`✅ AP Configured: ${ssid}`);
});

// Handle STA Form
document.getElementById("staForm").addEventListener("submit", (e) => {
  e.preventDefault();
  const ssid = document.getElementById("staSsid").value;
  alert(`📡 Connecting to: ${ssid}`);
});

// 🍔 Dropdown
const menuToggle = document.getElementById("menu-toggle");
const dropdownMenu = document.getElementById("dropdown-menu");

menuToggle.addEventListener("click", () => {
  menuToggle.classList.toggle("active");
  dropdownMenu.classList.toggle("active");
});

window.addEventListener("click", (e) => {
  if (!menuToggle.contains(e.target) && !dropdownMenu.contains(e.target)) {
    menuToggle.classList.remove("active");
    dropdownMenu.classList.remove("active");
  }
});
