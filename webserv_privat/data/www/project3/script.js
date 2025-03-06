// Wait until the DOM is fully loaded
document.addEventListener("DOMContentLoaded", function () {
  // Log to verify script is loaded
  console.log("JavaScript is loaded and working!");
  
  // Get the button element
  const button = document.getElementById("colorButton");
  button.addEventListener("click", function () {
  // Log when the button is clicked
    console.log("Button clicked!");
    document.body.style.backgroundColor = getRandomColor();
  });

  function getRandomColor() {
    const letters = "0123456789ABCDEF";
    let color = "#";
    for (let i = 0; i < 6; i++) {
      color += letters[Math.floor(Math.random() * 16)];
    }
    return color;
  }
});