
.pragma library

var navigationHistory = [];
var MAX_HISTORY_SIZE = 100;

function recordNavigation(page, details) {
    var timestamp = new Date().toISOString();
    var entry = { page: page, details: details, timestamp: timestamp };
    navigationHistory.unshift(entry); // Add to the beginning

    if (navigationHistory.length > MAX_HISTORY_SIZE) {
        navigationHistory.pop(); // Remove the oldest entry if history exceeds size
    }
    console.log("Navigation recorded: " + JSON.stringify(entry));
}

function getNavigationHistory() {
    return navigationHistory;
}

function clearNavigationHistory() {
    navigationHistory = [];
    console.log("Navigation history cleared.");
}

function exportHistoryToJson() {
    return JSON.stringify(navigationHistory, null, 2);
}
