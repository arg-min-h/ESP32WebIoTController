// ページ読み込み時にシステムテーマを取得してbodyのクラスを設定する
document.addEventListener("DOMContentLoaded", function () {
  const prefersDarkScheme = window.matchMedia(
    "(prefers-color-scheme: dark)",
  ).matches;
  const body = document.body;
  body.classList.toggle("dark-theme", prefersDarkScheme);
  body.classList.toggle("light-theme", !prefersDarkScheme);
  document
    .querySelector('meta[name="theme-color"]')
    .setAttribute("content", prefersDarkScheme ? "#333333" : "#ffffff");
});
