let currentStatus = "Lost";

let records = JSON.parse(
    localStorage.getItem("campusItems") || "[]"
);

function saveData() {
    localStorage.setItem(
        "campusItems",
        JSON.stringify(records)
    );
}

function showForm(status) {

    currentStatus = status;

    document.getElementById("formTitle").innerText =
        "Report a " + status + " Item";

    document.getElementById("report")
        .classList.remove("hidden");

    document.getElementById("report")
        .scrollIntoView({ behavior: "smooth" });
}

function hideForm() {
    document.getElementById("report")
        .classList.add("hidden");
}

document.getElementById("itemForm")
.addEventListener("submit", function(event) {

    event.preventDefault();

    const file = document.getElementById("image").files[0];

    const newRecord = {
        id: Date.now(),
        name: document.getElementById("name").value.trim(),
        category: document.getElementById("category").value,
        date: document.getElementById("date").value,
        contact: document.getElementById("contact").value.trim(),
        status: currentStatus,
        image: ""
    };

    if (!newRecord.name || !newRecord.contact) {
        alert("Please enter all required details.");
        return;
    }

    function finishReport(imageData) {

        newRecord.image = imageData || "";

        records.unshift(newRecord);

        try {
            saveData();
        } catch (error) {
            records.shift();
            alert("Storage is full. Try a smaller image.");
            return;
        }

        alert("Item successfully reported!");

        document.getElementById("itemForm").reset();

        hideForm();
        displayItems();
    }

    if (file) {

        if (!file.type.startsWith("image/")) {
            alert("Please select a valid image.");
            return;
        }

        if (file.size > 1024 * 1024) {
            alert("Image must be smaller than 1 MB.");
            return;
        }

        const reader = new FileReader();

        reader.onload = function() {
            finishReport(reader.result);
        };

        reader.onerror = function() {
            alert("Unable to read image.");
        };

        reader.readAsDataURL(file);

    } else {
        finishReport("");
    }
});

function displayItems() {

    const list = document.getElementById("itemList");

    const keyword = document.getElementById("search")
        .value.toLowerCase().trim();

    list.innerHTML = "";

    const filtered = records.filter(item =>
        item.name.toLowerCase().includes(keyword) ||
        item.category.toLowerCase().includes(keyword)
    );

    if (filtered.length === 0) {
        list.innerHTML =
            '<div class="empty">No matching items found.</div>';
    }

    filtered.forEach(item => {

        const card = document.createElement("div");
        card.className = "item-card";

        const image = document.createElement("img");
        image.alt = item.name;

        image.src = item.image ||
            "https://placehold.co/400x250?text=No+Image";

        const info = document.createElement("div");
        info.className = "item-info";

        const badge = document.createElement("span");
        badge.className = "status " +
            (item.status === "Lost" ? "lost" : "found");
        badge.textContent = item.status;

        const title = document.createElement("h3");
        title.textContent = item.name;

        const category = document.createElement("p");
        category.textContent = "Category: " + item.category;

        const date = document.createElement("p");
        date.textContent = "Date: " + item.date;

        const contact = document.createElement("p");
        contact.textContent = "Contact: " + item.contact;

        info.append(badge, title, category, date, contact);

        card.append(image, info);

        list.appendChild(card);
    });

    updateStats();
}

function updateStats() {

    document.getElementById("totalCount").innerText =
        records.length;

    document.getElementById("lostCount").innerText =
        records.filter(item =>
            item.status === "Lost"
        ).length;

    document.getElementById("foundCount").innerText =
        records.filter(item =>
            item.status === "Found"
        ).length;
}

document.getElementById("search")
.addEventListener("input", displayItems);

displayItems();