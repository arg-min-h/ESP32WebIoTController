// WebSocketの設定や初期色の取得などは省略
fetch("/color")
  .then((response) => response.json())
  .then((data) => {
    const color = `#${((1 << 24) + (data.r << 16) + (data.g << 8) + data.b).toString(16).slice(1)}`;
    document.getElementById("colorPicker").value = color;
    document.getElementById("colorRGB").textContent =
      `RGB: (${data.r}, ${data.g}, ${data.b})`;
  })
  .catch((error) => console.error("Error fetching color:", error));
