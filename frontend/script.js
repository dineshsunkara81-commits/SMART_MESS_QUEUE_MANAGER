// State
let queue = [];
let history = [];
let currentToken = 1;
let isAdmin = false;

// Load data on start
document.addEventListener('DOMContentLoaded', function() {
    createParticles();
    updateClock();
    setInterval(updateClock, 1000);
    loadData();
    updateStats();
});

// Particles
function createParticles() {
    const container = document.getElementById('particles');
    for (let i = 0; i < 40; i++) {
        const p = document.createElement('div');
        p.className = 'particle';
        const size = Math.random() * 6 + 2;
        p.style.width = size + 'px';
        p.style.height = size + 'px';
        p.style.left = Math.random() * 100 + '%';
        p.style.animationDuration = (Math.random() * 20 + 15) + 's';
        p.style.animationDelay = (Math.random() * 15) + 's';
        container.appendChild(p);
    }
}

// Clock
function updateClock() {
    const now = new Date();
    document.getElementById('currentTime').textContent = 
        now.toLocaleTimeString('en-US', { hour: '2-digit', minute: '2-digit', second: '2-digit', hour12: true });
}

// Data
function loadData() {
    const q = localStorage.getItem('queue');
    const h = localStorage.getItem('history');
    const t = localStorage.getItem('token');
    if (q) queue = JSON.parse(q);
    if (h) history = JSON.parse(h);
    if (t) currentToken = parseInt(t);
}

function saveData() {
    localStorage.setItem('queue', JSON.stringify(queue));
    localStorage.setItem('history', JSON.stringify(history));
    localStorage.setItem('token', String(currentToken));
    updateStats();
}

function updateStats() {
    document.getElementById('totalWaiting').textContent = queue.length;
    document.getElementById('totalServed').textContent = history.length;
    if (history.length > 0) {
        const avg = Math.round(history.reduce((s, h) => s + h.waitTime, 0) / history.length);
        document.getElementById('avgTime').textContent = avg;
    } else {
        document.getElementById('avgTime').textContent = '0';
    }
}

// Navigation
function showScreen(id) {
    ['welcomeScreen', 'studentMenu', 'adminLogin', 'adminMenu', 'resultDisplay']
        .forEach(s => document.getElementById(s).style.display = 'none');
    document.getElementById(id).style.display = 'block';
    document.getElementById(id).style.animation = 'fadeIn 0.4s ease';
}

function goBack() {
    isAdmin  = false;
    showScreen('welcomeScreen');
}

function showStudentMenu() {
    isAdmin = false;
    showScreen('studentMenu');
}

function showAdminLogin() {
    showScreen('adminLogin');
}

function showAdminMenu() {
    isAdmin = true;
    showScreen('adminMenu');
}

// Student Functions
function generateToken() {
    const name = prompt('Enter Student Name:');
    if (!name || name.trim() === '') {
        showResult('error', 'Invalid Name', 'Please enter a valid name.');
        return;
    }
    const token = { number: currentToken++, name: name.trim(), timestamp: Date.now() };
    queue.push(token);
    saveData();
    showSMS(token.number, 'Token generated successfully!');
    showResult('success', '✅ Token Generated!',
        `<span class="token-number">#${token.number}</span>
         <p>Student: <strong>${token.name}</strong></p>
         <p>Position: <strong>${queue.length}</strong></p>`
    );
}

function viewQueuePosition() {
    if (queue.length === 0) {
        showResult('info', 'Queue Empty', 'No students waiting.');
        return;
    }
    const num = prompt('Enter Token Number:');
    if (!num) return;
    const idx = queue.findIndex(t => t.number === parseInt(num));
    if (idx === -1) {
        showResult('error', 'Not Found', `Token #${num} not found.`);
        return;
    }
    showResult('success', 'Your Position',
        `<span class="token-number">#${idx + 1}</span>
         <p>Token #${queue[idx].number} - ${queue[idx].name}</p>
         <p>People ahead: <strong>${idx}</strong></p>`
    );
}

function viewWaitingTime() {
    if (queue.length === 0) {
        showResult('info', 'Queue Empty', 'No students waiting.');
        return;
    }
    const num = prompt('Enter Token Number:');
    if (!num) return;
    const idx = queue.findIndex(t => t.number === parseInt(num));
    if (idx === -1) {
        showResult('error', 'Not Found', `Token #${num} not found.`);
        return;
    }
    const position = idx + 1;
    let time = history.length > 0 ? Math.round(history.reduce((s, h) => s + h.waitTime, 0) / history.length) * position : position * 5;
    if (time < 1) time = 1;
    if (time > 120) time = 120;
    let crowd = position <= 5 ? '🟢 LOW' : position <= 15 ? '🟡 MEDIUM' : position <= 30 ? '🟠 HIGH' : '🔴 VERY HIGH';
    showResult('ai', '🤖 AI Prediction',
        `<p>Token #${queue[idx].number} - ${queue[idx].name}</p>
         <p>Position: <strong>#${position}</strong></p>
         <p>Wait Time: <strong>${time} min</strong></p>
         <p>Crowd: <strong>${crowd}</strong></p>`
    );
}

function cancelToken() {
    if (queue.length === 0) {
        showResult('info', 'Queue Empty', 'No tokens to cancel.');
        return;
    }
    const num = prompt('Enter Token Number to Cancel:');
    if (!num) return;
    const idx = queue.findIndex(t => t.number === parseInt(num));
    if (idx === -1) {
        showResult('error', 'Not Found', `Token #${num} not found.`);
        return;
    }
    const token = queue.splice(idx, 1)[0];
    saveData();
    showResult('success', 'Cancelled', `<p>Token #${token.number} for <strong>${token.name}</strong> cancelled.</p>`);
}

function viewHistory() {
    if (history.length === 0) {
        showResult('info', 'No History', 'No tokens served yet.');
        return;
    }
    let html = '<table style="width:100%;color:#fff;font-size:13px;border-collapse:collapse;">';
    html += '<tr style="border-bottom:1px solid rgba(255,255,255,0.2);"><th style="padding:6px;text-align:left;">Token</th><th style="padding:6px;text-align:left;">Student</th><th style="padding:6px;text-align:right;">Wait</th></tr>';
    history.slice(-8).reverse().forEach(h => {
        html += `<tr><td style="padding:6px;">#${h.number}</td><td style="padding:6px;">${h.name}</td><td style="padding:6px;text-align:right;">${h.waitTime}min</td></tr>`;
    });
    html += '</table>';
    showResult('history', '📜 Token History', html);
}

// Admin Functions
function adminLogin() {
    const u = document.getElementById('adminUser').value;
    const p = document.getElementById('adminPass').value;
    if (u === 'admin' && p === 'admin123') {
        isAdmin = true;
        showAdminMenu();
        showResult('success', '✅ Login Successful', 'Welcome to Admin Panel!');
    } else {
        showResult('error', 'Login Failed', 'Invalid username or password.');
    }
}

function adminViewQueue() {
    if (queue.length === 0) {
        showResult('info', 'Queue Empty', 'No students waiting.');
        return;
    }
    let html = '<table style="width:100%;color:#fff;font-size:13px;border-collapse:collapse;">';
    html += '<tr style="border-bottom:1px solid rgba(255,255,255,0.2);"><th style="padding:6px;text-align:left;">Pos</th><th style="padding:6px;text-align:left;">Token</th><th style="padding:6px;text-align:left;">Student</th></tr>';
    queue.forEach((t, i) => {
        html += `<tr><td style="padding:6px;">#${i+1}</td><td style="padding:6px;">${t.number}</td><td style="padding:6px;">${t.name}</td></tr>`;
    });
    html += '</table>';
    showResult('queue', '📋 Current Queue', html);
}

function adminCallNext() {
    if (queue.length === 0) {
        showResult('info', 'Queue Empty', 'No tokens to call.');
        return;
    }
    const token = queue.shift();
    const wait = Math.round((Date.now() - token.timestamp) / 60000) || 1;
    history.push({ number: token.number, name: token.name, waitTime: wait, timestamp: Date.now() });
    saveData();
    showSMS(token.number, 'Please come to the mess counter now!');
    showResult('success', '✅ Called Next',
        `<span class="token-number">#${token.number}</span>
         <p>Student: <strong>${token.name}</strong></p>
         <p>Wait Time: <strong>${wait} min</strong></p>`
    );
}

function adminViewReports() {
    const served = history.length;
    const avg = served > 0 ? Math.round(history.reduce((s, h) => s + h.waitTime, 0) / served) : 0;
    const today = new Date().toLocaleDateString('en-US', { weekday: 'long', year: 'numeric', month: 'long', day: 'numeric' });
    showResult('reports', '📊 Daily Report',
        `<p>📅 ${today}</p>
         <hr style="border-color:rgba(255,255,255,0.1);margin:10px 0;">
         <p>👥 Served: <strong>${served}</strong></p>
         <p>⏱️ Avg Wait: <strong>${avg} min</strong></p>
         <p>📌 Waiting: <strong>${queue.length}</strong></p>
         <p>🔢 Next Token: <strong>#${currentToken}</strong></p>`
    );
}

// Result
function showResult(type, title, body) {
    const icons = { success: '✅', error: '❌', info: 'ℹ️', ai: '🤖', history: '📜', queue: '📋', reports: '📊' };
    document.getElementById('resultIcon').textContent = icons[type] || '📌';
    document.getElementById('resultTitle').textContent = title;
    document.getElementById('resultBody').innerHTML = body;
    showScreen('resultDisplay');
}

function closeResult() {
    if (isAdmin) showScreen('adminMenu');
    else showScreen('studentMenu');
}

// SMS Modal
function showSMS(token, msg) {
    document.getElementById('smsToken').textContent = token;
    document.getElementById('smsMessage').textContent = msg;
    document.getElementById('smsModal').classList.add('show');
    setTimeout(closeSMS, 5000);
}

function closeSMS() {
    document.getElementById('smsModal').classList.remove('show');
}