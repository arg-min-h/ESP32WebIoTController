document.addEventListener("DOMContentLoaded", function () {
  const prefersDarkScheme = window.matchMedia(
    "(prefers-color-scheme: dark)",
  ).matches;
  const savedTheme = localStorage.getItem("theme");

  if (savedTheme) {
    document.body.classList.toggle("dark", savedTheme === "dark");
    document.body.classList.toggle("light", savedTheme === "light");
  } else {
    document.body.classList.toggle("dark", prefersDarkScheme);
    document.body.classList.toggle("light", !prefersDarkScheme);
  }

  document
    .querySelector('meta[name="theme-color"]')
    .setAttribute("content", prefersDarkScheme ? "#333333" : "#ffffff");
});
