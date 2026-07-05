const state = {
  intakeTemp: 42,
  engineRpm: 2800,
  throttlePos: 31,
  pumpManual: false,
  pumpAuto: false,
};

const els = {
  intakeTemp: document.querySelector("#intakeTemp"),
  engineRpm: document.querySelector("#engineRpm"),
  throttlePos: document.querySelector("#throttlePos"),
  tempBar: document.querySelector("#tempBar"),
  rpmBar: document.querySelector("#rpmBar"),
  throttleBar: document.querySelector("#throttleBar"),
  autoMode: document.querySelector("#autoMode"),
  tempOn: document.querySelector("#tempOn"),
  tempOff: document.querySelector("#tempOff"),
  rpmOn: document.querySelector("#rpmOn"),
  rpmOff: document.querySelector("#rpmOff"),
  throttleOn: document.querySelector("#throttleOn"),
  throttleOff: document.querySelector("#throttleOff"),
  tempOnOut: document.querySelector("#tempOnOut"),
  tempOffOut: document.querySelector("#tempOffOut"),
  rpmOnOut: document.querySelector("#rpmOnOut"),
  rpmOffOut: document.querySelector("#rpmOffOut"),
  throttleOnOut: document.querySelector("#throttleOnOut"),
  throttleOffOut: document.querySelector("#throttleOffOut"),
  manualSpray: document.querySelector("#manualSpray"),
  pumpState: document.querySelector("#pumpState"),
  systemState: document.querySelector("#systemState span:last-child"),
};

const thresholdInputs = [
  els.autoMode,
  els.tempOn,
  els.tempOff,
  els.rpmOn,
  els.rpmOff,
  els.throttleOn,
  els.throttleOff,
];

const clampPercent = (value, min, max) => {
  const percent = ((value - min) / (max - min)) * 100;
  return Math.max(0, Math.min(100, percent));
};

const numberValue = (input) => Number(input.value);

function sendPumpCommand(enabled, source) {
  // Replace with fetch("/api/pump", { method: "POST", body: JSON.stringify({ enabled, source }) })
  // when the ESP32 endpoint is ready.
  console.log("pump", enabled ? "on" : "off", source);
}

function sendSettings() {
  const payload = {
    autoMode: els.autoMode.checked,
    tempOn: numberValue(els.tempOn),
    tempOff: numberValue(els.tempOff),
    rpmOn: numberValue(els.rpmOn),
    rpmOff: numberValue(els.rpmOff),
    throttleOn: numberValue(els.throttleOn),
    throttleOff: numberValue(els.throttleOff),
  };

  // Replace with fetch("/api/settings", { method: "POST", body: JSON.stringify(payload) }).
  console.log("settings", payload);
}

function keepOffBelowOn(offInput, onInput) {
  if (numberValue(offInput) > numberValue(onInput)) {
    offInput.value = onInput.value;
  }
}

function normalizeThresholds() {
  keepOffBelowOn(els.tempOff, els.tempOn);
  keepOffBelowOn(els.rpmOff, els.rpmOn);
  keepOffBelowOn(els.throttleOff, els.throttleOn);
}

function renderThresholds() {
  els.tempOnOut.textContent = `${els.tempOn.value} \u00b0C`;
  els.tempOffOut.textContent = `${els.tempOff.value} \u00b0C`;
  els.rpmOnOut.textContent = `${els.rpmOn.value} RPM`;
  els.rpmOffOut.textContent = `${els.rpmOff.value} RPM`;
  els.throttleOnOut.textContent = `${els.throttleOn.value} %`;
  els.throttleOffOut.textContent = `${els.throttleOff.value} %`;
}

function evaluateAutoSpray() {
  if (!els.autoMode.checked) {
    state.pumpAuto = false;
    return;
  }

  const shouldTurnOn =
    state.intakeTemp >= numberValue(els.tempOn) &&
    state.engineRpm >= numberValue(els.rpmOn) &&
    state.throttlePos >= numberValue(els.throttleOn);

  const shouldTurnOff =
    state.intakeTemp <= numberValue(els.tempOff) ||
    state.engineRpm <= numberValue(els.rpmOff) ||
    state.throttlePos <= numberValue(els.throttleOff);

  if (!state.pumpAuto && shouldTurnOn) {
    state.pumpAuto = true;
    sendPumpCommand(true, "auto");
  }

  if (state.pumpAuto && shouldTurnOff) {
    state.pumpAuto = false;
    if (!state.pumpManual) {
      sendPumpCommand(false, "auto");
    }
  }
}

function renderSensors() {
  els.intakeTemp.textContent = Math.round(state.intakeTemp);
  els.engineRpm.textContent = Math.round(state.engineRpm);
  els.throttlePos.textContent = Math.round(state.throttlePos);

  els.tempBar.style.width = `${clampPercent(state.intakeTemp, 0, 110)}%`;
  els.rpmBar.style.width = `${clampPercent(state.engineRpm, 700, 8200)}%`;
  els.throttleBar.style.width = `${clampPercent(state.throttlePos, 0, 100)}%`;
}

function renderPump() {
  const enabled = state.pumpManual || state.pumpAuto;
  els.manualSpray.classList.toggle("is-active", state.pumpManual);
  els.manualSpray.setAttribute("aria-pressed", String(state.pumpManual));
  els.pumpState.textContent = enabled ? "Включен" : "Выключен";
  els.pumpState.classList.toggle("is-on", enabled);
  els.systemState.textContent = state.pumpManual ? "Ручной режим" : state.pumpAuto ? "Авто орошение" : "Готов";
}

function renderAll() {
  normalizeThresholds();
  renderThresholds();
  evaluateAutoSpray();
  renderSensors();
  renderPump();
}

function startManualSpray(event) {
  event.preventDefault();
  if (state.pumpManual) return;
  state.pumpManual = true;
  sendPumpCommand(true, "manual");
  renderPump();
}

function stopManualSpray() {
  if (!state.pumpManual) return;
  state.pumpManual = false;
  sendPumpCommand(state.pumpAuto, state.pumpAuto ? "auto" : "manual");
  renderPump();
}

function simulateSensors() {
  const now = Date.now() / 1000;
  state.intakeTemp = 47 + Math.sin(now * 0.72) * 12 + Math.sin(now * 0.19) * 6;
  state.engineRpm = 2950 + Math.sin(now * 0.58) * 1450 + Math.sin(now * 1.31) * 220;
  state.throttlePos = 38 + Math.sin(now * 0.82) * 28 + Math.sin(now * 0.23) * 11;
  renderAll();
}

["input", "change"].forEach((eventName) => {
  thresholdInputs.forEach((input) => {
    input.addEventListener(eventName, () => {
      renderAll();
      sendSettings();
    });
  });
});

els.manualSpray.addEventListener("pointerdown", startManualSpray);
els.manualSpray.addEventListener("pointerup", stopManualSpray);
els.manualSpray.addEventListener("pointercancel", stopManualSpray);
els.manualSpray.addEventListener("pointerleave", stopManualSpray);
els.manualSpray.addEventListener("mousedown", startManualSpray);
els.manualSpray.addEventListener("mouseup", stopManualSpray);
els.manualSpray.addEventListener("mouseleave", stopManualSpray);
els.manualSpray.addEventListener("touchstart", startManualSpray, { passive: false });
els.manualSpray.addEventListener("touchend", stopManualSpray);
els.manualSpray.addEventListener("touchcancel", stopManualSpray);
window.addEventListener("blur", stopManualSpray);

renderAll();
setInterval(simulateSensors, 900);
