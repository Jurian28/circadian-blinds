const ApiUrl = "https://circadian-blinds.onrender.com";

const message = document.getElementById("message");

async function sendRequest(url, options) {
	const response = await fetch(ApiUrl + url, options);
	const result = await response.json();

	if (!response.ok || !result.success) {
		throw new Error(result.error || "Request failed");
	}

	return result;
}

async function loadData() {
	try {
		const result = await sendRequest("/api/data");
		const data = result.data;

		document.getElementById("motion").textContent = data.motion ?? "—";
		document.getElementById("light").textContent = data.light ?? "—";
		document.getElementById("rain").textContent = data.rain ?? "—";

		document.getElementById("lastUpdate").textContent = data.updatedAt
			? "Last updated: " + new Date(data.updatedAt).toLocaleString()
			: "No update time available";

		if (data.wakeupTime) {
			document.getElementById("wakeTime").value = data.wakeupTime;
		}

		if (data.blindStatus) {
			document.getElementById("status").value = data.blindStatus;
		}
	} catch (error) {
		document.getElementById("lastUpdate").textContent =
			"Could not load data: " + error.message;
	}
}

document.getElementById("refresh").addEventListener("click", loadData);

document.getElementById("saveTime").addEventListener("click", async () => {
	try {
		const wakeupTime = document.getElementById("wakeTime").value;

		await sendRequest("/api/wakeup-time", {
			method: "POST",
			headers: { "Content-Type": "application/json" },
			body: JSON.stringify({ wakeupTime })
		});

		await loadData();
		message.textContent = "Wake-up time saved.";
	} catch (error) {
		message.textContent = "Could not save time: " + error.message;
	}
});

document.getElementById("saveStatus").addEventListener("click", async () => {
	try {
		const status = document.getElementById("status").value;

		await sendRequest("/api/status", {
			method: "POST",
			headers: { "Content-Type": "application/json" },
			body: JSON.stringify({ status })
		});

		await loadData();
		message.textContent = "Blind status saved.";
	} catch (error) {
		message.textContent = "Could not save status: " + error.message;
	}
});

loadData();
setInterval(loadData, 30000);
