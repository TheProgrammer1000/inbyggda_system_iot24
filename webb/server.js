import express from 'express';
import http from 'http';
import { Server as IOServer } from 'socket.io';
import { io } from 'socket.io-client';  // Import io from socket.io-client
// Connect to the Linux server using the socket



const app = express();
const server = http.createServer(app);
const port = 3000;
const ioServer = new IOServer(server);


// Serve static files (HTML, JS, etc.)
app.use(express.static('public'));

// Set up Socket.IO client to connect to the Linux BLE server
const espSocket = io('http://192.168.0.47:3001');  // Change to your Linux server IP

// Relay messages from ESP32 to connected web clients
espSocket.on('deviceData', (data) => {
    console.log(`Received from ESP32: ${JSON.stringify(data)}`);
    ioServer.emit('deviceData', data);
  });
  
  // Handle commands from web clients and send them to ESP32
  ioServer.on('connection', (socket) => {
    console.log('Web client connected');
  
    socket.on('setState', (state) => {
      console.log(`Command from web client: ${state}`);
      espSocket.emit('setState', state);
    });
  });
server.listen(port, () => {
  console.log(`Frontend server running at http://localhost:${port}`);
});
