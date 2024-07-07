// service-worker.js

// キャッシュするファイルのリスト
const cacheName = "esp32-webserver-v1";
const cacheFiles = [
  "/",
  "/index.html",
  "/js/websocket.js",
  "/css/style.css",
  "/favicon.ico",
];

// インストール時の処理
self.addEventListener("install", function (event) {
  event.waitUntil(
    caches.open(cacheName).then(function (cache) {
      return cache.addAll(cacheFiles);
    }),
  );
});

// リクエスト時のキャッシュ利用
self.addEventListener("fetch", function (event) {
  event.respondWith(
    caches.match(event.request).then(function (response) {
      if (response) {
        return response;
      }
      return fetch(event.request);
    }),
  );
});
