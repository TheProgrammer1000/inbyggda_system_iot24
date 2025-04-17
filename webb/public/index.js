const socket = io();

const statusEl = document.getElementById('status');
const btnOn = document.getElementById('btnOn');
const btnOff = document.getElementById('btnOff');

// Update status when receiving data from the server
socket.on('deviceData', ({ ts, value }) => {
  statusEl.textContent = value;
  console.log(`Device status updated: ${value} at ${new Date(ts).toLocaleTimeString()}`);
});

// Send commands to the server
btnOn.addEventListener('click', () => {
  socket.emit('setState', 'on');
});

btnOff.addEventListener('click', () => {
  socket.emit('setState', 'off');
});
