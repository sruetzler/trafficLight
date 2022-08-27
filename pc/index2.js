Scanner = require("bluetooth-scanner");

var device = "hci0";

bleScanner = new Scanner(device, function(mac, name) {
    console.log('Found device: ' + name);
});