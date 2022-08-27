var btSerial = new (require("bluetooth-serial-port").BluetoothSerialPort)();

btSerial.on("found", function (address, name) {
    console.log("found", address)
  btSerial.findSerialPortChannel(
    address,
    function (channel) {
        console.log("channel found", address)
      btSerial.connect(
        address,
        channel,
        function () {
          console.log("connected");
          
          btSerial.write(Buffer.from("toggle|1\n", "utf-8"), function (
            err,
            bytesWritten
          ) {
            if (err) console.log(err);
            btSerial.write(Buffer.from("getAutomaticMode|2\n", "utf-8"), function (
                err,
                bytesWritten
              ) {
                if (err) console.log(err);
                btSerial.write(Buffer.from("setAutomaticMode|3|true\n", "utf-8"), function (
                  err,
                  bytesWritten
                ) {
                  if (err) console.log(err);
                });
              });
            });

          btSerial.on("data", function (buffer) {
            console.log(buffer.toString("utf-8"));
          });
        },
        function () {
          console.log("cannot connect");
        }
      );

      // close the connection when you're ready
      btSerial.close();
    },
    function () {
      console.log("found nothing");
    }
  );
});

console.log(btSerial)

btSerial.on("failure", err=>console.error(err));
btSerial.on("error", err=>console.error(err));
btSerial.on("closed", err=>console.log("close"));

btSerial.inquire();