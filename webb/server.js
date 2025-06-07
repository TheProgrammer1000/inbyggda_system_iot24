// server.js
import { createBluetooth } from 'node-ble';
import express             from 'express';
import http                from 'http';
import { Server as IOServer } from 'socket.io';

const { bluetooth } = createBluetooth();
const app     = express();
const server  = http.createServer(app);
const io      = new IOServer(server);
const port    = 3001;

app.use(express.static('public'));
app.use(express.json());

// UUIDs...
const SERVICE_UUID = '00000180-0000-1000-8000-00805f9b34fb';
const READ_UUID    = '0000fef4-0000-1000-8000-00805f9b34fb';
const WRITE_UUID   = '0000dead-0000-1000-8000-00805f9b34fb';
const NOTIFY_UUID  = '0000beef-0000-1000-8000-00805f9b34fb';

let controlChar;  // make this visible to the socket handler

async function initBLE() {
  const adapter = await bluetooth.defaultAdapter();
  if (!await adapter.isDiscovering()) {
    await adapter.startDiscovery();
  }

  console.log("Waiting on the device...");
  const device = await adapter.waitDevice('40:4C:CA:4D:C9:82');
  if (await adapter.isDiscovering()) {
    await adapter.stopDiscovery();
  }

  console.log("✅ Found device, connecting…");
  await device.connect();

  const gatt = await device.gatt();
  const svc  = await gatt.getPrimaryService(SERVICE_UUID);

  // grab each characteristic
  await svc.getCharacteristic(READ_UUID); // if you need to read once
  controlChar = await svc.getCharacteristic(WRITE_UUID);
  const notifyChar = await svc.getCharacteristic(NOTIFY_UUID);

  // notifications
  await notifyChar.startNotifications(buffer => {
    const status = buffer.toString('utf8').trim();
    io.emit('deviceData', { ts: Date.now(), value: status });
  });

  console.log("✅ BLE fully initialized");
}

initBLE().catch(err => {
  console.error("BLE init failed:", err);
  process.exit(1);
});

// Socket.IO handler
io.on('connection', socket => {
  socket.on('setState', async state => {
    const buf = Buffer.from(state, 'utf8');
    await controlChar.writeValue(buf);
  });
});

server.listen(port, () => {
  console.log(`🚀 Listening on http://localhost:${port}`);
});
