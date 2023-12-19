const express = require("express");
const bodyParser = require("body-parser");
const app = express();

app.use(bodyParser.urlencoded({ extended: true }));

app.use(express.json());

const carDefaultState = {
  forward: false,
  backward: false,
  left: false,
  right: false,
  stop: false,
};

let carState = {
  forward: false,
  backward: false,
  left: false,
  right: false,
  stop: false,
};

let count = 1;

app.post("/api/v1/sensor/", (req, res) => {
  const temp = req.body;
  console.log(temp);
  res.send("Temperature received");
});

app.post("/api/v1/car/", (req, res) => {
  const { CAR } = req.body;
  console.log(CAR, "car");
  // carState = carDefaultState;
  Object.keys(carState).forEach((key) => {
    carState[key] = false;
  });
  carState[CAR] = true;
  res.redirect("/car");
});

app.get("/api/v1/carcontrol/", (req, res) => {
  console.log("count ", count++);
  res.send(JSON.stringify(carState));
});

app.get("/car", (req, res) => {
  res.sendFile(__dirname + "/car.html");
});

app.get("/", (req, res) => {
  res.send("Hello World!");
});
app.listen(3000, () => console.log("Server running on port 3000"));
