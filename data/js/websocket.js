var socket = new WebSocket("ws://" + window.location.hostname + "/ws");

socket.onopen = function () {
  console.log("WebSocket connected");
};

socket.onerror = function (error) {
  console.error("WebSocket error:", error);
};

socket.onmessage = function (event) {
  console.log("Message from ESP32:", event.data);
};

function sendColor() {
  var color = document.getElementById("colorPicker").value;
  var r = parseInt(color.substring(1, 3), 16);
  var g = parseInt(color.substring(3, 5), 16);
  var b = parseInt(color.substring(5, 7), 16);
  document.getElementById("colorRGB").textContent = `RGB: (${r}, ${g}, ${b})`;

  // WebSocketを介してESP32にRGB値を送信する
  var message = {
    type: "color",
    data: { r: r, g: g, b: b },
  };
  socket.send(JSON.stringify(message));
}
