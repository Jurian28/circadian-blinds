const express = require("express");
const { DatabaseSync } = require("node:sqlite");

const app = express();
app.use(express.json());

const db = new DatabaseSync(process.env.DB_FILE || "app.db");

// One row that always holds the current values.
db.exec(`
	CREATE TABLE IF NOT EXISTS data (
		id          INTEGER PRIMARY KEY CHECK (id = 1),
		motor       REAL,
		light       REAL,
		rain        REAL,
		blindStatus TEXT NOT NULL DEFAULT 'automatic',
		wakeupTime  TEXT
	);
	INSERT OR IGNORE INTO data (id) VALUES (1);
`);

const getData = db.prepare(
	"SELECT motor, light, rain, blindStatus, wakeupTime FROM data WHERE id = 1"
);

const getBlindStatus = db.prepare(
	"SELECT blindStatus, wakeupTime FROM data WHERE id = 1"
);

const getWakeupTime = db.prepare(
	"SELECT blindStatus, wakeupTime FROM data WHERE id = 1"
);
const setSensors = db.prepare("UPDATE data SET motor = ?, light = ?, rain = ? WHERE id = 1");
const setStatus = db.prepare("UPDATE data SET blindStatus = ? WHERE id = 1");
const setWakeupTime = db.prepare("UPDATE data SET wakeupTime = ? WHERE id = 1");

const isNumber = (v) => typeof v === "number" && Number.isFinite(v);
const isStatus = (v) => ["open", "closed", "automatic"].includes(v);
const isTime = (v) => typeof v === "string" && /^([01]\d|2[0-3]):[0-5]\d$/.test(v);

// Get all current data.
app.get("/api/data", (req, res) => {
	res.json(getData.get());
});

app.get("/api/wakeup-time", (req, res) => {
	res.type("text/plain").send(string(getWakeupTime.get()));
});

app.get("/api/blind-status", (req, res) => {
	res.type("text/plain").send(string(getBlindStatus.get()));
});

// Set the sensor values, e.g. {"Motor": 0, "Light": 512, "Rain": 0}.
app.post("/api/sensors", (req, res) => {
	const { motor, light, rain } = req.body || {};
	if (![motor, light, rain].every(isNumber)) {
		return res.status(400).json({ error: "motor, light and rain must all be numbers." });
	}

	setSensors.run(motor, light, rain);
	res.json(getData.get());
});

// Set the blind status, e.g. {"BlindStatus": "open"}.
app.post("/api/status", (req, res) => {
	const { blindStatus } = req.body || {};
	if (!isStatus(blindStatus)) {
		return res.status(400).json({ error: "blindStatus must be one of: open, closed, automatic." });
	}

	setStatus.run(blindStatus);
	res.json(getData.get());
});

// Set the wake-up time, e.g. {"WakeupTime": "07:00"}.
app.post("/api/wakeup-time", (req, res) => {
	const { wakeupTime } = req.body || {};
	if (!isTime(wakeupTime)) {
		return res.status(400).json({ error: 'wakeupTime must be in HH:MM format, e.g. "07:00".' });
	}

	setWakeupTime.run(wakeupTime);
	res.json(getData.get());
});

app.listen(Number(process.env.PORT) || 3000);
