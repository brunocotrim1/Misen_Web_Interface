const imageInpt = document.querySelector("#image_input");
const imgPreview = document.querySelector("#image_toSearch");
var base64Image = "none";
document.querySelector("#add_but").onclick = function () {
  imageInpt.click();
};

imageInpt.addEventListener("change", function () {
  const file = this.files[0];
  if (file) {
    var reader = new FileReader();
    imgPreview.style.display = "block";
    reader.addEventListener("load", function () {
      imgPreview.setAttribute("src", this.result);
      base64Image = this.result;
    });
    reader.readAsDataURL(file);

    processImgClick();
  }
});

function processImgClick() {
  document.querySelector("#image_search_container").style.display = "block";
  var searchBar = document.querySelector(".seach_box");
  if (searchBar.value) {
    document.getElementById("resultsFor").innerHTML = searchBar.value;
  }
}

function closeProcessing() {
  document.getElementById("image_search_container").style.display = "none";
  document.getElementById("resultsFor").innerHTML = "";
  imgPreview.setAttribute("src", "");
}

async function processSearch() {
  var toSearch =
    "/api/search?query=" + document.querySelector(".seach_box").value;
  console.log(toSearch)
  var i = await fetch(toSearch, {
    method: "POST",
    body: base64Image,
  }).then((response) => {
    if (response.ok) {
      console.log(response);
      return response.text();
    } else return "No Results Found";
  });

  document.querySelector("#search_results").innerHTML = i;
}
