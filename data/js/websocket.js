var socket = new WebSocket("ws://" + window.location.hostname + "/ws");
var speed = 100; // デフォルトの速度

// ドロップダウンメニューのオプションを設定
function populateSpeedOptions() {
  var dropdown = document.getElementById("speedDropdown");
  for (var i = 100; i <= 500; i += 100) {
    var option = document.createElement("option");
    option.value = i;
    option.text = i;
    dropdown.add(option);
  }
}

socket.onopen = function () {
  console.log("WebSocket connected");
  populateSpeedOptions(); // ドロップダウンメニューのオプションを設定
};

socket.onerror = function (error) {
  console.error("WebSocket error:", error);
};

socket.onmessage = function (event) {
  console.log("Message from ESP32:", event.data);
  var message = JSON.parse(event.data);
  if (message.type === "initial") {
    var r = message.data.r;
    var g = message.data.g;
    var b = message.data.b;
    speed = message.data.speed;
    var redCorrection = message.data.redCorrection;
    var greenCorrection = message.data.greenCorrection;
    var blueCorrection = message.data.blueCorrection;

    document.getElementById("colorPicker").value = rgbToHex(r, g, b);
    document.getElementById("colorRGB").textContent = `RGB: (${r}, ${g}, ${b})`;
    document.getElementById("speedDropdown").value = speed;
    document.getElementById("speedValue").textContent = `速度: ${speed}`;
    document.getElementById("redCorrection").value = redCorrection;
    document.getElementById("greenCorrection").value = greenCorrection;
    document.getElementById("blueCorrection").value = blueCorrection;
  }
};

function sendColor() {
  var color = document.getElementById("colorPicker").value;
  var r = parseInt(color.substring(1, 3), 16);
  var g = parseInt(color.substring(3, 5), 16);
  var b = parseInt(color.substring(5, 7), 16);
  document.getElementById("colorRGB").textContent = `RGB: (${r}, ${g}, ${b})`;

  // WebSocketを介してESP32にRGB値と速度を送信する
  var message = {
    type: "color",
    data: { r: r, g: g, b: b, speed: speed },
  };
  socket.send(JSON.stringify(message));
}

function updateSpeed() {
  speed = document.getElementById("speedDropdown").value;
  document.getElementById("speedValue").textContent = `速度: ${speed}`;
  sendColor(); // 色と共に速度を送信するために呼び出す
}

function sendBrightnessCorrection() {
  var redCorrection = parseFloat(
    document.getElementById("redCorrection").value,
  );
  var greenCorrection = parseFloat(
    document.getElementById("greenCorrection").value,
  );
  var blueCorrection = parseFloat(
    document.getElementById("blueCorrection").value,
  );

  var message = {
    type: "brightnessCorrection",
    data: {
      redCorrection: redCorrection,
      greenCorrection: greenCorrection,
      blueCorrection: blueCorrection,
    },
  };
  socket.send(JSON.stringify(message));
  sendColor(); // 補正値を送信した後にLEDの色を再設定
}

function rgbToHex(r, g, b) {
  return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
}

function componentToHex(c) {
  var hex = c.toString(16);
  return hex.length === 1 ? "0" + hex : hex;
}
