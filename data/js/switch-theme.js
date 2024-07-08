function toggleTheme() {
  const body = document.body;
  if (body.classList.contains("light-theme")) {
    body.classList.remove("light-theme");
    body.classList.add("dark-theme");
    document
      .querySelector('meta[name="theme-color"]')
      .setAttribute("content", "#333333");
    saveThemePreference("dark");
  } else {
    body.classList.remove("dark-theme");
    body.classList.add("light-theme");
    document
      .querySelector('meta[name="theme-color"]')
      .setAttribute("content", "#ffffff");
    saveThemePreference("light");
  }
}

function saveThemePreference(theme) {
  localStorage.setItem("theme", theme);
}

document.addEventListener("DOMContentLoaded", function () {
  const savedTheme = localStorage.getItem("theme");
  const body = document.body;
  if (savedTheme === "dark") {
    body.classList.add("dark-theme");
    document
      .querySelector('meta[name="theme-color"]')
      .setAttribute("content", "#333333");
  } else {
    body.classList.add("light-theme");
    document
      .querySelector('meta[name="theme-color"]')
      .setAttribute("content", "#ffffff");
  }

  document.getElementById("themeToggle").addEventListener("click", toggleTheme);
});
