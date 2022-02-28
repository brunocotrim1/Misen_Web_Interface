const imageInpt = document.querySelector("#image_input");
const imgPreview = document.querySelector("#image_toSearch");
const update_input = document.querySelector("#update_input");
var base64Image = "none";
document.querySelector("#add_but").onclick = function () {
  imageInpt.click();
};

document.querySelector("#addUpdate").onclick = function () {
  update_input.click();
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

update_input.addEventListener("change", async () => {
  if (confirm("De certeza que quer adicionar este ficheiro ?")) {
    var file = update_input.files[0];
    var toSearch =
      "/api/update?file_type=" +
      file.name.split(".").pop() +
      "&name=" +
      file.name.split(".")[0];
    console.log(console.log(toSearch));

    var query = fetch(toSearch, {
      method: "POST",
      body: file,
    }).then((response) => {
      alert(response.text);
    });
  } else {
    update_input.setAttribute("src", "");
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
    "/api/search?query=" +
    document.querySelector(".seach_box").value;
  console.log(toSearch);
  var i = await fetch(toSearch, {
    method: "POST",
    body: base64Image,
  }).then((response) => {
    if (response.ok) {
      console.log(response);
      return response.text();
    } else return "No Results Found";
  });
  base64Image = "none";
  document.querySelector("#search_results").innerHTML = i;
}

function showUpdate() {
  var button = document.querySelector("#update_show");
  var update_div = document.querySelector("#misen_update");
  if (button.textContent == "+") {
    update_div.style.height = "150px";
    button.innerHTML = "-";
  } else {
    button.innerHTML = "+";
    update_div.style.height = "50px";
  }
}
