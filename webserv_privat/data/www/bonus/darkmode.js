// Function to get a cookie value by name
function getCookie(name) {
    let cookies = document.cookie.split('; ');
    for (let i = 0; i < cookies.length; i++) {
        let cookiePair = cookies[i].split('=');
        if (cookiePair[0] === name) {
            return cookiePair[1];
        }
    }
    return null;
}

// Function to set a cookie
function setCookie(name, value, days) {
    let date = new Date();
    date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
    document.cookie = name + "=" + value + "; expires=" + date.toUTCString() + "; path=/";
}

// Function to toggle dark mode
function toggleDarkMode() {
    let darkMode = getCookie("darkmode");

    if (darkMode === "true") {
        document.body.classList.remove("dark");
        setCookie("darkmode", "false", 365);
    } else {
        document.body.classList.add("dark");
        setCookie("darkmode", "true", 365);
    }
}

// Check cookie on page load
window.onload = function () {
    if (getCookie("darkmode") === "true") {
        document.body.classList.add("dark");
    }
};

// Attach event to button
document.addEventListener("DOMContentLoaded", function () {
    let darkModeButton = document.getElementById("darkModeToggle");
    if (darkModeButton) {
        darkModeButton.addEventListener("click", toggleDarkMode);
    }
});
