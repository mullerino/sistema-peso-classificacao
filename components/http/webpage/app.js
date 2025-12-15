async function carregarRegistros() {
  const res = await fetch("/registros");
  const dados = await res.json();

  if (!dados || dados.length === 0) return;

  const atual = dados[dados.length - 1];

  const section = document.getElementById("current-section");
  const card = document.getElementById("current-card");
  const statusEl = document.getElementById("current-status");

  document.getElementById("current-peso").innerText =
    `${atual.peso.toFixed(2)} g`;

  document.getElementById("current-time").innerText = atual.timestamp;

  statusEl.innerText = atual.status;

  section.classList.remove("aprovado", "reprovado");
  statusEl.classList.remove("aprovado", "reprovado");

  if (atual.status === "APROVADO") {
    section.classList.add("aprovado");
    statusEl.classList.add("aprovado");
  } else {
    section.classList.add("reprovado");
    statusEl.classList.add("reprovado");
  }

  const tbody = document.getElementById("tabela-body");
  tbody.innerHTML = "";

  dados
    .slice()
    .reverse()
    .forEach((item, index) => {
      const tr = document.createElement("tr");

      if (index === 0) tr.classList.add("highlight");

      tr.innerHTML = `
        <td>${item.timestamp}</td>
        <td>${item.peso.toFixed(2)}</td>
        <td>
          <span class="status-badge ${
            item.status === "APROVADO" ? "aprovado" : "reprovado"
          }">
            ${item.status}
          </span>
        </td>
      `;

      tbody.appendChild(tr);
    });
}

document.addEventListener("DOMContentLoaded", () => {
  carregarRegistros();
  setInterval(carregarRegistros, 3000);
});
