#pragma once

#include <Arduino.h>

const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="id">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>SPPG LOOP | Dashboard Admin</title>

<style>
:root {
  --cream:#FFF7DF;
  --forest:#315D18;
  --dark:#23450F;
  --sage:#80A45C;
  --light:#DCE9B9;
  --teal:#164F59;
  --text:#263B32;
  --muted:#65735D;
  --white:#FFFFFF;
}
*{box-sizing:border-box;margin:0;padding:0}
body{
  font-family:Arial,sans-serif;background:var(--cream);color:var(--text);
  padding:25px;position:relative;overflow-x:hidden
}
.container{max-width:1250px;margin:auto;position:relative;z-index:1}
body:before,body:after{
  content:"❋";position:fixed;font-size:180px;color:#80A45C18;
  z-index:0;pointer-events:none
}
body:before{top:35%;left:-75px}
body:after{bottom:5%;right:-65px}
.header{
  background:linear-gradient(120deg,#315D18,#456F29);color:white;
  padding:32px;border-radius:17px;border-bottom:5px solid var(--sage);
  margin-bottom:23px;position:relative;overflow:hidden
}
.header:after{
  content:"∞";position:absolute;right:35px;top:-28px;font-size:150px;
  font-weight:bold;color:#FFFFFF18;pointer-events:none
}
.brand{font-size:39px;font-weight:bold;letter-spacing:1px}
.tagline{font-size:15px;color:#E4EDCE;margin-top:8px;letter-spacing:.5px}
.header-bottom{
  display:flex;justify-content:space-between;align-items:center;
  margin-top:25px;flex-wrap:wrap;gap:12px
}
.admin-label{font-size:15px;font-weight:bold}
.admin-badge{
  background:#FFFFFF25;border:1px solid #FFFFFF30;border-radius:30px;
  padding:10px 20px;font-size:13px;font-weight:bold
}
.grid{display:grid;grid-template-columns:repeat(2,1fr);gap:20px}
.card{
  background:white;border:1px solid #E9E6D6;padding:25px;border-radius:15px;
  box-shadow:0 4px 14px #315D1810
}
.card h3{color:var(--forest);font-size:18px;margin-bottom:15px}
.section-subtitle{font-size:15px;line-height:1.8;color:var(--muted);margin-bottom:18px}
.summary-card{display:flex;justify-content:space-between;align-items:center;min-height:125px}
.summary-text{color:var(--muted);font-size:15px;line-height:1.7}
.summary-icon{
  width:49px;height:49px;background:var(--light);color:var(--forest);
  border-radius:50%;display:flex;justify-content:center;align-items:center;
  font-size:24px;flex-shrink:0
}
.status-row{display:flex;align-items:center;gap:9px}
.dot{width:10px;height:10px;background:#D39B32;border-radius:50%;display:inline-block}
.status{color:#99701B;font-size:14px;font-weight:bold}
.weight-card{
  grid-column:span 2;background:linear-gradient(115deg,#FFF,#F0F5DF);
  border-left:6px solid var(--sage);padding:29px
}
.weight-label{font-size:17px;font-weight:bold;color:var(--forest)}
.active-category{
  display:inline-block;background:var(--light);color:var(--forest);
  padding:11px 16px;border-radius:9px;font-size:14px;font-weight:bold;margin-top:20px
}
.current-label{color:var(--muted);font-size:14px;margin-top:20px;font-weight:bold}
.weight{font-size:65px;color:var(--teal);font-weight:bold;margin-top:17px}
.unit{font-size:24px}
.weight-desc{color:var(--muted);font-size:15px;margin-top:7px;line-height:1.7}
.measurement-grid{display:grid;grid-template-columns:repeat(2,1fr);gap:13px;margin-top:25px}
.measurement-item{background:#F5F7EA;border-radius:12px;padding:18px}
.measurement-item strong{display:block;font-size:13px;color:var(--forest);margin-bottom:10px}
.measurement-item div{color:var(--teal);font-size:25px;font-weight:bold}
.total-box{
  display:flex;justify-content:space-between;align-items:center;background:var(--forest);
  color:white;padding:18px;border-radius:12px;margin-top:15px;gap:10px
}
.total-box strong{font-size:24px}
.measurement-buttons{display:flex;flex-wrap:wrap;gap:12px;margin-top:20px}
.save-button,.reset-button{
  padding:13px 20px;border:none;border-radius:9px;font-weight:bold;cursor:pointer
}
.save-button{background:var(--forest);color:white}
.reset-button{background:#E9E6D6;color:var(--text)}
.save-button:hover{background:var(--teal)}
.reset-button:hover{background:#D8D5C7}
.history-card{grid-column:span 2}
.table-wrap{overflow-x:auto}
table{width:100%;border-collapse:collapse;min-width:650px}
th{background:var(--forest);color:white;padding:13px 10px;font-size:13px}
td{padding:13px 10px;text-align:center;border-bottom:1px solid #E9E6D6;font-size:14px}
tr:nth-child(even) td{background:#F8FAF1}
.history-status{font-size:13px;color:var(--muted);margin-top:14px}
.analysis-card{grid-column:span 2;background:linear-gradient(120deg,#FFF,#F1F6E6);border-left:6px solid var(--teal)}
.ai-head{display:flex;justify-content:space-between;align-items:flex-start;gap:18px;flex-wrap:wrap}
.ai-button{
  display:inline-flex;align-items:center;gap:10px;padding:14px 26px;border:none;border-radius:11px;
  background:var(--teal);color:white;font-size:14px;font-weight:bold;letter-spacing:.5px;
  cursor:pointer;box-shadow:0 4px 0 #0E3640;transition:.2s;white-space:nowrap
}
.ai-button:hover:not(:disabled){background:#1D6C79;transform:translateY(-2px)}
.ai-button:disabled{background:#7E9AA0;box-shadow:0 4px 0 #648086;cursor:progress}
.ai-status{font-size:13px;color:var(--muted);margin-top:16px;min-height:18px}
.ai-result{margin-top:20px}
.ai-score{
  display:flex;align-items:center;gap:18px;background:white;border:1px solid #E9E6D6;
  border-radius:12px;padding:18px;flex-wrap:wrap
}
.ai-score-badge{
  width:78px;height:78px;border-radius:50%;background:var(--forest);color:white;
  display:flex;align-items:center;justify-content:center;font-size:26px;font-weight:bold;flex-shrink:0
}
.ai-score-info{flex:1;min-width:200px}
.ai-score-info strong{display:block;font-size:13px;color:var(--forest);margin-bottom:10px;letter-spacing:.5px}
.ai-score-info p{font-size:13px;color:var(--muted);line-height:1.6;margin-top:9px}
.ai-score-bar{height:10px;background:#E9E6D6;border-radius:20px;overflow:hidden}
.ai-score-bar span{display:block;height:100%;border-radius:20px;transition:width .5s}
.ai-conclusion{background:white;border-left:4px solid var(--sage);border-radius:10px;padding:16px;margin-top:15px}
.ai-conclusion strong{display:block;font-size:13px;color:var(--forest);margin-bottom:8px;letter-spacing:.5px}
.ai-conclusion p{font-size:14px;line-height:1.8;color:var(--text)}
.ai-grid{display:grid;grid-template-columns:repeat(2,1fr);gap:14px;margin-top:15px}
.ai-box{background:#F5F7EA;border-radius:12px;padding:18px;border-top:4px solid var(--sage)}
.ai-box.no{background:#FDF6E7;border-top-color:#D39B32}
.ai-box h4{font-size:13px;color:var(--forest);margin-bottom:12px;letter-spacing:.5px}
.ai-box.no h4{color:#8A6414}
.ai-item{padding:10px 0;border-bottom:1px dashed #DDE3CF}
.ai-item:last-child{border-bottom:none;padding-bottom:0}
.ai-item strong{display:block;font-size:14px;color:var(--text);margin-bottom:5px;line-height:1.5}
.ai-item p{font-size:13px;color:var(--muted);line-height:1.7}
.ai-item .ai-action{margin-top:6px;color:var(--forest);font-weight:bold}
.ai-box.no .ai-action{color:#8A6414}
.ai-item-empty{font-size:13px;color:var(--muted)}
.ai-reco{background:var(--light);border-radius:12px;padding:18px;margin-top:15px}
.ai-reco strong{display:block;font-size:13px;color:var(--forest);letter-spacing:.5px}
.ai-reco ul{margin:10px 0 0 18px}
.ai-reco li{font-size:13px;line-height:1.8;color:var(--text);margin-bottom:7px}
.ai-meta{font-size:12px;color:var(--muted);margin-top:14px}
.ai-raw{
  white-space:pre-wrap;background:white;border:1px solid #E9E6D6;border-radius:10px;
  padding:16px;font-size:13px;line-height:1.8;color:var(--text);font-family:inherit
}
.refresh-button{
  padding:10px 15px;border:0;border-radius:8px;background:var(--light);
  color:var(--forest);font-weight:bold;cursor:pointer;margin-bottom:14px
}
.device-grid{display:grid;grid-template-columns:repeat(2,1fr);gap:13px}
.device-item{
  background:#F5F7EA;padding:19px;border-radius:12px;border-bottom:4px solid var(--sage)
}
.device-item strong{display:block;color:var(--forest);font-size:16px;margin-bottom:8px}
.device-item p{color:var(--muted);font-size:14px;line-height:1.6}
.purpose-card{background:var(--light);border:none}
.purpose-card p{font-size:15px;line-height:1.9}
.purpose-list{margin-top:17px;display:grid;gap:12px}
.purpose-item{display:flex;align-items:center;gap:12px;font-size:15px;font-weight:bold;color:var(--forest)}
.purpose-icon{
  width:34px;height:34px;border-radius:50%;background:#FFFFFFA0;
  display:flex;justify-content:center;align-items:center;flex-shrink:0
}
.workflow-card{grid-column:span 2;background:#F1F5E5;border-left:6px solid var(--sage)}
.workflow-grid{display:grid;grid-template-columns:repeat(5,1fr);gap:13px}
.workflow-item{background:white;padding:18px;border-radius:12px;border-bottom:4px solid var(--sage)}
.number{
  width:34px;height:34px;border-radius:50%;background:var(--forest);color:white;
  display:flex;justify-content:center;align-items:center;font-weight:bold;margin-bottom:13px
}
.workflow-item strong{font-size:14px;color:var(--forest)}
.workflow-item p{font-size:14px;color:var(--muted);line-height:1.7;margin-top:9px}
.impact-card{grid-column:span 2}
.impact-grid{display:grid;grid-template-columns:repeat(3,1fr);gap:14px}
.impact-item{background:#F5F7EA;padding:21px;border-radius:12px;text-align:center}
.impact-icon{font-size:30px;margin-bottom:12px}
.impact-item strong{display:block;color:var(--forest);font-size:16px;margin-bottom:9px}
.impact-item p{font-size:14px;color:var(--muted);line-height:1.7}
.footer{
  background:var(--forest);color:white;text-align:center;margin-top:24px;
  padding:28px;border-radius:15px;line-height:1.9;font-size:14px
}
.footer-title{font-size:20px;font-weight:bold}
.logout-button{
  display:inline-flex;align-items:center;gap:11px;padding:9px 23px 9px 10px;
  margin-top:17px;border-radius:40px;background:var(--dark);
  border:2px solid var(--sage);color:white;font-size:14px;font-weight:bold;
  cursor:pointer;box-shadow:0 4px 0 #19370A;transition:.2s
}
.logout-button:hover{background:var(--teal);transform:translateY(-2px)}
.logout-icon{
  background:var(--sage);width:32px;height:32px;display:flex;
  align-items:center;justify-content:center;border-radius:50%;font-size:19px
}
@media(max-width:750px){
  body{padding:13px}.grid{grid-template-columns:1fr}
  .weight-card,.workflow-card,.impact-card,.history-card,.analysis-card{grid-column:span 1}
  .workflow-grid{grid-template-columns:repeat(2,1fr)}
  .impact-grid{grid-template-columns:1fr}
  .ai-grid{grid-template-columns:1fr}
  .weight{font-size:45px}.brand{font-size:30px}
  .header:after{font-size:90px;right:10px}
}
@media(max-width:420px){
  .workflow-grid,.device-grid,.measurement-grid{grid-template-columns:1fr}
  .measurement-buttons{flex-direction:column}
}
</style>
</head>

<body>
<div class="container">

<header class="header">
  <div class="brand">SPPG LOOP</div>
  <div class="tagline">SISTEM PEMANTAUAN SISA MAKANAN BERBASIS IoT</div>
  <div class="header-bottom">
    <div class="admin-label">DASHBOARD ADMIN</div>
    <div class="admin-badge">AKSES ADMIN</div>
  </div>
</header>

<main class="grid">

<section class="card summary-card">
  <div>
    <h3>Status Sistem</h3>
    <div class="status-row">
      <span class="dot" id="statusDot"></span>
      <span class="status" id="systemStatus">MENGHUBUNGKAN...</span>
    </div>
    <p class="summary-text">Informasi kondisi dan koneksi sistem pemantauan.</p>
  </div>
  <div class="summary-icon">◉</div>
</section>

<section class="card summary-card">
  <div>
    <h3>Informasi Pemantauan</h3>
    <p class="summary-text">Pemantauan berat sisa makanan secara berkala melalui sensor Load Cell dan ESP32.</p>
  </div>
  <div class="summary-icon">⌁</div>
</section>

<section class="card weight-card">
  <div class="weight-label">PEMANTAUAN BERAT SISA MAKANAN</div>
  <p class="weight-desc">Penimbangan dilakukan secara bergantian menggunakan satu Load Cell.</p>

  <div class="active-category" id="activeCategory">PENIMBANGAN 1: SISA MAKANAN DAPAT DIMAKAN</div>

  <div class="current-label">BERAT TERDETEKSI SAAT INI</div>
  <div class="weight"><span id="weight">0.00</span> <span class="unit">g</span></div>
  <p class="weight-desc">Berat sisa makanan yang terdeteksi oleh sensor.</p>

  <div class="measurement-grid">
    <div class="measurement-item">
      <strong>SISA DAPAT DIMAKAN</strong>
      <div><span id="edibleWeight">0.00</span> g</div>
    </div>
    <div class="measurement-item">
      <strong>BAGIAN TIDAK DIMAKAN</strong>
      <div><span id="inedibleWeight">0.00</span> g</div>
    </div>
  </div>

  <div class="total-box">
    <span>TOTAL BERAT SISA</span>
    <strong><span id="totalWeight">0.00</span> g</strong>
  </div>

  <p class="weight-desc" id="measurementInfo">Letakkan wadah pertama di atas timbangan.</p>

  <div class="measurement-buttons">
    <button class="save-button" id="saveButton" onclick="saveMeasurement()">SIMPAN & LANJUT</button>
    <button class="reset-button" onclick="resetMeasurement()">RESET PENIMBANGAN</button>
  </div>
</section>

<section class="card history-card">
  <h3>RIWAYAT PENIMBANGAN</h3>
  <p class="section-subtitle">Riwayat sesi penimbangan yang tersimpan di Firebase Realtime Database.</p>
  <button class="refresh-button" onclick="loadHistory()">↻ MUAT ULANG RIWAYAT</button>
  <div class="table-wrap">
    <table>
      <thead>
        <tr>
          <th>TANGGAL</th>
          <th>WAKTU</th>
          <th>DAPAT DIMAKAN</th>
          <th>TIDAK DIMAKAN</th>
          <th>TOTAL</th>
        </tr>
      </thead>
      <tbody id="historyBody">
        <tr><td colspan="5">Memuat riwayat...</td></tr>
      </tbody>
    </table>
  </div>
  <p class="history-status" id="historyStatus">Mengambil data dari Firebase...</p>
</section>

<section class="card analysis-card">
  <div class="ai-head">
    <div>
      <h3>ANALISIS AI SISA MAKANAN</h3>
      <p class="section-subtitle">Membaca seluruh data penimbangan di Firebase, lalu menyimpulkan sisa makanan yang masih dapat dimakan dan sisa yang sudah tidak dapat dimakan (misalnya sudah dimakan setengah orang lain atau tercampur sisa piring).</p>
    </div>
    <button class="ai-button" id="aiButton" onclick="analyzeWithAI()">🤖 ANALISIS AI</button>
  </div>
  <p class="ai-status" id="aiStatus">Tekan tombol ANALISIS AI untuk memulai. Pastikan perangkat terhubung ke internet.</p>
  <div class="ai-result" id="aiResult"></div>
</section>

<section class="card">
  <h3>Informasi Perangkat</h3>
  <div class="device-grid">
    <div class="device-item"><strong>ESP32</strong><p>Mikrokontroler pengolah data.</p></div>
    <div class="device-item"><strong>LOAD CELL</strong><p>Sensor pengukur berat hingga 5 KG.</p></div>
    <div class="device-item"><strong>HX711</strong><p>Modul penguat sinyal sensor.</p></div>
    <div class="device-item"><strong id="connection">Memeriksa...</strong><p>Status koneksi perangkat.</p></div>
  </div>
</section>

<section class="card purpose-card">
  <h3>PENGELOLAAN PANGAN CERDAS</h3>
  <p>SPPG LOOP membantu memantau sisa makanan sebagai bagian dari upaya pengelolaan pangan yang lebih efisien dan berkelanjutan.</p>
  <div class="purpose-list">
    <div class="purpose-item"><span class="purpose-icon">⚖</span>Pemantauan berat sisa makanan</div>
    <div class="purpose-item"><span class="purpose-icon">⌁</span>Evaluasi data konsumsi</div>
    <div class="purpose-item"><span class="purpose-icon">♧</span>Pengurangan food waste</div>
  </div>
</section>

<section class="card workflow-card">
  <h3>ALUR KERJA SPPG LOOP</h3>
  <p class="section-subtitle">Tahapan pemantauan dan pengolahan data sisa makanan.</p>
  <div class="workflow-grid">
    <div class="workflow-item"><div class="number">01</div><strong>PENGUKURAN</strong><p>Load Cell mengukur berat sisa makanan.</p></div>
    <div class="workflow-item"><div class="number">02</div><strong>PEMBACAAN DATA</strong><p>HX711 membaca sinyal dari sensor.</p></div>
    <div class="workflow-item"><div class="number">03</div><strong>PENGOLAHAN</strong><p>ESP32 mengolah data hasil pengukuran.</p></div>
    <div class="workflow-item"><div class="number">04</div><strong>PEMANTAUAN</strong><p>Data ditampilkan melalui dasbor web.</p></div>
    <div class="workflow-item"><div class="number">05</div><strong>EVALUASI</strong><p>Data digunakan untuk evaluasi konsumsi.</p></div>
  </div>
</section>

<section class="card impact-card">
  <h3>MANFAAT YANG DIHARAPKAN</h3>
  <p class="section-subtitle">Mendukung pengelolaan makanan yang lebih terukur.</p>
  <div class="impact-grid">
    <div class="impact-item"><div class="impact-icon">🍱</div><strong>EFISIENSI PORSI</strong><p>Mendukung penyesuaian jumlah porsi berdasarkan evaluasi sisa makanan.</p></div>
    <div class="impact-item"><div class="impact-icon">♻</div><strong>PENGURANGAN LIMBAH</strong><p>Membantu mengidentifikasi potensi pemborosan makanan.</p></div>
    <div class="impact-item"><div class="impact-icon">🌱</div><strong>KEBERLANJUTAN</strong><p>Mendukung pengelolaan pangan yang lebih bertanggung jawab.</p></div>
  </div>
</section>

</main>

<footer class="footer">
  <div class="footer-title">SPPG LOOP</div>
  <p>SAPIENTIA VIRIDIS | GONZAGA COLLEGE 2026</p>
  <p>Sistem Pemantauan untuk Pengelolaan Pangan Berkelanjutan</p>
  <button class="logout-button" onclick="logout()">
    <span class="logout-icon">↪</span> LOGOUT
  </button>
</footer>

</div>

<script>
let currentWeight = 0;
let savedEdible = null;
let savedInedible = null;
let activeCategory = "edible";
let saving = false;

function updateDisplay() {
  document.getElementById("weight").innerText = currentWeight.toFixed(2);
  document.getElementById("edibleWeight").innerText =
    savedEdible === null ? "0.00" : savedEdible.toFixed(2);
  document.getElementById("inedibleWeight").innerText =
    savedInedible === null ? "0.00" : savedInedible.toFixed(2);

  const total = (savedEdible || 0) + (savedInedible || 0);
  document.getElementById("totalWeight").innerText = total.toFixed(2);
}

function updateCategory() {
  const category = document.getElementById("activeCategory");
  const info = document.getElementById("measurementInfo");

  if (activeCategory === "edible") {
    category.innerText = "PENIMBANGAN 1: SISA MAKANAN DAPAT DIMAKAN";
    info.innerText = "Letakkan wadah sisa makanan yang dapat dimakan di atas timbangan.";
  } else {
    category.innerText = "PENIMBANGAN 2: BAGIAN TIDAK DIMAKAN";
    info.innerText = "Ganti wadah dengan bagian tidak dimakan, lalu pastikan timbangan sudah di-nol-kan.";
  }
}

async function updateData() {
  try {
    const response = await fetch("/data");
    if (!response.ok) throw new Error("Koneksi gagal");

    const data = await response.json();
    const weight = Number(data.weight);
    if (!Number.isFinite(weight)) throw new Error("Data berat tidak valid");

    currentWeight = weight;
    document.getElementById("connection").innerText = "Terhubung";
    document.getElementById("systemStatus").innerText = "ONLINE";
    document.getElementById("statusDot").style.background = "#4C9B50";
    document.getElementById("systemStatus").style.color = "#315D18";
    updateDisplay();
  } catch (error) {
    document.getElementById("connection").innerText = "Terputus";
    document.getElementById("systemStatus").innerText = "OFFLINE";
    document.getElementById("statusDot").style.background = "#D94B4B";
    document.getElementById("systemStatus").style.color = "#B52B2B";
  }
}

async function saveMeasurement() {
  if (saving) return;

  if (savedEdible !== null && savedInedible !== null) {
    alert("Kedua kategori sudah ditimbang. Silakan reset untuk memulai lagi.");
    return;
  }

  if (activeCategory === "edible") {
    savedEdible = currentWeight;
    activeCategory = "inedible";
    updateCategory();
    updateDisplay();

    alert(
      "Berat dapat dimakan tersimpan sementara: " +
      savedEdible.toFixed(2) +
      " gram. Ganti wadah dan lakukan tare sebelum penimbangan kedua."
    );
    return;
  }

  const inedibleCandidate = currentWeight;
  if (savedEdible === null) {
    alert("Timbang kategori dapat dimakan terlebih dahulu.");
    return;
  }

  saving = true;
  const button = document.getElementById("saveButton");
  button.disabled = true;
  button.innerText = "MENYIMPAN...";

  try {
    const formData = new URLSearchParams();
    formData.append("edible", String(savedEdible));
    formData.append("inedible", String(inedibleCandidate));

    const response = await fetch("/save", {
      method: "POST",
      headers: {"Content-Type": "application/x-www-form-urlencoded"},
      body: formData
    });

    const result = await response.json();
    if (!response.ok || !result.success) {
      throw new Error(result.message || "Penyimpanan gagal");
    }

    savedInedible = inedibleCandidate;
    updateDisplay();
    await loadHistory();
    alert("Berhasil! Satu sesi penimbangan tersimpan di Firebase.");
  } catch (error) {
    alert("Gagal menyimpan ke Firebase. Data belum dicatat sebagai berhasil. Cek koneksi dan Serial Monitor.");
    console.error(error);
  } finally {
    saving = false;
    button.disabled = false;
    button.innerText = "SIMPAN & LANJUT";
  }
}

function resetMeasurement() {
  if (!confirm("Apakah kamu yakin ingin menghapus hasil penimbangan sementara?")) return;

  savedEdible = null;
  savedInedible = null;
  activeCategory = "edible";
  updateCategory();
  updateDisplay();
}

async function loadHistory() {
  const body = document.getElementById("historyBody");
  const status = document.getElementById("historyStatus");

  body.innerHTML = '<tr><td colspan="5">Memuat riwayat...</td></tr>';

  try {
    const response = await fetch("/history");
    if (!response.ok) throw new Error("Gagal membaca Firebase");

    const history = await response.json();
    body.innerHTML = "";

    if (!history || Object.keys(history).length === 0) {
      body.innerHTML = '<tr><td colspan="5">Belum ada riwayat penimbangan.</td></tr>';
      status.innerText = "Belum ada data tersimpan.";
      return;
    }

    const entries = Object.entries(history).sort((a, b) =>
      b[0].localeCompare(a[0])
    );

    entries.forEach(([key, item]) => {
      const row = document.createElement("tr");
      const values = [
        item.date || "-",
        item.time || "-",
        Number(item.edible || 0).toFixed(2) + " g",
        Number(item.inedible || 0).toFixed(2) + " g",
        Number(item.total || 0).toFixed(2) + " g"
      ];

      values.forEach(value => {
        const cell = document.createElement("td");
        cell.textContent = value;
        row.appendChild(cell);
      });

      body.appendChild(row);
    });

    status.innerText = "Total " + entries.length + " sesi penimbangan.";
  } catch (error) {
    body.innerHTML = '<tr><td colspan="5">Riwayat belum dapat dimuat.</td></tr>';
    status.innerText = "Gagal mengambil data. Periksa koneksi Firebase.";
    console.error(error);
  }
}

// ==========================================
// ANALISIS AI
// ==========================================

const AI_ENDPOINT = "https://ai.sumopod.com/v1/chat/completions";
const AI_MODEL = "deepseek-v4-flash";
const AI_API_KEY = "sk-PklOVJHytNEmJDXWs6otZw";
const AI_TIMEOUT_MS = 90000;

let analyzing = false;

function historyToEntries(history) {
  return Object.entries(history || {})
    .map(([key, item]) => {
      const edible = Number(item && item.edible) || 0;
      const inedible = Number(item && item.inedible) || 0;
      return {
        key: key,
        date: String((item && item.date) || "-"),
        time: String((item && item.time) || "-"),
        edible: edible,
        inedible: inedible,
        total: edible + inedible
      };
    })
    .sort((a, b) => String(b.key).localeCompare(String(a.key)));
}

function buildAIPrompt(entries) {
  const totalEdible = entries.reduce((sum, e) => sum + e.edible, 0);
  const totalInedible = entries.reduce((sum, e) => sum + e.inedible, 0);
  const total = totalEdible + totalInedible;
  const pct = value => total > 0 ? (value / total * 100).toFixed(1) : "0.0";

  const perDate = {};
  entries.forEach(entry => {
    if (!perDate[entry.date]) perDate[entry.date] = { edible: 0, inedible: 0, count: 0 };
    perDate[entry.date].edible += entry.edible;
    perDate[entry.date].inedible += entry.inedible;
    perDate[entry.date].count += 1;
  });

  const sessionLines = entries.map(entry =>
    "- " + entry.date + " " + entry.time +
    " | edible " + entry.edible.toFixed(2) + " g" +
    " | inedible " + entry.inedible.toFixed(2) + " g" +
    " | total " + entry.total.toFixed(2) + " g"
  ).join("\n");

  const dateLines = Object.entries(perDate).map(([date, value]) =>
    "- " + date + " | edible " + value.edible.toFixed(2) + " g" +
    " | inedible " + value.inedible.toFixed(2) + " g" +
    " | total " + (value.edible + value.inedible).toFixed(2) + " g" +
    " | " + value.count + " sesi"
  ).join("\n");

  return [
    "Kamu adalah ahli gizi dan keamanan pangan (food safety) yang membantu program SPPG (Sekolah Penggerak Program Gizi) di Indonesia.",
    "",
    "Sistem SPPG LOOP menimbang SISA MAKANAN dengan load cell. Sistem HANYA mengukur berat (gram), bukan jenis makanan.",
    "Arti kategori data:",
    "- edible   = sisa makanan yang secara kategori masih BAIK / belum tersentuh / masih bisa disajikan atau dimakan.",
    "- inedible = bagian yang TIDAK BISA dimakan: sisa di piring, makanan yang sudah dimakan setengah oleh orang lain, kulit, tulang, bumbu, dan sejenisnya.",
    "",
    "DATA FIREBASE (" + entries.length + " sesi, diurutkan dari yang terbaru):",
    sessionLines,
    "",
    "REKAP PER TANGGAL:",
    dateLines,
    "",
    "TOTAL KESELURUHAN:",
    "- edible    : " + totalEdible.toFixed(2) + " g (" + pct(totalEdible) + "%)",
    "- inedible  : " + totalInedible.toFixed(2) + " g (" + pct(totalInedible) + "%)",
    "- total sisa: " + total.toFixed(2) + " g",
    "",
    "TUGAS ANALISIS:",
    "1. Tentukan sisa makanan yang MASIH DAPAT dimakan (aman disajikan atau dikonsumsi ulang) beserta alasan dan tindakannya.",
    "2. Tentukan sisa makanan yang SUDAH TIDAK DAPAT dimakan, termasuk sisa yang sudah dimakan setengah orang lain, sisa piring, sudah bercampur, atau terlalu lama, beserta alasan dan tindakannya.",
    "3. Beri rekomendasi operasional singkat untuk dapur SPPG.",
    "4. Beri skor kelayakan 0-100 untuk kondisi sisa makanan saat ini (0 = tidak ada yang layak, 100 = semua layak).",
    "5. Gunakan angka dari data bila relevan. Jangan mengarang jenis menu spesifik; sebut kategori atau kelompok makanan secara umum.",
    "6. Jika data kosong atau terlalu sedikit, sampaikan keterbatasannya dengan jujur.",
    "",
    "Jawab HANYA dengan JSON valid tanpa markdown dan tanpa teks tambahan, dengan struktur:",
    '{"kesimpulan":"...","skor_kelayakan":0,"bisa_dimakan":[{"kategori":"...","alasan":"...","tindakan":"..."}],"tidak_bisa_dimakan":[{"kategori":"...","alasan":"...","tindakan":"..."}],"rekomendasi":["..."]}',
    "Gunakan bahasa Indonesia yang jelas, singkat, dan mudah dipahami."
  ].join("\n");
}

function parseAIJson(text) {
  let clean = String(text || "").trim();
  clean = clean.replace(/^```(?:json)?/i, "").replace(/```$/, "").trim();

  const start = clean.indexOf("{");
  const end = clean.lastIndexOf("}");
  if (start === -1 || end === -1 || end <= start) return null;

  try {
    return JSON.parse(clean.slice(start, end + 1));
  } catch (error) {
    return null;
  }
}

function appendAIBox(container, title, items, isGood) {
  const box = document.createElement("div");
  box.className = "ai-box" + (isGood ? "" : " no");

  const heading = document.createElement("h4");
  heading.textContent = title;
  box.appendChild(heading);

  const list = Array.isArray(items) ? items : [];

  if (list.length === 0) {
    const empty = document.createElement("p");
    empty.className = "ai-item-empty";
    empty.textContent = "Tidak ada data pada kategori ini.";
    box.appendChild(empty);
    container.appendChild(box);
    return;
  }

  list.forEach(item => {
    const row = document.createElement("div");
    row.className = "ai-item";

    if (typeof item === "string") {
      const text = document.createElement("p");
      text.textContent = item;
      row.appendChild(text);
    } else {
      const name = document.createElement("strong");
      name.textContent = item.kategori || item.jenis || "Sisa makanan";
      row.appendChild(name);

      if (item.alasan) {
        const reason = document.createElement("p");
        reason.textContent = item.alasan;
        row.appendChild(reason);
      }

      if (item.tindakan) {
        const action = document.createElement("p");
        action.className = "ai-action";
        action.textContent = "Tindakan: " + item.tindakan;
        row.appendChild(action);
      }
    }

    box.appendChild(row);
  });

  container.appendChild(box);
}

function renderAIRawText(text) {
  const result = document.getElementById("aiResult");
  result.innerHTML = "";

  const pre = document.createElement("p");
  pre.className = "ai-raw";
  pre.textContent = text;
  result.appendChild(pre);
}

function renderAIResult(data, entries) {
  const result = document.getElementById("aiResult");
  result.innerHTML = "";

  const score = Math.max(0, Math.min(100, Math.round(Number(data.skor_kelayakan) || 0)));

  const scoreRow = document.createElement("div");
  scoreRow.className = "ai-score";

  const badge = document.createElement("div");
  badge.className = "ai-score-badge";
  badge.textContent = String(score);
  badge.style.background = score >= 70 ? "#315D18" : (score >= 40 ? "#B07F1F" : "#A93C2C");

  const info = document.createElement("div");
  info.className = "ai-score-info";

  const label = document.createElement("strong");
  label.textContent = "SKOR KELAYAKAN SISA MAKANAN (0-100)";

  const bar = document.createElement("div");
  bar.className = "ai-score-bar";
  const fill = document.createElement("span");
  fill.style.width = score + "%";
  fill.style.background = score >= 70 ? "#4C9B50" : (score >= 40 ? "#D39B32" : "#C24A3A");
  bar.appendChild(fill);

  const note = document.createElement("p");
  if (score >= 70) {
    note.textContent = "Sebagian besar sisa makanan masih layak. Tetap lakukan verifikasi fisik dan suhu sebelum disajikan ulang.";
  } else if (score >= 40) {
    note.textContent = "Sebagian sisa makanan masih layak, sebagian harus dibuang. Perlu pemilahan manual sebelum digunakan.";
  } else {
    note.textContent = "Sebagian besar sisa makanan sudah tidak layak dikonsumsi ulang dan sebaiknya diolah sebagai limbah organik.";
  }

  info.append(label, bar, note);
  scoreRow.append(badge, info);
  result.appendChild(scoreRow);

  if (data.kesimpulan) {
    const conclusion = document.createElement("div");
    conclusion.className = "ai-conclusion";

    const title = document.createElement("strong");
    title.textContent = "KESIMPULAN";

    const text = document.createElement("p");
    text.textContent = data.kesimpulan;

    conclusion.append(title, text);
    result.appendChild(conclusion);
  }

  const grid = document.createElement("div");
  grid.className = "ai-grid";
  appendAIBox(grid, "MASIH DAPAT DIMAKAN", data.bisa_dimakan, true);
  appendAIBox(grid, "SUDAH TIDAK DAPAT DIMAKAN", data.tidak_bisa_dimakan, false);
  result.appendChild(grid);

  const recommendations = Array.isArray(data.rekomendasi) ? data.rekomendasi : [];
  if (recommendations.length > 0) {
    const box = document.createElement("div");
    box.className = "ai-reco";

    const title = document.createElement("strong");
    title.textContent = "REKOMENDASI UNTUK DAPUR SPPG";

    const list = document.createElement("ul");
    recommendations.forEach(value => {
      const item = document.createElement("li");
      item.textContent = typeof value === "string" ? value : JSON.stringify(value);
      list.appendChild(item);
    });

    box.append(title, list);
    result.appendChild(box);
  }

  const meta = document.createElement("p");
  meta.className = "ai-meta";
  meta.textContent = "Dianalisis dari " + entries.length + " sesi penimbangan | " +
    new Date().toLocaleString("id-ID");
  result.appendChild(meta);
}

async function analyzeWithAI() {
  if (analyzing) return;

  const button = document.getElementById("aiButton");
  const status = document.getElementById("aiStatus");
  const result = document.getElementById("aiResult");

  analyzing = true;
  button.disabled = true;
  const defaultLabel = button.innerHTML;
  button.innerHTML = "⏳ MENGANALISIS...";
  status.innerText = "Membaca seluruh data dari Firebase...";
  result.innerHTML = "";

  const controller = new AbortController();
  const timer = setTimeout(() => controller.abort(), AI_TIMEOUT_MS);

  try {
    const historyResponse = await fetch("/history");
    if (!historyResponse.ok) {
      throw new Error("Data Firebase belum dapat dibaca. Pastikan perangkat online.");
    }

    const history = await historyResponse.json();
    const entries = historyToEntries(history);

    if (entries.length === 0) {
      status.innerText = "Belum ada data penimbangan untuk dianalisis.";
      return;
    }

    status.innerText = "Mengirim " + entries.length + " sesi penimbangan ke AI, mohon tunggu...";

    const aiResponse = await fetch(AI_ENDPOINT, {
      method: "POST",
      signal: controller.signal,
      headers: {
        "Content-Type": "application/json",
        "Authorization": "Bearer " + AI_API_KEY
      },
      body: JSON.stringify({
        model: AI_MODEL,
        messages: [
          {
            role: "system",
            content: "Kamu adalah ahli gizi dan keamanan pangan. Selalu menjawab dengan JSON valid tanpa markdown."
          },
          {
            role: "user",
            content: buildAIPrompt(entries)
          }
        ],
        max_tokens: 1600,
        temperature: 0.3
      })
    });

    if (!aiResponse.ok) {
      const detail = await aiResponse.text().catch(() => "");
      throw new Error("Layanan AI menolak permintaan (" + aiResponse.status + "). " + detail.slice(0, 160));
    }

    const payload = await aiResponse.json();
    const content = payload && payload.choices && payload.choices[0] &&
      payload.choices[0].message
      ? String(payload.choices[0].message.content || "").trim()
      : "";

    if (content.length === 0) {
      throw new Error("AI tidak mengembalikan jawaban.");
    }

    const parsed = parseAIJson(content);
    if (parsed) {
      renderAIResult(parsed, entries);
      status.innerText = "Analisis selesai dari " + entries.length + " sesi penimbangan.";
    } else {
      renderAIRawText(content);
      status.innerText = "Analisis selesai (format teks).";
    }
  } catch (error) {
    if (error.name === "AbortError") {
      status.innerText = "Analisis melebihi batas waktu. Coba lagi.";
    } else {
      status.innerText = "Analisis gagal: " + error.message;
    }
    console.error(error);
  } finally {
    clearTimeout(timer);
    analyzing = false;
    button.disabled = false;
    button.innerHTML = defaultLabel;
  }
}

async function logout() {
  try {
    await fetch("/logout");
  } catch (error) {
    console.log("Permintaan logout gagal.");
  }
  window.location.reload();
}

updateCategory();
updateDisplay();
updateData();
loadHistory();

setInterval(updateData, 1000);
</script>
</body>
</html>
)rawliteral";
