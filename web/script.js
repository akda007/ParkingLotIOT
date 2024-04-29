const light1 = document.querySelector(".light-1");
const light2 = document.querySelector(".light-2");
const light3 = document.querySelector(".light-3");

const updateInfo = (data) => {
    let { s1, s2, s3 } = data;


    if (s1 === 1) {
        light1.classList.remove("light-red");
        light1.innerText = "FREE";
    } else {
        light1.classList.add("light-red");
        light1.innerText = "PARKED";
    }

    if (s2 === 1) {
        light2.classList.remove("light-red");
        light2.innerText = "FREE";
    } else {
        light2.classList.add("light-red");
        light2.innerText = "PARKED";
    }

    if (s3 === 1) {
        light3.classList.remove("light-red");
        light3.innerText = "FREE";
    } else {
        light3.classList.add("light-red");
        light3.innerText = "PARKED";
    }
}

const fetchData = () => {
    fetch("http://192.168.137.222/data", {
        method: "GET",

    }).then(res => {
        return res.json();
    }).then(data => {
        updateInfo(data);
    }).catch(err => console.log(err.message));
}

document.addEventListener("DOMContentLoaded", () => {
    setInterval(() => {
        fetchData();
    }, 300);
})