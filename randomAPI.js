// json = js object notation
let userdata = 0;
const image = document.querySelector("img");
const buttonclick = document.querySelector('button');

buttonclick.addEventListener("click", () => {
  fetch("https://dog.ceo/api/breeds/image/random")
    .then((response) => response.json())
    .then((json) => {
      console.log("Fetched ! ");
      console.log(json.message);
      image.src = json.message;
      // userdata = json
    });
});
